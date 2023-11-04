/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-27 13:53:59
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/BackEnd/SHM.cpp
 * @Description: 共享内存后端
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <SHM.hpp>
#include <ERROR.hpp>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <Config.hpp>
#include <utils/StringToNum.hpp>
#include <string.h>

/*
* 共享内存部分的内存布局，形成一个循环队列
* 	| (long) p_read | (long) p_read | data ... |
* 其中，读指针的值为当前要读的位置，写指针的值为当前要写的位置
* 读指针==写指针，读阻塞
* 读指针==写指针+1，写阻塞
*
* 写指针只能在 Transmitter 中修改，读指针只能在 Receiver 中修改
*/

Transceiver::SHM::Receiver::Receiver(std::string name,
				     const Config::Config &conf)
	: AbstractReceiver(name)
{
	// 只允许同一个用户之间进行操作
	auto mode = S_IRUSR | S_IWUSR | S_IXUSR;
	this->bufersize = utils::StringToLong(conf["BufferSize"]);
	if (conf["OnlyUser"] == "False")
		mode |= S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH |
			S_IXOTH;
	this->sfd = shm_open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, mode);
	if (this->sfd == -1)
		throw ERROR(std::string("Receiver: shm_open ERROR, name = ") +
			    name);
	if (ftruncate(this->sfd,
		      this->bufersize + sizeof(ringbuffer_matedata)) == -1) {
		this->~Receiver();
		throw ERROR(std::string("Receiver: ftruncate ERROR, name = ") +
			    name);
	}
	this->addr = mmap(NULL, this->bufersize + sizeof(ringbuffer_matedata),
			  PROT_READ | PROT_WRITE, MAP_SHARED, this->sfd, 0);
	if (this->addr == MAP_FAILED) {
		this->~Receiver();
		throw ERROR(std::string("Receiver: mmap ERROR, name = ") +
			    name);
	}
	this->matedata = (ringbuffer_matedata *)this->addr;
	this->data = (char *)this->addr + sizeof(ringbuffer_matedata);
	this->matedata->p_read = 0;
	this->matedata->p_write = 0;
}

long Transceiver::SHM::Receiver::receive(char *buf, long size)
{
	long readSize = 0;
	// 剩余尺寸
	long remainSize;
	// 本次读取的尺寸
	long onceReadSize;
	// 为了防止在处理过程中写指针发生变化导致数据不一致
	long p_write_copy;
	// 为了优化性能，因为 matedata->p_read 为 volatile 类型
	// 但只能由本函数修改
	long p_read_copy;
	while (readSize < size) {
		p_write_copy = matedata->p_write;
		p_read_copy = matedata->p_read;
		remainSize = size - readSize;
		if (p_write_copy > p_read_copy) {
			onceReadSize = (p_write_copy - p_read_copy) >
						       remainSize ?
					       remainSize :
					       (p_write_copy - p_read_copy);
			memcpy(buf + readSize, data + p_read_copy,
			       onceReadSize);
			readSize += onceReadSize;
			matedata->p_read = onceReadSize + p_read_copy;
		} else if (p_write_copy < p_read_copy) {
			onceReadSize = (bufersize - p_read_copy) > remainSize ?
					       remainSize :
					       (bufersize - p_read_copy);
			memcpy(buf + readSize, data + p_read_copy,
			       onceReadSize);
			readSize += onceReadSize;
			matedata->p_read =
				(onceReadSize + p_read_copy) % bufersize;
		}
	}
	return readSize;
}

Transceiver::SHM::Receiver::~Receiver()
{
	close(this->sfd);
	shm_unlink(this->getName().c_str());
}

Transceiver::SHM::Transmitter::Transmitter(std::string dest,
					   const Config::Config &conf)
	: AbstractTransmitter(dest)
{
	this->bufersize = utils::StringToLong(conf["BufferSize"]);
}

void Transceiver::SHM::Transmitter::init()
{
	this->sfd = shm_open(this->getDest().c_str(), O_RDWR, 0);
	if (this->sfd == -1)
		throw ERROR(
			std::string("Transmitter: shm_open ERROR, name = ") +
			this->getDest());
	this->addr = mmap(NULL, this->bufersize + sizeof(ringbuffer_matedata),
			  PROT_READ | PROT_WRITE, MAP_SHARED, this->sfd, 0);
	if (this->addr == MAP_FAILED) {
		this->~Transmitter();
		throw ERROR(std::string("Transmitter: mmap ERROR, name = ") +
			    this->getDest());
	}
	this->matedata = (ringbuffer_matedata *)this->addr;
	this->data = (char *)this->addr + sizeof(ringbuffer_matedata);
	this->inited = true;
}

long Transceiver::SHM::Transmitter::send(const char *buf, long size)
{
	if(!this->inited)
		this->init();
	// 已发送尺寸
	long sendSize = 0;
	// 剩余尺寸
	long remainSize;
	// 本次发送尺寸
	long onceSendSize;
	// 为了防止在处理过程中读指针发生变化导致数据不一致
	long p_read_copy;
	// 为了优化性能，因为 matedata->p_write 为 volatile 类型
	// 但只能由本函数修改
	long p_write_copy;
	while (sendSize < size) {
		p_read_copy = matedata->p_read;
		p_write_copy = matedata->p_write;
		remainSize = size - sendSize;
		if (p_read_copy <= p_write_copy) {
			if (p_read_copy != 0) {
				// 从写指针到最后都能写
				onceSendSize =
					(bufersize - p_write_copy) >
							remainSize ?
						remainSize :
						(bufersize - p_write_copy);
				memcpy(data + p_write_copy, buf + sendSize,
				       onceSendSize);
				matedata->p_write =
					(onceSendSize + p_write_copy) %
					bufersize;
			} else {
				// 最后一格不能写
				onceSendSize =
					(bufersize - p_write_copy) >
							remainSize ?
						remainSize :
						(bufersize - p_write_copy - 1);
				memcpy(data + p_write_copy, buf + sendSize,
				       onceSendSize);
				matedata->p_write = onceSendSize + p_write_copy;
			}
			sendSize += onceSendSize;
		} else {
			// 只能写写指针到读指针-1
			onceSendSize = (p_read_copy - p_write_copy - 1) >
						       remainSize ?
					       remainSize :
					       (p_read_copy - p_write_copy - 1);
			memcpy(data + p_write_copy, buf + sendSize,
			       onceSendSize);
			sendSize += onceSendSize;
			matedata->p_write = onceSendSize + p_write_copy;
		}
	}
	return sendSize;
}

Transceiver::SHM::Transmitter::~Transmitter()
{
	close(this->sfd);
}
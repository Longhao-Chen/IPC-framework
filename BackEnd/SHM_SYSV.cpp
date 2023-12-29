/*
 * @Author: Haoyu Chen <null-qwerty@outlook.com>
 * @Date: 2023-12-30 01:30:12
 * @LastEditors: Haoyu Chen <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/BackEnd/SHM_SYSV.cpp
 * @Description: System V 共享内存后端
 * Copyright (c) 2023 by Haoyu Chen, All Rights Reserved.
 */

#include <Config.hpp>
#include <ERROR.hpp>
#include <SHM_SYSV.hpp>
#include <string.h>
#include <utils/StringToNum.hpp>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

Transceiver::SHM_SYSV::Receiver::Receiver(std::string name,
                                      const Config::Config &conf)
    : AbstractReceiver(name) {
        auto mode = S_IRUSR | S_IWUSR | S_IXUSR;
    	if (conf["OnlyUser"] == "False")
		mode |= S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH |
			S_IXOTH;
	    this->key = shm_open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, mode);
        if (key == -1)
            throw ERROR(std::string("Receiver: ftok ERROR, name = ") + name);
        
        this->BufferSize = utils::StringToLong(conf["BufferSize"]);
        this->shmid = shmget(key, this->BufferSize, IPC_CREAT | 0666);
        if (this->shmid == -1)
            throw ERROR(std::string("Receiver: shmget ERROR, name = ") + name);
        
        this->shmaddr = shmat(this->shmid, NULL, 0);
        if (this->shmaddr == (void *)-1)
            throw ERROR(std::string("Receiver: shmat ERROR, name = ") + name);
    }

long Transceiver::SHM_SYSV::Receiver::receive(char *buf, long size) {
    if (size > this->BufferSize)
        throw ERROR("Receiver: receive ERROR, size > BufferSize");
    memcpy(buf, this->shmaddr, size);
    return size;
}

Transceiver::SHM_SYSV::Receiver::~Receiver() {
    shmdt(this->shmaddr);
    shm_unlink(this->getName().c_str());
}

Transceiver::SHM_SYSV::Transmitter::Transmitter(std::string dest,
                                            const Config::Config &conf)
    : AbstractTransmitter(dest) {
        auto mode = S_IRUSR | S_IWUSR | S_IXUSR;
    	if (conf["OnlyUser"] == "False")
		mode |= S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH |
			S_IXOTH;
	    this->key = shm_open(dest.c_str(), O_RDWR | O_CREAT | O_TRUNC, mode);
        if (key == -1)
            throw ERROR(std::string("Transmitter: ftok ERROR, dest = ") +
                        dest);
        
        this->BufferSize = utils::StringToLong(conf["BufferSize"]);
        this->shmid = shmget(key, this->BufferSize, IPC_CREAT | 0666);
        if (this->shmid == -1)
            throw ERROR(std::string("Transmitter: shmget ERROR, dest = ") +
                        dest);
        
        this->shmaddr = shmat(this->shmid, NULL, 0);
        if (this->shmaddr == (void *)-1)
            throw ERROR(std::string("Transmitter: shmat ERROR, dest = ") +
                        dest);
    }

long Transceiver::SHM_SYSV::Transmitter::send(const char *buf, long size) {
    if (size > this->BufferSize)
        throw ERROR("Transmitter: send ERROR, size > BufferSize");
    memcpy(this->shmaddr, buf, size);
    return size;
}

Transceiver::SHM_SYSV::Transmitter::~Transmitter() {
    shmdt(this->shmaddr);
    shm_unlink(this->getDest().c_str());
}
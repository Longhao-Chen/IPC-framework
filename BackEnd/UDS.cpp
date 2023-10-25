/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-24 18:58:31
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/BackEnd/UDS.cpp
 * @Description: UNIX 域套接字
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ERROR.hpp>
#include <UDS.hpp>
using namespace Transceiver::UDS;

/*
* 接收器的实现
*/
Server::Server(std::string name)
	: AbstractReceiver(name)
{
	this->sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (this->sfd == -1)
		throw ERROR(std::string("Create socket ERROR, name = ") + name);
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path + 1, name.c_str(),
		sizeof(struct sockaddr_un) - 1 - sizeof(AF_UNIX));
	if (bind(sfd, (const sockaddr *)&addr, sizeof(struct sockaddr_un)) ==
	    -1) {
		throw ERROR(std::string("Create bind ERROR, name = ") + name);
	}
	if (listen(sfd, 16) == -1) {
		this->~Server();
		throw ERROR(std::string("Create listen ERROR, name = ") + name);
	}
}

long Server::receive(char *buf, long size)
{
	if (this->cfd == -1) {
		this->cfd = accept(this->sfd, NULL, NULL);
		if (this->cfd == -1)
			throw ERROR("accept ERROR");
	}
	long num = read(this->cfd, buf, size);
	while (num < size) {
		num += read(this->cfd, buf + num, size - num);
	}
	return num;
}

Server::~Server()
{
	close(this->cfd);
	close(this->sfd);
}

/*
* 发送器的实现
*/
Client::Client(std::string dest)
	: AbstractTransmitter(dest)
{
	this->sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (this->sfd == -1)
		throw ERROR("Create socket ERROR");
	memset(&(this->addr), 0, sizeof(struct sockaddr_un));
	this->addr.sun_family = AF_UNIX;
	strncpy(this->addr.sun_path + 1, dest.c_str(),
		sizeof(struct sockaddr_un) - 1 - sizeof(AF_UNIX));
}

long Client::send(const char *buf, long size)
{
	long num = write(this->sfd, buf, size);
	if (num == -1) {
		if (connect(this->sfd, (struct sockaddr *)&(this->addr),
			    sizeof(struct sockaddr_un)) == -1)
			throw ERROR("connect ERROR");
		num = write(this->sfd, buf, size);
	}
	while (num < size) {
		num += write(this->sfd, buf + num, size - num);
	}
	return num;
}

Client::~Client()
{
	close(this->sfd);
}
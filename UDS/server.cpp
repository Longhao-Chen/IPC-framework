/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-19 19:58:42
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/UDS/server.cpp
 * @Description: 
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ERROR.hpp>
#include <UDS.hpp>
using namespace Transceiver::UDS;

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
	long num = read(this->cfd, buf, size);
	if(num == -1)
		this->cfd = accept(this->sfd, NULL, NULL);
	if (this->cfd == -1)
		throw ERROR("accept ERROR");
	num = read(this->cfd, buf, size);
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
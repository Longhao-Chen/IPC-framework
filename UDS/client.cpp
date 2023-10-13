/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-20 02:07:21
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/UDS/client.cpp
 * @Description: 
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <ERROR.hpp>
#include <UDS.hpp>
using namespace Transceiver::UDS;

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
    if(connect(this->sfd, (struct sockaddr*)&(this->addr), sizeof(struct sockaddr_un)) == -1)
        throw ERROR("connect ERROR");
    long num = write(this->sfd, buf, size);
    if(num == -1)
        throw ERROR("write ERROR: -1");
    if(num != size)
        throw ERROR("write ERROR: num!=size");
    return num;
}

Client::~Client()
{
    close(this->sfd);
}
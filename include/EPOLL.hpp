/*
 * @Author: null-qwerty <null-qwerty@outlook.com>
 * @Date: 2023-12-14 17:54:38
 * @LastEditors: null-qwerty <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/include/EPOLL.hpp
 * @Description: UNIX域套接字加入epoll的收发器实现
 * Copyright (c) 2023 by null-qwerty, All Rights Reserved. 
 */
#ifndef HEADER_EPOLL
#define HEADER_EPOLL

#include "Transceiver.hpp"
#include <sys/un.h>
#include <sys/epoll.h>

#define MAX_EVENTS 16

namespace Transceiver::EPOLL
{
class Server : virtual public AbstractReceiver {
    private:
	// 套接字fd
	int sfd = -1;
	// -1 表示未进行连接
	int cfd = -1;
	// epoll
	int epfd;
	struct epoll_event ev, events[MAX_EVENTS];

public:
	Server(std::string name);
	using AbstractReceiver::receive;
	long receive(char *buf, long size) override;
	~Server() override;
};

class Client : virtual public AbstractTransmitter {
    private:
	int sfd = -1;
	struct sockaddr_un addr;

    public:
	Client(std::string dest);
	using AbstractTransmitter::send;
	long send(const char *buf, long size) override;
	~Client() override;
};
};
#endif
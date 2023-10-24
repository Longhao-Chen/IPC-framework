/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-19 20:21:17
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/include/UDS.hpp
 * @Description: UNIX域套接字的收发器实现
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#ifndef HEADER_UDS
#define HEADER_UDS

#include "Transceiver.hpp"
#include <sys/un.h>

namespace Transceiver::UDS
{
class Server : virtual public AbstractReceiver {
    private:
	// 套接字fd
	int sfd = -1;
	// -1 表示未进行连接
	int cfd = -1;

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
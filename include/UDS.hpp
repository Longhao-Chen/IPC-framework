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
	int sfd;
	int cfd;

    public:
	Server(std::string name);
	using AbstractReceiver::receive;
	long receive(char *buf, long size) override;
	~Server();
};

class Client : virtual public AbstractTransmitter {
    private:
	int sfd;
	struct sockaddr_un addr;

    public:
	Client(std::string dest);
	using AbstractTransmitter::send;
	long send(const char *buf, long size) override;
	~Client();
};

class Transceiver : public AbstractTransceiver,
				public Server,
				public Client
{
    public:
	Transceiver(std::string name, std::string dest);
};
};
#endif
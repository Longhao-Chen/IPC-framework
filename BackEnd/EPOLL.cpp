/*
 * @Author: null-qwerty <null-qwerty@outlook.com>
 * @Date: 2023-12-14 05:04:22
 * @LastEditors: null-qwerty <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/BackEnd/EPOLL.cpp
 * @Description: UNIX 域套接字加入epoll实现多对一发送
 * Copyright (c) 2023 by null-qwerty, All Rights Reserved. 
 */
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <ERROR.hpp>
#include <EPOLL.hpp>
using namespace Transceiver::EPOLL;

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
	epfd = epoll_create(MAX_EVENTS);
	ev.events = EPOLLIN;
	ev.data.fd = this->sfd;
	// 将sfd加入epoll监听
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->sfd, &ev) == -1)
		throw ERROR("epoll_ctl: sfd");
}

long Server::receive(char *buf, long size)
{
	// 等待事件发生
	int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

	// 处理所发生的所有事件
	int num = 0;
	for (int i = 0; i < nfds; i++)
	{
		if(this->events[i].data.fd == this->sfd){
			this->cfd = accept(this->sfd, NULL, NULL);
			if (this->cfd == -1)
				throw ERROR("accept ERROR");
			ev.events = EPOLLIN;
			ev.data.fd = this->cfd;
			// 将cfd加入epoll监听
			if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->cfd, &ev) == -1)
				throw ERROR("epoll_ctl: cfd");
		}else{
            int n = recv(events[i].data.fd, buf + num, size, 0);
			num += n;
			if(n == -1)
				throw ERROR("recv ERROR");
			else if(n == 0){
				// 关闭连接
				if (epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1)
					throw ERROR("epoll_ctl: cfd");
				close(events[i].data.fd);
				std::cout << "Client exit" << std::endl;
			}
		}
	}

	return nfds;
}

Server::~Server()
{
	close(this->cfd);
	close(this->sfd);
	close(this->epfd);
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
	if (connect(this->sfd, (struct sockaddr *)&(this->addr),
			sizeof(struct sockaddr_un)) == -1)
		throw ERROR("connect ERROR");
}

long Client::send(const char *buf, long size)
{
	long num = ::send(this->sfd, buf, size, 0);
	return num;
}

Client::~Client()
{
	close(this->sfd);
}
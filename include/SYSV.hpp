/*
 * @Author: Haoyu Chen <null-qwerty@outlook.com>
 * @Date: 2023-12-28 00:30:12
 * @LastEditors: Haoyu Chen <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/include/SYSV.hpp
 * @Description: SYS V 共享内存后端
 * Copyright (c) 2023 by Haoyu Chen, All Rights Reserved. 
 */
#ifndef HEADER_SYSV
#define HEADER_SYSV
#include "Transceiver.hpp"
#include <unistd.h>
#include <sys/shm.h>

#define SYSV_SHM_MAXSIZE 100 * 1024 * 1024

namespace Transceiver::SYSV
{
class Receiver : virtual public AbstractReceiver {
    private:
	int shmid;
	void *shmaddr;
    long BufferSize;

public:
	Receiver(std::string name, const Config::Config &conf);
	using AbstractReceiver::receive;
	long receive(char *buf, long size) override;
	~Receiver() override;
};

class Transmitter : virtual public AbstractTransmitter {
    private:
	int shmid;
	void *shmaddr;
    long BufferSize;

    public:
	Transmitter(std::string dest, const Config::Config &conf);
	using AbstractTransmitter::send;
	long send(const char *buf, long size) override;
	~Transmitter() override;
};
};

#endif
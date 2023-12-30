/*
 * @Author: Haoyu Chen <null-qwerty@outlook.com>
 * @Date: 2023-12-30 01:30:12
 * @LastEditors: Haoyu Chen <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/include/SHM_SYSV.hpp
 * @Description: System V 共享内存后端
 * Copyright (c) 2023 by Haoyu Chen, All Rights Reserved. 
 */
#ifndef HEADER_SHM_SYSV
#define HEADER_SHM_SYSV
#include "Transceiver.hpp"
#include <unistd.h>
#include <sys/shm.h>

namespace Transceiver::SHM_SYSV
{
class Receiver : virtual public AbstractReceiver {
    private:
	key_t key;
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
	key_t key;
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
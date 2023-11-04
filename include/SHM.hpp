/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-27 13:53:47
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/include/SHM.hpp
 * @Description: 共享内存后端
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#ifndef HEADER_SHM
#define HEADER_SHM
#include "Transceiver.hpp"
#include <unistd.h>
#include <atomic>
#include <sys/shm.h>

namespace Transceiver::SHM
{
typedef struct {
	volatile std::atomic<long> p_read;
	volatile std::atomic<long> p_write;
} ringbuffer_matedata;

class Receiver : virtual public AbstractReceiver {
    private:
	int sfd;
	void *addr;
	ringbuffer_matedata *matedata;
	char *data;
	long bufersize;

    public:
	Receiver(std::string name, const Config::Config &conf);
	using AbstractReceiver::receive;
	long receive(char *buf, long size) override;
	~Receiver() override;
};

class Transmitter : virtual public AbstractTransmitter {
    private:
	int sfd;
	void *addr;
	ringbuffer_matedata *matedata;
	char *data;
	long bufersize;

	void init();
	bool inited = false;

    public:
	Transmitter(std::string dest, const Config::Config &conf);
	using AbstractTransmitter::send;
	long send(const char *buf, long size) override;
	~Transmitter() override;
};
};
#endif
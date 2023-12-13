/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-12-14 05:04:22
 * @LastEditors: null-qwerty <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/Transceiver/Transceiver.cpp
 * @Description: 通用收发器，可以通过 Config 选择不同的后端
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <Transceiver.hpp>
#include <UDS.hpp>
#include <SHM.hpp>
#include <EPOLL.hpp>
#include <Config.hpp>
#include <ERROR.hpp>

Transceiver::Receiver::Receiver(std::string name, Config::Config conf)
	: AbstractReceiver(name)
{
	if(conf["BackEnd"] == "UDS")
		this->BackEnd = new UDS::Server(name);
	else if(conf["BackEnd"] == "SHM")
		this->BackEnd = new SHM::Receiver(name, conf);
	else if(conf["BackEnd"] == "EPOLL")
		this->BackEnd = new EPOLL::Server(name);
	else
		throw ERROR("conf[\"BackEnd\"] 无法识别的参数:" + conf["BackEnd"]);
}

long Transceiver::Receiver::receive(char *buf, long size)
{
	return this->BackEnd->receive(buf, size);
}

Transceiver::Receiver::~Receiver()
{
	delete this->BackEnd;
}

Transceiver::Transmitter::Transmitter(std::string dest, Config::Config conf)
	: AbstractTransmitter(dest)
{
	if(conf["BackEnd"] == "UDS")
		this->BackEnd = new UDS::Client(dest);
	else if(conf["BackEnd"] == "SHM")
		this->BackEnd = new SHM::Transmitter(dest, conf);
	else if(conf["BackEnd"] == "EPOLL")
		this->BackEnd = new EPOLL::Client(dest);
	else
		throw ERROR("conf[\"BackEnd\"] 无法识别的参数:" + conf["BackEnd"]);
}

long Transceiver::Transmitter::send(const char *buf, long size)
{
	return this->BackEnd->send(buf, size);
}

Transceiver::Transmitter::~Transmitter()
{
    delete this->BackEnd;
}

Transceiver::Transceiver::Transceiver(std::string name, std::string dest,
				      Config::Config conf)
	: AbstractTransceiver(name, dest)
	, Receiver(name, conf)
	, Transmitter(dest, conf)
{
}
/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-19 21:00:00
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/Transceiver/AbstractTransceiver.cpp
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <Transceiver.hpp>
using namespace Transceiver;

AbstractReceiver::AbstractReceiver()
{
}

AbstractReceiver::AbstractReceiver(std::string name)
	: name(name)
{
}

long AbstractReceiver::receive(Message::AbstractMessage &msg)
{
	return this->receive(msg.setBuf(), msg.getSize());
}

std::string AbstractReceiver::getName() const
{
	return this->name;
}

AbstractTransmitter::AbstractTransmitter()
{
}

AbstractTransmitter::AbstractTransmitter(std::string dest)
	: dest(dest)
{
}

long AbstractTransmitter::send(const Message::AbstractMessage &msg)
{
	return this->send(msg.getBuf(), msg.getSize());
}

std::string AbstractTransmitter::getDest() const
{
	return this->dest;
}

Transceiver::AbstractTransceiver::AbstractTransceiver(std::string name,
						      std::string dest)
	: AbstractReceiver(name)
	, AbstractTransmitter(dest)
{
}
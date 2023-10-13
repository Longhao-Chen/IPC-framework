/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-20 20:53:55
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/UDS/transceiver.cpp
 * @Description: UDS 的收发器
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved.
 */
#include <UDS.hpp>
Transceiver::UDS::Transceiver::Transceiver(std::string name, std::string dest)
	: AbstractTransceiver(name, dest)
	, Server(name)
	, Client(dest)
{
}
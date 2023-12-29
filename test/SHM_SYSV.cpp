/*
 * @Author: Haoyu Chen <null-qwerty@outlook.com>
 * @Date: 2023-12-30 01:30:39
 * @LastEditors: Haoyu Chen <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/test/SHM_SYSV.cpp
 * @Description: 测试 SHM_SYSV 后端的功能
 * Copyright (c) 2023 by Haoyu Chen, All Rights Reserved. 
 */

#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <chrono>

#include <Transceiver.hpp>
#include <Config.hpp>
#include <Message.hpp>
#include <ERROR.hpp>

TEST(SHM_SYSV, SHM_SYSVReceiverTransmitter)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM_SYSV");
	int i = 10; // 发送次数
	long int size = 1280 * 1024 * 3 * sizeof(int);
    conf.set("BufferSize", std::to_string(size));
	pid_t pid = fork(); // 创建子进程
	if (pid < 0) { // 创建失败
		std::cout << "fork error" << std::endl;
	}
	try {
		if (pid == 0) { // 子进程，作为客户端
			Transceiver::Transmitter cli("test-server", conf);
			char *buf;

			buf = (char *)malloc(size); // 模拟发送相机的图像
			for (long i = 0; i < size; ++i)
				buf[i] = (char)i;
			while (i--) {
				cli.send(buf, size);
			}
		} else { // 父进程，作为服务器

			Transceiver::Receiver srv("test-server", conf);
			char *buf;
			buf = (char *)malloc(size); // 模拟接收相机的图像
			while (i--) {
				srv.receive(buf, size);
			}
			for (long i = 0; i < size; ++i)
				if (buf[i] != (char)i)
					FAIL();
		}
	} catch (ERROR e) {
		std::cout << e;
		FAIL();
	}
	if (pid == 0)
		exit(0);
	else
		wait(0);
}
/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-13 22:05:39
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/test/SHM.cpp
 * @Description: 测试 SHM 后端的功能
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
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

TEST(SHM, SHMReceiverTransmitter)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM");
	Message::Message msg(16);
	// 这里我们分成2个进程来测试通讯功能
	if (fork() == 0) {
		Transceiver::Transmitter c("Testaaa", conf);
		char *a = msg.returnMsgArea();
		for (int i = 0; i < 16; ++i) {
			a[i] = (char)i;
		}
		usleep(10000);
		c.send(msg);
		exit(0);
	} else {
		Transceiver::Receiver s("Testaaa", conf);
		s.receive(msg);
		bool testres = true;
		for (int i = 0; i < 16; ++i) {
			if (msg.returnMsgArea()[i] != (char)i)
				testres = false;
		}
		wait(0);
		EXPECT_TRUE(testres);
	}
}

TEST(SHM, SHMTransceiver)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM");
	int pid = fork();
	try {
		// 仍然是2个进程通讯
		if (pid == 0) {
			Transceiver::Transceiver t("Test0s", "Test1s", conf);
			Message::Message msg(16);
			t.receive(msg);
			t.send(msg);
			SUCCEED();
		} else {
			Transceiver::Transceiver t("Test1s", "Test0s", conf);
			Message::Message msg(16), msg1(16);
			for (int i = 0; i < 16; ++i) {
				msg.returnMsgArea()[i] = (char)i;
			}
			// 防止子进程的服务器未生成导致抛出错误
			usleep(10000);
			t.send(msg);
			t.receive(msg1);
			bool testres = true;
			for (int i = 0; i < 16; ++i) {
				if (msg1.returnMsgArea()[i] != (char)i)
					testres = false;
			}
			EXPECT_TRUE(testres);
		}
	} catch (ERROR e) {
		std::cout << "PID = " << pid << std::endl;
		std::cout << e;
		FAIL();
	}
	if (pid == 0)
		exit(0);
	else
		wait(0);
}

TEST(SHM, SHMTransceiverRepeat)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM");
	int i = 10; // 发送次数
	long int size = 1280 * 1024 * 3 * sizeof(int);
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
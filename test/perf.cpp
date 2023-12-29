/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-12-30 01:48:05
 * @LastEditors: Haoyu Chen <null-qwerty@outlook.com>
 * @FilePath: /IPC-framework/test/perf.cpp
 * @Description: 性能测试程序
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <Transceiver.hpp>
#include <gtest/gtest.h>
#include <chrono>
#include <ERROR.hpp>

// 发送数据的尺寸，配置，发送次数
bool testPref(long int size, Config::Config conf, long int N)
{
	char *buf = new char[size / sizeof(char)];
	long int count = N;
	pid_t pid = fork();
	if (pid < 0) {
		std::cout << "Fork failed" << std::endl;
		return false;
	}
	try {
		if (pid == 0) {
			Transceiver::Transmitter c("TestPerf", conf);
			while (count--) {
				c.send(buf, size);
			}
		} else {
			Transceiver::Receiver c("TestPerf", conf);
			auto start = std::chrono::steady_clock::now();
			while (count--) {
				c.receive(buf, size);
			}
			auto end = std::chrono::steady_clock::now();
			auto diff = end - start;
			std::cout << "Time: "
				  << std::chrono::duration<double, std::micro>(
					     diff)
						     .count() /
					     N
				  << " us" << std::endl;
		}
	} catch (ERROR e) {
		std::cout << e;
		return false;
	}
	if (pid == 0)
		exit(0);
	else {
		wait(0);
		return true;
	}
}

TEST(Pref, UDSBigData)
{
	Config::Config conf;
	conf.set("BackEnd", "UDS");
	long int size = 1280 * 1024 * 3 * sizeof(char);
	long int N = 1000;
	EXPECT_TRUE(testPref(size, conf, N));
}

TEST(Pref, UDSSmallData)
{
	Config::Config conf;
	conf.set("BackEnd", "UDS");
	long int size = 1;
	long int N = 500000;
	EXPECT_TRUE(testPref(size, conf, N));
}

TEST(Perf, SHMBigData)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM");
	long int size = 1280 * 1024 * 3 * sizeof(char);
	long int N = 5000;
	EXPECT_TRUE(testPref(size, conf, N));
}

TEST(Perf, SHMSmallData)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM");
	long int size = 1;
	long int N = 10000000;
	EXPECT_TRUE(testPref(size, conf, N));
}

TEST(Perf, SHM_SYSVBigData)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM_SYSV");
	long int size = 1280 * 1024 * 3 * sizeof(char);
	conf.set("BufferSize", std::to_string(size));
	long int N = 5000;
	EXPECT_TRUE(testPref(size, conf, N));
}

TEST(Perf, SYSV_SHMSmallData)
{
	Config::Config conf;
	conf.set("BackEnd", "SHM_SYSV");
	long int size = 1;
	conf.set("BufferSize", std::to_string(size));
	long int N = 10000000;
	EXPECT_TRUE(testPref(size, conf, N));
}
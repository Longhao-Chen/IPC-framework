/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-22 00:21:38
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/test/Message.cpp
 * @Description: 对 Message 目录下进行测试
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <gtest/gtest.h>
#include <sys/time.h>
#include <Message.hpp>

/**
 * @description: 抽象信息接口测试
 * @param {AbstractMessage} &msg 传入的信息对象
 * @param {long int} size 信息段大小
 * @return {bool} 成功返回 true 失败 false
 */
bool abstractMessageTest(Message::AbstractMessage &msg, long int size)
{
	if (msg.getSize() != size)
		return false;
	char *tmp;
	tmp = new char[size];
	try {
		for (int i = 0; i < size; ++i)
			tmp[i] = msg.getBuf()[i];
		for (int i = 0; i < size; ++i)
			msg.setBuf()[i] = tmp[i];
		delete[] tmp;
	} catch (...) {
		return false;
	}
	return true;
}

TEST(Message, MessageTimestamp)
{
	struct timeval tv, tv1;
	gettimeofday(&tv, 0);
	Message::Message a(10);
	gettimeofday(&tv1, 0);
	EXPECT_LE(tv.tv_sec * 1000000L + tv.tv_usec, a.getTimestamp());
	EXPECT_GE(tv1.tv_sec * 1000000L + tv1.tv_usec, a.getTimestamp());
	a.updateTimestamp();
	gettimeofday(&tv, 0);
	EXPECT_GE(tv.tv_sec * 1000000L + tv.tv_usec, a.getTimestamp());
	EXPECT_LE(tv1.tv_sec * 1000000L + tv1.tv_usec, a.getTimestamp());
}

TEST(Message, MessageAbstractMessageTest)
{
	Message::Message a(10);
	EXPECT_TRUE(abstractMessageTest(a, 10 + sizeof(long)));
	Message::Message *b;
	b = new Message::Message(0);
	EXPECT_TRUE(abstractMessageTest(*b, 0 + sizeof(long)));
	delete b;
	Message::Message *c;
	c = new Message::Message(10);
	EXPECT_TRUE(abstractMessageTest(*c, 10 + sizeof(long)));
	delete c;
}
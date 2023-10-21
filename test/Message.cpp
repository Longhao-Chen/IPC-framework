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
#include <ERROR.hpp>

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

TEST(Message, Message)
{
	Message::Message a(10);
	for (int i = 0; i < 10; ++i) {
		a.returnMsgArea()[i] = i;
	}
	try {
		a.checkCanary();
	} catch (ERROR e) {
		std::cout << e;
		FAIL();
	}
	bool issuccess = true;
	for (int i = 0; i < 10; ++i) {
		if (a.returnMsgArea()[i] != i)
			issuccess = false;
	}
	EXPECT_TRUE(issuccess);

	char tmp[100];
	EXPECT_EQ(a.copy(tmp), 10);
	EXPECT_EQ(a.copy(tmp, 100), 10);
	EXPECT_EQ(a.copy(tmp, 1000, 1), 9);
	try{
		a.checkCanary();
		SUCCEED();
	} catch (ERROR e){
		std::cout << e;
		FAIL();
	}
	for(int i=0; i<100; ++i)
		tmp[i] = i;
	a.copy(tmp);
	for(int i=0; i<10; ++i){
		if(a.returnMsgArea()[i] != i)
			FAIL();
	}
}

// 死亡测试
TEST(Message, MessageDie)
{
	Message::Message a(10);
	for (int i = 0; i < 11; ++i) {
		a.returnMsgArea()[i] = i;
	}
	try {
		a.checkCanary();
		FAIL();
	} catch (ERROR e) {
		SUCCEED();
	}
}

TEST(Message, pMessage)
{
	int mi[10];
	double md[10];
	char mc[10];
	Message::pMessage Mi(mi, 10);
	Message::pMessage Md(md, 10);
	Message::pMessage Mc(mc, 10);
	bool intsuccess = true;
	bool doublesuccess = true;
	bool charsuccess = true;
	for (int i = 0; i < 10; ++i) {
		mi[i] = i;
		md[i] = i;
		mc[i] = i;
		if (Mi.getMessage()[i] != i)
			intsuccess = false;
		if (Md.getMessage()[i] != i)
			doublesuccess = false;
		if (Mc.getMessage()[i] != i)
			charsuccess = false;
	}
	EXPECT_TRUE(intsuccess);
	EXPECT_TRUE(doublesuccess);
	EXPECT_TRUE(charsuccess);

	EXPECT_TRUE(abstractMessageTest(Mi, sizeof(int)*10));
	EXPECT_TRUE(abstractMessageTest(Md, sizeof(double)*10));
	EXPECT_TRUE(abstractMessageTest(Mc, 10));
}
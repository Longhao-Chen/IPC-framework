/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-11-01 11:33:15
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/test/StringToNum.cpp
 * @Description: 字符串转数字测试
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <utils/StringToNum.hpp>
#include <string>
#include <gtest/gtest.h>

TEST(StringToNum, StringToLong)
{
	std::string str = "123456789";
	long ret = utils::StringToLong(str);
	EXPECT_EQ(ret, 123456789);
	str = "-123456789";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, -123456789);
	str = "0";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, 0);
	str = "-0";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, 0);
	str = "123456789123456789";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, 123456789123456789);
	str = "-123456789123456789";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, -123456789123456789);
	str = "00001";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, 1);
	str = "-00001";
	ret = utils::StringToLong(str);
	EXPECT_EQ(ret, -1);
}
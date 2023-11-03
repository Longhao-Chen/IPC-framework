/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-11-01 11:25:02
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/utils/StringToNum.cpp
 * @Description: 字符串转数字
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <utils/StringToNum.hpp>
#include <ERROR.hpp>

long utils::StringToLong(const std::string &str)
{
	long ret = 0;
	bool neg = false;
	for (auto c : str) {
		if ((c < '0' || c > '9') && c != '-')
			throw ERROR("StringToLong ERROR");
		if (c == '-') {
			neg = !neg;
			continue;
		}
		ret = ret * 10 + c - '0';
	}
	return neg ? -ret : ret;
}
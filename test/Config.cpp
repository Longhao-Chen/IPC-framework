/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-24 13:22:50
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/test/Config.cpp
 * @Description: 测试 Config::Config 类
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <gtest/gtest.h>
#include <fstream>

#include <Config.hpp>
#include <ERROR.hpp>

TEST(Config, save)
{
	try {
		Config::Config conf;
		conf.set("BackEnd", "UDS");
		std::ofstream fs("/tmp/Config_test_fdasfasf");
		if (fs.is_open()) {
			conf.save(fs);
			fs.close();
			SUCCEED();
		} else
			FAIL();
	} catch (ERROR e) {
		std::cout << e << std::endl;
		FAIL();
	}
}

TEST(Config, load)
{
	try {
		std::ifstream fs("/tmp/Config_test_fdasfasf");
		if (!fs.is_open())
			FAIL();
		else {
			Config::Config conf(fs);
			fs.close();
			SUCCEED();
		}

		fs.open("/tmp/Config_test_fdasfasf");
		if (!fs.is_open())
			FAIL();
		else {
			Config::Config conf1;
			fs >> conf1;
			fs.close();
			SUCCEED();
		}
	} catch (ERROR e) {
		std::cout << e << std::endl;
		FAIL();
	}
}

TEST(Config, read)
{
	try {
		std::ifstream fs("/tmp/Config_test_fdasfasf");
		if (!fs.is_open())
			FAIL();
		else {
			Config::Config conf(fs);
			fs.close();
			if (conf["BackEnd"] == "UDS")
				SUCCEED();
			else
				FAIL();
		}
	} catch (ERROR e) {
		std::cout << e << std::endl;
		FAIL();
	}
}
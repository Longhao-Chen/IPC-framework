/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-21 01:07:43
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/include/Config.hpp
 * @Description: 配置对象
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#ifndef HEADER_CONFIG
#define HEADER_CONFIG
#include <fstream>
#include <vector>
namespace Config
{
const std::vector<std::string> ConfigOption = { "BackEnd", "OnlyUser",
						"BufferSize" };

class Config {
    private:
	std::string BackEnd;
	// 如果为True则只能在同一个用户下通信
	// 为False则可以在系统全局范围内通信
	// 对USD后端无效
	std::string OnlyUser;
	// 缓冲区大小，单位为字节
	std::string BufferSize;

    public:
	// 生成默认配置
	Config();
	// 从文件流中生成
	Config(std::istream &);
	// 保存到文件
	void save(std::ostream &) const;
	// 从文件或标准输入读取配置
	friend std::istream &operator>>(std::istream &, Config &);
	// 输出配置或保存到文件
	friend std::ostream &operator<<(std::ostream &, const Config &);

	// 获取属性
	// 此函数只能用于获得属性，这样做的目的是防止配置被意外的更改
	std::string operator[](const std::string co) const;

	// 设置属性
	void set(const std::string co, const std::string value);
};
std::istream &operator>>(std::istream &, Config &);
std::ostream &operator<<(std::ostream &, const Config &);
}
#endif
/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-10-21 01:26:06
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/utils/Config.cpp
 * @Description: 配置对象
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <Config.hpp>
#include <ERROR.hpp>
#include <string>

Config::Config::Config()
{
	// 默认配置
	this->BackEnd = "UDS";
	this->OnlyUser = "True";
	// 512KiB
	this->BufferSize = "524288";
}

Config::Config::Config(std::istream &ins)
	: Config()
{
	ins >> *this;
}

void Config::Config::save(std::ostream &outs) const
{
	outs << *this;
}

std::string Config::Config::operator[](const std::string co) const
{
	if (co == "BackEnd")
		return this->BackEnd;
	else if (co == "OnlyUser")
		return this->OnlyUser;
	else if (co == "BufferSize")
		return this->BufferSize;
	else
		throw ERROR("参数错误");
}

void Config::Config::set(const std::string type, const std::string value)
{
	if (type == "BackEnd")
		this->BackEnd = value;
	else if (type == "OnlyUser")
		this->OnlyUser = value;
	else if (type == "BufferSize")
		this->BufferSize = value;
	else
		throw ERROR("参数错误");
}

std::istream &Config::operator>>(std::istream &in, Config &conf)
{
	std::string type, val;
	while (in >> type) {
		in >> val;
		conf.set(type, val);
	}
	return in;
}

std::ostream &Config::operator<<(std::ostream &out, const Config &conf)
{
	for (auto it = ConfigOption.begin(); it != ConfigOption.end(); ++it) {
		out << *it << " ";
		out << conf[*it] << std::endl;
	}
	return out;
}
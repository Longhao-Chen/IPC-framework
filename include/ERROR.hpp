/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-19 20:00:58
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/include/ERROR.hpp
 * @Description: 错误处理类，用于打印错误信息
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#ifndef HEADER_ERROR
#define HEADER_ERROR
#include <string>
#include <iostream>
#include <vector>

class ERROR
{
private:
    void backtrace();
    std::vector<std::string> bt; //调用栈
    std::string msg;             // 错误信息
public:
    ERROR(std::string);
    ERROR(const char*);
    friend std::ostream &operator<<(std::ostream &out, const ERROR &t);
};
std::ostream &operator<<(std::ostream &out, const ERROR &t);

#endif
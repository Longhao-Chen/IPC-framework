/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-19 20:00:42
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/utils/ERROR.cpp
 * @Description: 错误处理
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
// 错误处理模块
#include "ERROR.hpp"

#ifndef HEADER_LIBUNWIND
#define HEADER_LIBUNWIND
#include <libunwind.h>
#endif

ERROR::ERROR(std::string msg)
{
	this->msg = msg;
	this->backtrace();
}

ERROR::ERROR(const char *msg)
{
    std::string m(msg);
    this->msg = m;
    this->backtrace();
}

// 获取函数调用栈
void ERROR::backtrace()
{
	unw_cursor_t cursor;
	unw_context_t context;

	unw_getcontext(&context);
	unw_init_local(&cursor, &context);

	while (unw_step(&cursor) > 0) {
		unw_word_t offset, pc;
		unw_get_reg(&cursor, UNW_REG_IP, &pc);
		if (pc == 0)
			break;

		char sym[256];
		if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) ==
		    0) {
			this->bt.push_back("0x" + std::to_string(pc) + ": (" +
					   std::string(sym) + "0x" +
					   std::to_string(offset) + ")");
		} else {
			this->bt.push_back(
				"0x" + std::to_string(pc) +
				": -- error: unable to obtain symbol name for this frame");
		}
	}
}

std::ostream &operator<<(std::ostream &out, const ERROR &t)
{
	out << "----------" << std::endl;
	out << "IPC-ERROR: " << t.msg << std::endl << std::endl;
	out << "Backtrace: " << std::endl;
	for (auto it = t.bt.begin(); it < t.bt.end(); ++it) {
		out << *it << std::endl;
	}
	return out;
}
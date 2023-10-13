#
# @Author: Longhao.Chen <Longhao.Chen@outlook.com>
# @Date: 2023-09-19 20:18:47
# @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
# @FilePath: /IPC-framework/Makefile
# Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
#
ROOT_DIR = $(PWD)
CXX = g++
MAKE = make
CXXFLAGS = -I $(ROOT_DIR)/include/ -fPIC -std=c++20 -lunwind -lunwind-x86_64 -O3 -Wall

TARGET = ipc_framework
# 目录名不要加 /
SUB_DIR = Message Transceiver UDS utils
SUB_DIR_TAGET = $(addsuffix .out, $(join $(addsuffix /, $(SUB_DIR)), $(SUB_DIR)))

export CXX
export MAKE
export CXXFLAGS
export TARGET

.PHONY: $(TARGET) $(SUB_DIR) test clean clean-all

$(TARGET): lib$(TARGET).so

lib$(TARGET).so: $(SUB_DIR_TAGET)
	$(CXX) -shared $^ -o $@

$(SUB_DIR_TAGET): $(SUB_DIR)

$(SUB_DIR):
	$(MAKE) -C $@

test: $(TARGET)
	$(MAKE) -C test

clean:
	- find -name *.o | xargs -I {} rm {}
	- find -name *.out | xargs -I {} rm {}
	- find -name *.d | xargs -I {} rm {}

clean-all: clean
	-rm lib$(TARGET).so
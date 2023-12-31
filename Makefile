#
# @Author: Longhao.Chen <Longhao.Chen@outlook.com>
# @Date: 2023-09-19 20:18:47
# @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
# @FilePath: /IPC-framework/Makefile
# Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
#
ROOT_DIR = $(shell pwd)
CXX = g++
MAKE = make
CXXFLAGS = -flto=auto -g -I $(ROOT_DIR)/include/ -fPIC -std=c++20 -lunwind -lunwind-x86_64 -O3 -march=native -Wall -Wextra
DESTDIR = /
TARGET = ipc_framework
# 目录名不要加 /
SUB_DIR = Message Transceiver BackEnd utils
SUB_DIR_TAGET = $(addsuffix .out, $(join $(addsuffix /, $(SUB_DIR)), $(SUB_DIR)))

export CXX
export MAKE
export CXXFLAGS
export TARGET

.PHONY: $(TARGET) $(SUB_DIR) test clean clean-all install uninstall

$(TARGET): lib$(TARGET).so

lib$(TARGET).so: $(SUB_DIR_TAGET)
	$(CXX) -shared $(CXXFLAGS) $^ -o $@

$(SUB_DIR_TAGET): $(SUB_DIR)

$(SUB_DIR):
	$(MAKE) -C $@

test: $(TARGET)
	$(MAKE) -C test

install: $(TARGET) uninstall
	mkdir -p $(DESTDIR)/usr/lib/x86_64-linux-gnu/
	mkdir -p $(DESTDIR)/usr/include/IPC-framework/
	cp libipc_framework.so $(DESTDIR)/usr/lib/x86_64-linux-gnu/
	cp include/*.hpp $(DESTDIR)/usr/include/IPC-framework/

uninstall:
	- rm -r $(DESTDIR)/usr/include/IPC-framework
	- rm $(DESTDIR)/usr/lib/x86_64-linux-gnu/libipc_framework.so

clean:
	- find -name *.o | xargs -I {} rm {}
	- find -name *.out | xargs -I {} rm {}
	- find -name *.d | xargs -I {} rm {}

clean-all: clean
	- rm lib$(TARGET).so
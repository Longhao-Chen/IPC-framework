#
# @Author: Longhao.Chen <Longhao.Chen@outlook.com>
# @Date: 2023-09-21 23:22:09
# @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
# @FilePath: /IPC-framework/test/Makefile
# Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
# 
CXXFLAGS = -O3 -I../include -L.. -lgtest -lgtest_main -l$(TARGET) -pthread -lunwind -lunwind-x86_64

.PHONY: test test-shared

test: test-shared $(patsubst %.cpp,%.d,$(wildcard *.cpp))

include $(patsubst %.cpp,%.d,$(wildcard *.cpp))

test-shared: test-shared.out
	./test-shared.out

test-shared.out: $(patsubst %.cpp,%.o,$(wildcard *.cpp)) ../lib$(TARGET).so
	$(CXX) $^ $(CXXFLAGS) -o $@

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< | \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@; \
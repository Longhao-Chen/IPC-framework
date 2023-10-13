#
# @Author: Longhao.Chen <Longhao.Chen@outlook.com>
# @Date: 2023-09-22 00:34:47
# @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
# @FilePath: /IPC-framework/Message/Makefile
# Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
#
DIRNAME = Message

.PHONY: all

all: $(patsubst %.cpp,%.d,$(wildcard *.cpp)) $(DIRNAME).out

$(DIRNAME).out: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	ld -relocatable $^ -o $@

include $(patsubst %.cpp,%.d,$(wildcard *.cpp))

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< | \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@; \
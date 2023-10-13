/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-21 19:36:38
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/Message/Message.cpp
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <Message.hpp>
#include <sys/time.h>
#include <ERROR.hpp>

Message::Message::Message(long sizeIn)
{
    this->msg = new char[sizeIn + sizeof(long) + 1];
    // 设置金丝雀值
    this->msg[sizeIn + sizeof(long)] = (char)85;
    this->timestamp = (long*)this->msg;
    this->msgArea = this->msg + sizeof(long);
    this->size = sizeIn + sizeof(long);
    this->updateTimestamp();
}

long Message::Message::updateTimestamp()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    *(this->timestamp) = tv.tv_sec * 1000000L + tv.tv_usec;
    return *(this->timestamp);
}

long Message::Message::getTimestamp() const
{
    return *(this->timestamp);
}

char *Message::Message::returnMsgArea()
{
    return this->msgArea;
}

void Message::Message::checkCanary()
{
    if(this->msg[this->size] != (char)85)
        throw ERROR("checkCanary Fail!");
}

Message::Message::~Message()
{
    delete this->msg;
}
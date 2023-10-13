/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-21 19:27:32
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/Message/AbstractMessage.cpp
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#include <Message.hpp>

const char *Message::AbstractMessage::getBuf() const
{
    return this->msg;
}

char *Message::AbstractMessage::setBuf()
{
    return this->msg;
}

long Message::AbstractMessage::getSize() const
{
	return this->size;
}
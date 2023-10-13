/*
 * @Author: Longhao.Chen <Longhao.Chen@outlook.com>
 * @Date: 2023-09-21 19:07:14
 * @LastEditors: Longhao.Chen <Longhao.Chen@outlook.com>
 * @FilePath: /IPC-framework/include/Message.hpp
 * @Description: 信息抽象类及信息类
 * Copyright (c) 2023 by Longhao.Chen, All Rights Reserved. 
 */
#ifndef HEADER_MESSAGE
#define HEADER_MESSAGE
namespace Message
{

class AbstractMessage {
    protected:
    // 信息段大小
	long size;
    // 指向信息段的指针
	char *msg;
    public:
	// 返回信息段的指针
	virtual const char *getBuf() const;
    // 返回信息段的可写指针
    virtual char *setBuf();
    // 返回信息段大小
    virtual long getSize() const;
};

/*
信息的构成：
时间戳 + 正文 + 1字节的金丝雀值
信息段的前64位为时间戳，时间戳精确到us
*/
class Message: public AbstractMessage {
    private:
    // 时间戳
    long *timestamp;
    // 指向信息保存区域，长度大小为size-sizeof(long)
    char *msgArea;
    public:
    // 传入信息大小（不包括时间戳）
    // 信息段大小为 sizeIn + sizeof(long)
	Message(long sizeIn);
    // 更新时间戳，返回更新后的时间戳
    long updateTimestamp();
    // 返回时间戳
    long getTimestamp() const;
    // 返回信息区域的指针，用于写入数据，不安全
    // 如果对写入越界，不会进行检查
    // 建议写入之后运行
    char *returnMsgArea();
    // 检查金丝雀值，如果不通过会抛出错误
    void checkCanary();
	~Message();
};
};
#endif
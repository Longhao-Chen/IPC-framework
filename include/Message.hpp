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
	virtual ~AbstractMessage();
};

/*
信息的构成：
时间戳 + 正文 + 1字节的金丝雀值
信息段的前64位为时间戳，时间戳精确到us
*/
class Message : public AbstractMessage {
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

	// 从用户指定的指针拷贝数据到数据区
	// 均返回拷贝的大小，单位字节
	// 拷贝的大小为信息的尺寸
	long copy(const char *p);
	// 指定拷贝的尺寸
	// (指针， 要拷贝的数据大小， 拷贝到信息内部缓冲区的开始位置)
	// 如果拷贝的尺寸大于信息的尺寸，则只拷贝信息的尺寸
	long copy(const char *p, const long sizeCp, const long start = 0);

	// 检查金丝雀值，如果不通过会抛出错误
	void checkCanary();
	~Message();
};

/*
从指定指针生成的信息
*/
template <class T> class pMessage : public AbstractMessage {
    public:
	// 指定指针, 指针指向区域的大小(单位为多少个指针数据类型)
	// 例如 long int[10] 的大小为 10
	pMessage(T *p, const long sizeIn);
	// 获得信息的只读指针
	T const *getMessage() const;
	// 获得信息的可写指针
	T *setMessage();
	// 获得指针区域大小(单位为多少个指针数据类型)
	long getPMSize() const;
};

template <class T> pMessage<T>::pMessage(T *p, const long sizeIn)
{
	this->size = sizeIn * sizeof(decltype(*p));
	this->msg = static_cast<char *>(static_cast<void *>(p));
}

template <class T> T const *pMessage<T>::getMessage() const
{
	return static_cast<T *>(static_cast<void *>(this->msg));
}

template <class T> T *pMessage<T>::setMessage()
{
	return static_cast<T *>(static_cast<void *>(this->msg));
}

template <class T> long pMessage<T>::getPMSize() const
{
	return this->size / sizeof(T);
}

};
#endif
# 接口文档
## 收发器部分
### Transceiver::AbstractReceiver
接收信息，接收信息是可靠的。
```cpp
// 传入接收器名
AbstractReceiver();
AbstractReceiver(std::string name);

// 调用此函数开始接收，未接收到时会阻塞，返回接收到的字节数
// 传入缓冲区指针和缓冲区字节数
virtual long receive(char *buf, long size) = 0;

// 将接受到的信息放入一个信息类
virtual long receive(Message::AbstractMessage &);

// 返回接收器名
std::string getName() const;
```

### Transceiver::AbstractTransmitter
发送信息，发送信息是可靠的。
```cpp
// 传入目标接收器的名
AbstractTransmitter();
AbstractTransmitter(std::string dest);

// 调用此函数开始发送，会阻塞，返回发送的字节数
// 传入缓冲区和总字节数
virtual long send(const char *buf, long size) = 0;
virtual long send(const Message::AbstractMessage &);

// 返回目标接收器名
std::string getDest() const;
```

### Transceiver::AbstractTransceiver
此类虚公有继承`Transceiver::AbstractReceiver`和`Transceiver::AbstractTransmitter`

可以双向收发信息

## 信息部分
### Message::AbstractMessage
对信息的一个接口
```cpp
// 返回信息段的指针
virtual const char *getBuf() const;
// 返回信息段的可写指针
virtual char *setBuf();
// 返回信息段大小
virtual long getSize() const;
```
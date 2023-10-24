# 通用收发器
## Transceiver::Receiver
### 接口
虚继承 `Transceiver::AbstractReceiver`
### 构造
传入一个`std::string`作为监听的地址，格式任意，再传入 `Config::Config` 对象

## Transceiver::Transmitter
### 接口
虚继承 `Transceiver::AbstractTransmitter`
### 构造
传入一个`std::string`作为目标的地址，格式任意，再传入 `Config::Config` 对象

**注意：在调用`send`之前，必须保证目标地址存在**

## Transceiver::Transceiver
### 接口
继承`Transceiver::AbstractTransceiver`

### 构造
传入`(std::string name, std::string dest)`，`name` 为监听的地址，`dest` 为目标
，再传入 `Config::Config` 对象

## 例子
见各个后端的文档
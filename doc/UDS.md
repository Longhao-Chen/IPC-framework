# UNIX 域套接字部分
**不推荐直接使用这些接口，建议使用 通用收发器**
## Transceiver::UDS::Server
### 接口
虚继承 `Transceiver::AbstractReceiver`
### 构造
传入一个`std::string`作为监听的地址，格式任意

## Transceiver::UDS::Client
### 接口
虚继承 `Transceiver::AbstractTransmitter`
### 构造
传入一个`std::string`作为目标的地址，格式任意

**注意：在调用`send`之前，必须保证目标地址存在**

## 一个在通用收发器中使用 `UDS` 的简短示例
```cpp
#include <string>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#include <IPC-framework/Transceiver.hpp>
#include <IPC-framework/Message.hpp>
#include <IPC-framework/ERROR.hpp>
#include <IPC-framework/Config.hpp>

int main()
{
	int pid = fork();
	Config::Config conf;
	conf.set("BackEnd", "UDS");
	try {
		// 仍然是2个进程通讯
		if (pid == 0) {
			Transceiver::Transceiver *t =
				new Transceiver::Transceiver("uTest0cs",
							     "uTest1cs", conf);
			Message::Message msg(16);
			t->receive(msg);
			t->send(msg);
			delete t;
		} else {
			Transceiver::Transceiver *t =
				new Transceiver::Transceiver("uTest1cs",
							     "uTest0cs", conf);
			Message::Message msg(16), msg1(16);
			for (int i = 0; i < 16; ++i) {
				msg.returnMsgArea()[i] = (char)i;
			}
			// 防止子进程的服务器未生成导致抛出错误
			sleep(1);
			t->send(msg);
			t->receive(msg1);
			bool testres = true;
			for (int i = 0; i < 16; ++i) {
				if (msg1.returnMsgArea()[i] != (char)i)
					testres = false;
			}
			delete t;
			if (testres) {
				std::cout << "通信成功" << std::endl;
			}
		}
	} catch (ERROR e) {
		std::cout << "PID = " << pid << std::endl;
		std::cout << e;
	}
}
```

编译命令：
```
g++ test.cpp -lipc_framework -lunwind -lunwind-x86_64 -O3
```
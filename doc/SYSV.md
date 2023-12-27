# system V 共享内存后端

## 介绍

使用 `system V` 共享内存作为后端的共享内存实现，支持多发送端和多接收端。  
`system V` 本身提供可靠性保证。  
发送的数据包长度不能超过设置的共享内存大小，默认为 512KiB。

**warning**
1. 收发是**非阻塞**的；
2. 多发送端获取共享内存写入权的方式为**随机争用**；
3. 无数据继续发送时，接收器将**获取到最后一次发送的数据**。

## 使用

通过该后端实现视频传输的例子：

### Receiver
```cpp
// IPC-framework
#include <IPC-framework/Config.hpp>
#include <IPC-framework/Transceiver.hpp>
#include <IPC-framework/SHM.hpp>
#include <IPC-framework/ERROR.hpp>
// STD
#include <iostream>
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define DATALENTH 1024 * 1280 * 3

int main()
{
	Config::Config cfg;
	cfg.set("BackEnd", "SYSV");
	cfg.set("BufferSize", std::to_string(DATALENTH));   // 设置共享内存大小

	Transceiver::Receiver srv("foo", cfg);
	Transceiver::Receiver Presrv("Pre", cfg);

	char PreData[4 * sizeof(int)];
    // 先行接收图像参数，用于开辟空间
	Presrv.receive(PreData, 4 * sizeof(int));
	int width = *(int *)PreData;
	int height = *(int *)(PreData + sizeof(int));
	int type = *(int *)(PreData + 2 * sizeof(int));
	int channels = *(int *)(PreData + 3 * sizeof(int));

	cv::Mat img(height, width, type);

	while (1) {
		srv.receive((char *)(img.data), width * height * channels);
		cv::imshow("img", img);
		cv::waitKey(1);
	}
}
```
编译命令：
```
g++ Receiver.cpp -lipc_framework -lunwind -lunwind-x86_64 -O3
```

### Transmitter

```cpp
// IPC-framework
#include <IPC-framework/Config.hpp>
#include <IPC-framework/Transceiver.hpp>
#include <IPC-framework/SHM.hpp>
// STD
#include <iostream>
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define DATALENTH 1024 * 1280 * 3

int main()
{
	Config::Config cfg;
	cfg.set("BackEnd", "SYSV");
	cfg.set("BufferSize", std::to_string(DATALENTH));   // 设置共享内存大小

	Transceiver::Transmitter cli("foo", cfg);
	Transceiver::Transmitter Precli("Pre", cfg);

	cv::VideoCapture cap("path/to/video");
	cv::Mat frame;
    cap >> frame;   // 先读取一帧，用于获取图像参数

	char PreData[4 * sizeof(int)];

	((int*)PreData)[0] = frame.cols;
	((int*)PreData)[1] = frame.rows;
	((int*)PreData)[2] = frame.type();
	((int*)PreData)[3] = frame.channels();
    // 发送图像参数，用于接收者开辟空间
    Precli.send(PreData, 4 * sizeof(int));

	while (1) {
		cli.send((char*)(frame.data), frame.cols * frame.rows * frame.channels());
        cap >> frame;
		cv::waitKey(30);
	}
}
```
编译命令：
```
g++ Transmitter.cpp -lipc_framework -lunwind -lunwind-x86_64 -O3
```


**注意：使用本例时需要先运行 Transmitter 后运行 Receiver**
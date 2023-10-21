# 信息部分的文档
## Message::Message
标准信息，继承于`Message::AbstractMessage`

```cpp
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
```

### 一个例子
```cpp
Message::Message a(10);
for (int i = 0; i < 10; ++i) {
    a.returnMsgArea()[i] = i;
}
try {
    a.checkCanary();
} catch (ERROR e) {
    std::cout << e;
}
bool issuccess = true;
for (int i = 0; i < 10; ++i) {
    if (a.returnMsgArea()[i] != i)
        issuccess = false;
}
```

## Message::pMessage
从用户指定的指针生成信息
继承于`Message::AbstractMessage`
```cpp
// 指定指针, 指针指向区域的大小(单位为多少个指针数据类型)
// 例如 long int[10] 的大小为 10
pMessage(T *p, const long sizeIn);
// 获得信息的只读指针
const T *getMessage() const;
// 获得信息的可写指针
T *setMessage();
// 获得指针区域大小(单位为多少个指针数据类型)
const long getPMSize() const;
```

### 一个例子
```cpp
int mi[10];
double md[10];
char mc[10];
Message::pMessage Mi(mi, 10);
Message::pMessage Md(md, 10);
Message::pMessage Mc(mc, 10);
bool intsuccess = true;
bool doublesuccess = true;
bool charsuccess = true;
for (int i = 0; i < 10; ++i) {
    mi[i] = i;
    md[i] = i;
    mc[i] = i;
    if (Mi.getMessage()[i] != i)
        intsuccess = false;
    if (Md.getMessage()[i] != i)
        doublesuccess = false;
    if (Mc.getMessage()[i] != i)
        charsuccess = false;
}
```
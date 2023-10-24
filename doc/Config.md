# Config::Config
## 构造
```cpp
// 生成默认配置
Config();
// 从文件流中生成
Config(std::istream &);
```

## 成员
```cpp
// 保存到文件
void save(std::ostream &) const;
// 从文件或标准输入读取配置
friend std::istream &operator>>(std::istream &, Config &);
// 输出配置或保存到文件
friend std::ostream &operator<<(std::ostream &, const Config &);

// 获取属性
// 此函数只能用于获得属性，这样做的目的是防止配置被意外的更改
std::string operator[](const std::string co) const;

// 设置属性
void set(const std::string co, const std::string value);
```

## 友元
```cpp
std::istream &operator>>(std::istream &, Config &);
std::ostream &operator<<(std::ostream &, const Config &);
```
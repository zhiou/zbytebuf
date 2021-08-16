# zbytebuf
一个纯头文件的轻量易用的函数式字节处理类。

[![Build Status](https://www.travis-ci.org/zhiou/zbytebuf.svg?branch=master)](https://www.travis-ci.org/zhiou/zbytebuf)![GitHub](https://img.shields.io/github/license/zhiou/zbytebuf)![c++](https://img.shields.io/badge/c%2B%2B-%3E%3D%2011-green)

## Why zbytebuf?

目前在一家商用密码公司工作，常常要与硬件打交道，封装和解析APDU指令。

纯C或C++处理在拼接各种指令时往往是命令式的，对于byte流的拼接和更改也不怎么友好。

所以这个zbytebuf对vector提供了简单的接口封装，以专门适应apdu拼接或类似工作。

它可以把apdu的组装工作改成类似下面这样。

```c++
zbytebuf apdu("FFAA");
apdu.reserve(3)  // 保留3字节，用于填充负载长度
    .append<std::string>("112233")
    .append<uint16_t>(0x6A48);
    .replace<uint16_t>(2, 3, "000005");
    .debug();          // "FFAA0000051122336A48"
		
```

还可以把apdu的解析工作变为：

```c++
zbytebuf resp("FFAA081122334455667788");
uint8_t len;
size_t offset;
uint16_t a, b, c, d;
resp.drop_front(2)
    .scan(len, offset)  // len = 0x08
    .scan(a, offset)     // a = 0x1122
    .scan(b, offset)     // b = 0x3344
    .scan(c, offset)     // c = 0x5566
    .scan(d, offset);    // d = 0x7788
```

## License

zbytebuf is available under the [MIT license](https://www.mit-license.org)
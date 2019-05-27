# Jt808

中华人民共和国交通运输行业标准JT/T808协议的简单测试后台;


## 快速使用

以下操作命令基于 Ubuntu 16.04 系统


克隆源码及编译:
```bash
$ git clone https://github.com/hanoi404/jt808 && cd jt808/jt808
$ make all
```

运行后台服务:
```bash
$ ./jt808service
```

然后需要将支持808协议的终端连接到这个后台, IP为你自己服务器的IP, 端口为`8193`, 可以在`main.cc`文件中自己修改.
后台可识别的终端识别号为`13826539847`等, 如果有需要在源码目录下`devices/devices.list`文件里自行添加.
终端连接到本后台后, 如果终端实现了上报位置信息命令，后台会进行解析.


使用控制命令操作后台, 下发命令到终端. 下面是查询终端参数操作例子, 假设终端识别号为`13826539850`, 要查询的终端参数ID为`0x0020`(位置汇报策略), 输入以下命令并得到返回结果:
```bash
$ ./jt808command 13826539850 getterminalparameter 0020
terminal parameter(id:value): 0020:0
```


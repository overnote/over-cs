## 一 字节序

### 1.1 字节序概念

字节序是指多字节的数据的存储顺序，有两种存储方式（LSB 低地址，MSB 高地址）：

- 小端格式：将低位字节数据存储在低地址
- 大端格式：将高位字节数据存储在低地址

解析示例：内存中每一块最小区域单元只能存储 1 个字节，现实数据的字节数是很多的，比如 `int a = 0x123456`，该整形数据 a 占据 4 个字节，就需要内存中开辟 4 个最小区域，并将原始数据拆分为 4 分存储，在这四份数据中，靠近 0x 的称为高字节序，远离 0x 的称为低字节序。

如图所示的小端存储：

![小端存储](../../images/net-program/net-01.svg)

判断当前系统的字节序：

```c
#include <stdio.h>

union un
{
  int a;
  char b;
};

int main()
{
  union un myun;
  myun.a = 0x12345678;

  if (myun.b == 0x78)
  {
    printf("小端存储\n");
  }
  else
  {
    printf("大端存储\n");
  }

  return 0;
}
```

### 1.2 字节序转换

不同系统主机之间传输数据一定要保证字节序的一致，若字节序不同，则需要转换。当然如果数据只有 1 个字，肯定是不需要考虑字节序的问题了。

**在网络中，网络协议规定通讯字节序统一采用大端存储。**，所以不同计算机之间通信时，都需要转换自己的字节序为网络字节序。

常见转换函数有：

```txt
# h 代表 host 主机，n 代表 network 网络，最后一个字母 l 代表长整型，s代表短整型
htonl：将32位主机字节序数据转换为网络字节序数据
htons：将16位主机字节序数据转换为网络字节序数据
ntohl：将32位网络字节序数据转换为主机字节序数据
ntohs：将16位网络字节序数据转换为主机字节序数据
```

转换函数示例：

```c
#include <stdio.h>
#include <arpa/inet.h>
·
int main()
{
  int a = 0x12345678;
  short b = 0x1234;

  printf("%#x\n", htonl(a)); // 0x78561234
  printf("%#x\n", htons(b)); // 0x3412

  return 0;
}
```

## IP 地址转换

IP 地址由于其格式问题，看起来是一个字符串（点分十进制格式），在操作系统使用时，必须转换为整型数据。

IP 地址在内存中的存储如图所示：

![IP地址存储格式](../images/net-program/net-02.png)

常见转换函数：

```txt
inet_pton：将点分十进制数串转换为32位无符号整数
inet_ntop：将32位无符号整数转换成点分十进制数串
```

示例：

```c
#include <stdio.h>
#include <arpa/inet.h>

int main()
{
  char ip_str[] = "192.168.1.109";
  unsigned int ip_int = 0;
  unsigned char *ip_p = NULL;

  inet_pton(AF_INET, ip_str, &ip_int);
  printf("in_uint = %d\n", ip_int);

  ip_p = (unsigned char *)&ip_int;
  printf("in_unint = %d, %d, %d, %d, \n", *ip_p, *(ip_p + 1), *(ip_p + 2), *(ip_p + 3));

  return 0;
}
```

## Socket

### Socket 概述

socket 即套接字，是一个文件描述符，代表通信管道的一个端点，操作 socket 与操作文件类似，可以使用 read、write 等函数。

socket 分类：

- SOCK_STREAM:流式套接字，用于 TCP
- SOCK_DGRAM:数据报套接字，用于 UDP
- SOCK_RAW:原始套接字，用于其他层次协议

发送示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
  // 创建用于UDP编程的套接字
  int sockfd;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1)
  {
    perror("failed to create");
    exit(1);
  }
  printf("sockfd = %d\n", sockfd); // 3

  // 设立服务端网络信息结构体
  struct sockaddr_in udpServer;
  udpServer.sin_family = AF_INET;
  udpServer.sin_addr.s_addr = inet_addr("192.168.1.22"); // 转换自己的ip地址
  udpServer.sin_port = htons(8080);

  // 发送数据
  char buf[128] = "";
  fgets(buf, 128, stdin);      // 从终端输入数据
  buf[strlen(buf) - 1] = '\0'; // 把 buf字符串中的 \n 转换为 \0
  int sendRes;
  sendRes = sendto(sockfd, buf, 128, 9, (struct sockaddr *)&udpServer, sizeof(udpServer));
  if (sendRes == -1)
  {
    perror("failed to send");
    exit(1);
  }

  // 绑定端口
  int bindRes;
  bind(sockfd, (struct sockaddr *)&udpServer, sizeof(udpServer));
  if (bindRes == -1)
  {
    perror("failed to bind");
    exit(1);
  }

  // 关闭套接字
  close(sockfd);
  return 0;
}
```

服务端接收数据：

```c
struct sockadd_in udpClient;
int recRes;
recRes = recvfrom(sockfd, buf, 128, 0, (struct sockaddr *)&udpClient,sizeof(udpClient));
if (bindRes == -1)
{
  perror("failed to receive");
  exit(1);
}
printf("数据：%s\n", buf);
```

## 广播

广播：一台主机向该主机所在子网内所有主机发送数据，广播只能使用 UDP 或者原始 IP 方式实现，不能使用 TCP 实现。

常见用到广播的协议：

- ARP 地址解析解析
- DHCP 动态主机配置协议
- NTP 网络时间协议

广播用来减少单服务器与多客户主机通信时的分组流通，只适用于局域网。

## 多播

多播：数据手仅在同一分组中进行，所以多播也称呼为组播。

多播可以用于广域网。

## TCP 并发服务器

TCP 本身并不能实现并发，因为其核心的两个函数 accept、recv 需要先后运行，无法实现一边连接客户端，一遍与其他客户端通信。

要实现并发，需要依赖于系统本身的多进程、多线程模型。

## 原始套接字

套接字类型有：

- 流式套接字（SOCK_STREAM），用来收发 UDP 协议数据
- 数据报套接字（SOCK_DGRAM），用于收发 TCP 协议数据
- 原始套接字(SOCK_RAW)，实现于系统核心，可以接收本机网卡所有数据帧。

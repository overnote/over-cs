## 一 文件操作API概述

#### 1.1 文件打开函数 open()

`open()`函数用于打开或者创建一个不存在的文件，并得到该文件的文件描述符：
```
# 函数原型
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);         

# 函数参数
pathname：要打开或者创建的文件名，可以是相对路径、绝对路径
flags：文件打开模式
mode：用于创建文件时给与权限

# 返回值
若成功返回一个最小且未被占用的文件描述符，失败则返回-1，并设置errno值
```

注意：任何文件使用之前必须调用`open()`打开，且完成读写后还需要调用`close()`关闭。

#### 1.2 文件关闭函数 close()

`close()`函数用于关闭文件：
```
# 函数原型
int close(int fd);

# 函数参数
fd：文件描述符

# 返回值
若成功返回一个最小且未被占用的文件描述符，失败则返回-1，并设置errno值
```

#### 1.3 文件读取函数 read()

`read()`函数用于从打开的设备或文件中读取数据：
```
# 函数原型 位于 unistd.h
ssize_t read(int fd, void *buf, size_t len);      // 

# 函数参数
fd: 文件描述符
buf: 读上来的数据保存在缓冲区buf中，每次调用read()，会从文件当前偏移开始读取len字节到buf
count: buf缓冲区存放的最大字节数

# 返回值
>0：读取到的字节数，在该函数中，返回小于len的正整数才是合法的
=0：文件读取完毕，仅仅表示到达文件末尾（end-of-file，EOF）：这不是错误，只是表示到达了文件结尾，无数据可读
-1：出错，并设置errno
```

贴士：
- EOF不是错误，但是如果文件本身没有可读的字节（不是读到末尾无数据可读），且描述符是以阻塞模式打开，调用会阻塞（sleep），直到有数据可读
  - EOF和阻塞完全不同，分别是两个概念：到达数据结尾、没有数据可读
- fd移动的长度由读取到的字节数决定，但是如果fd指向的对象不支持seek操作，如字符设备文件，则读操作总是从当前位置开始。

关于该函数的返回值：
- 返回值等于len：读取到的所有len个字节都被存储到buf中，结果与预期一致
- 返回值小于len，大于0：该现象原因很多，如：
  - 读取数据时出错、遇到信号中断，则结果小于len
  - 读取len字节之前已经到达了EOF
  - 此时再次执行read会把剩余的字节读到缓冲区或者给出错误信息
- 返回0：表示EOF
- 返回-1：也有很多情况
  - 把errno设置为`EINTR`，表示在读取之前收到信号，调用可以重新执行
  - 把errno设置为`EAGAIN`，表示无数据可用，操作阻塞，请求应该重新执行。注意这种情况只在非则色模式下发生
  - 把errno设置为非上述值，表示严重错误，重新执行读操作不会成功


#### 1.3 文件读取函数 write()
`write()`函数用于向打开的设备或文件中写数据：
```
# 函数原型
ssize_t write(int fd, const void *buf, size_t count);

# 函数参数
fd：文件描述符
buf：缓冲区，要写入文件或设备的数据
count：buf中数据的长度

# 返回值
成功：返回写入的字节数。错误：返回-1并设置errno
```

#### 1.3 移动文件指针 lseek()

所有打开的文件都有一个当前文件偏移量(current file offset),以下简称为cfo. cfo通常是一个非负整数, 用于表明文件开始处到文件当前位置的字节数. 读写操作通常开始于 cfo, 并且使 cfo 增大, 增量为读写的字节数. 文件被打开时, cfo 会被初始化为 0, 除非使用了 O_APPEND。  

使用 lseek 函数可以改变文件的 cfo：
```
# 函数原型
off_t lseek(int fd, off_t offset, int whence);

# 函数参数
fd：文件描述符
offset：含义取决于参数 whence：
    如果 whence 是 SEEK_SET，文件偏移量将设置为 offset。
    如果 whence 是 SEEK_CUR，文件偏移量将被设置为 cfo 加上 offset，offset 可以为正也可以为负。
    如果 whence 是 SEEK_END，文件偏移量将被设置为文件长度加上 offset，offset 可以为正也可以为负。
# 返回值
若lseek成功执行, 则返回新的偏移量。
```

lseek函数常用操作：
```
# 文件指针移动到头部
lseek(fd, 0, SEEK_SET);
# 获取文件指针当前位置
int len = lseek(fd, 0, SEEK_CUR);
# 获取文件长度
int len = lseek(fd, 0, SEEK_END);
# lseek实现文件拓展
off_t currpos;
#  从文件尾部开始向后拓展1000个字节
currpos = lseek(fd, 1000, SEEK_END); 
# 额外执行一次写操作，否则文件无法完成拓展
write(fd, “a”, 1);	// 数据随便写
```

#### 1.4 获取文件属性函数 stat()/lstat()

```
# 函数原型
int stat(const char *pathname, struct stat *buf);
int lstat(const char *pathname, struct stat *buf);

# 函数返回值
成功返回 0，失败返回 -1
```

注意：对于连接文件，stat函数获取的是链接文件指向的文件的属性信息，lstat函数获取的是链接文件本身的属性信息。

## 二 文件打开模式与函数creat()

文件打开模式有三种，在open参数中只能书写一个：
- O_RDONLY：只读打开
- O_WRONLY：只写打开
- O_RDWR：可读可写打开

在传入打开模式后，可以使用 `|` 追加可选参数：
- O_APPEND：表示追加。如果文件已有内容, 这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容。
- O_CREAT：若此文件不存在则创建它。使用此选项时需要提供第三个参数mode, 表示该文件的访问权限。文件最终权限：mode & ~umask
- O_EXCL：如果同时指定了O_CREAT,并且文件已存在,则出错返回。
- O_TRUNC：如果文件已存在, 将其长度截断为为0字节。
- O_NONBLOCK：对于设备文件, 以O_NONBLOCK方式打开可以做非阻塞I/O(NonblockI/O),非阻塞I/O。

由于`O_WRONLY | O_CREAT | O_TRUNC` 组合经常使用，专门提供了`creat()`函数：`int creat(cont char *name,  mode_t mode)`


## 三 文件操作示例

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(){

    // 打开文件
    int fd = open("./test.txt", O_CREAT | O_RDWR);
    if (fd == -1) {
        printf("文件打开出错");
        return 0;
    }
    printf("打开的文件描述符为：%d\n", fd);

    // 关闭文件：成功则返回0
    fd = close(fd);
    if (fd == -1) { 
         printf("文件关闭出错");
        return 0;
    }
    printf("文件已关闭：%d\n", fd);

    return 0;
}
```

## 四 API补充

- 复制文件描述符：`int dup(int oldfd);`，成功则返回最小且没被占用的文件描述符，失败则返回-1, 设置errno值
- 复制文件描述符：`int dup2(int oldfd, int newfd);`，成功则将oldfd复制给newfd, 两个文件描述符指向同一个文件，失败则返回-1, 设置errno值
- 改变属性：`int fcntl(int fd, int cmd, ... /* arg */ );`，调用失败返回-1, 并设置errno值
  - 若cmd为F_DUPFD, 复制文件描述符, 与dup相同，调用成功则返回一个新的文件描述符
  - 若cmd为F_GETFL, 获取文件描述符的flag属性值，调用成功则返回文件描述符的flags值
  - 若cmd为 F_SETFL, 设置文件描述符的flag属性，调用成功则返回0

fcntl函数常用的操作:
```
1 复制一个新的文件描述符:
int newfd = fcntl(fd, F_DUPFD, 0);
2 获取文件的属性标志
int flag = fcntl(fd, F_GETFL, 0)
3 设置文件状态标志
flag = flag | O_APPEND;
fcntl(fd, F_SETFL, flag)
4 常用的属性标志
O_APPEND-----设置文件打开为末尾添加
O_NONBLOCK-----设置打开的文件描述符为非阻塞
```
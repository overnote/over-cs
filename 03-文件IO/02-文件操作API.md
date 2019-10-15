## 一 文件操作API概述

#### 1.1 文件打开函数 open()

`open()`函数用于打开文件或者创建文件，并得到该文件的文件描述符，函数如下：
```
# 函数原型
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);    // 用户创建新文件，额外传入mode，确定权限  

# 函数参数
pathname：要打开或者创建的文件名，可以是相对路径、绝对路径
flags：文件打开方式，常用方式有：O_RDONLY(只读)、O_WRONGLY(只写）、O_RDWR(读写）
mode：用于创建文件时给与权限

# 返回值
成功：返回文件描述符
失败：返回-1，错误代码会保存在全局变量errno里
```

注意：任何文件使用之前必须调用`open()`打开，且完成读写后还需要调用`close()`关闭。  

在传入打开模式后，可以使用 `|` 追加标志位，即可选参数：
- O_APPEND：表示追加，如果文件已有内容，这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容。其实是每次写操作前，会更新文件位置指针，指向文件末尾。
- O_CREAT：若此文件不存在则创建它，使用此选项时需要提供第三个参数mode，表示该文件的访问权限
- O_TRUNC：如果文件已存在，并且有写权限，则标志位会把其长度截断为为0字节
- O_CLOEXEC：在执行新进程时，文件会自动关闭，这样就可以省去调用fcntl()来设置标志位，避免出现竞争

还有一些标志位不适用于普通文件，适用于FIFO、管道、socket、终端，不适用于普通文件
- O_NONBLOCK：非阻塞模式打开的操作，不会导致进程在I/O中阻塞（sleep）
- O_ASYNC：如果文件可读、可写，会产生信号，默认是SIGIO
- O_SYNC：同步I/O，贴士：该标志位对读操作无效，因为读操作本身是同步的

新建文件时候还需要确定文件的所有权限：
- 确定文件的所有者，一般文件所有者的uid即创建该文件的进程的有效uid。  
- 确定文件的所有用户组，默认使用创建进程的有效gid，这也是标准Linux的方式
  - SystemV采用标准方式
  - BSD新建文件的用户组是其父目录的gid，如果比较关注文件用户组，可以通过系统调用`fchown()`手动设置

使用了`O_CREAT`标志位，则是创建文件，此时需要mode参数，mode用来提供新建文件的权限。参数mode是常见的UNIX权限位集合，比如八进制数0644）（文件所有者可以读写，其他人只读）。针对mode中比特位不可移植性，POSIX引入了按位操作常数：S_IRWXU（文件所有者读、写、执行权限）、S_IRWXO（任何人都有读、写、执行权限）等等。最终写入磁盘的权限位由mode参数和文件创建掩码umask执行按位与操作得到。umask是进程级属性，有login shell设置，可以通过umask()修改。在系统调用open()中，umask位要和参数mode取反，因此 umask 022 和 mode参数0666 取反后，结果是0644。  

示例：下列代码对文件file进行写操作，如果文件不存在，假设umask为022，文件创建时的权限位0644，如果文件已存在，长度被截断为0
```c
    int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IWSUR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
    if (fd == -1) {
        ...
    }
```

为了代码可读性，这段代码可以改写为：
```c
    int fd;
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        ...
    }
```

#### 1.2 创建文件函数 creat()

由于`O_WRONLY | O_CREAT | O_TRUNC` 组合经常使用，专门提供了`creat()`函数：
```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int creat(cont char *name,  mode_t mode)
```

函数返回值与open一致，其实creat函数即是：
```
int open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
```

#### 1.2 文件关闭函数 close()

`close()`函数用于关闭文件：
```
# 函数原型
int close(int fd);

# 函数参数
fd：文件描述符

# 返回值
成功：返回文件描述符
失败：返回-1，并设置errno值
```

close()函数会取消当前进程的文件描述符fd与其关联文件之间的映射，调用后fd不再有效，内核可以随时重用。

#### 1.3 文件读取函数 read()

`read()`函数用于从打开的设备或文件中读取数据。
```
# 函数原型
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t len);      

# 函数参数
fd: 文件描述符
buf: 读上来的数据保存在缓冲区buf中
count: buf缓冲区存放的最大字节数

# 返回值
>0：读取到的字节数，在该函数中，返回小于len的正整数才是合法的
=0：文件读取完毕，仅仅表示到达文件末尾（end-of-file，EOF）：这不是错误，只是表示到达了文件结尾，无数据可读
=-1：出错，并设置errno
```

每次调用read()，都会从文件当前偏移开始读取len个字节到buf中。fd的文件指针会一直向前移动，移动长度由读取到的字节数决定，但是如果fd指向的对象不支持seek操作，如字符设备文件，则读操作总是从当前位置开始。  

关于该函数的返回值：
- 返回值等于len：读取到的所有len个字节都被存储到buf中，结果与预期一致
- 返回值小于len，大于0：该现象原因很多，如：
  - 读取数据时出错、遇到信号中断，则结果小于len
  - 读取len字节之前已经到达了EOF，EOF不是错误，但是如果文件本身没有可读的字节（不是读到末尾无数据可读），且描述符是以阻塞模式打开，调用会阻塞（sleep），直到有数据可读
  - 此时再次执行read会把剩余的字节读到缓冲区或者给出错误信息
- 返回0：表示EOF，
- 返回-1：也有很多情况
  - 把errno设置为`EINTR`，表示在读取之前收到信号，调用可以重新执行
  - 把errno设置为`EAGAIN`，表示无数据可用，操作阻塞，请求应该重新执行。注意这种情况只在非则色模式下发生
  - 把errno设置为非上述值，表示严重错误，重新执行读操作不会成功

非阻塞读：read()函数会引起阻塞，如果希望调用立即返回，即是非阻塞I/O，设置open()中的参数为O_NONBLOCK即可，此时文件以非阻塞模式打开，若没有数据可读，read()返回-1，并设置errno为EAGAIN。  

为了防止在非阻塞I/O在读取多个文件时，遗漏文件，必须对返回的errno进行判定：
```c
ret = read(fd, buf, BUFSIZ);
if (nr == -1) {
    if (errno == EINTR) {
        // 业务
    }
    if (errno == EAGAIN) {
        // 业务
    }
}
```

#### 1.4 文件读取函数 write()

`write()`函数用于向打开的设备或文件中写数据：
```
# 函数原型
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t len);

# 函数参数
fd：文件描述符
buf：缓冲区，要写入文件或设备的数据
count：buf中数据的长度

# 返回值
>0：实际写入的字节数，该数值有可能小于count
=0：未写入任何数据
=-1：发生错误，错误代码会保存在全局变量errno里
```
注意：
- write会将缓冲区buf中最多len个字节写入到文件中，执行成功则返回写入的字节数，并更新文件位置。  
- write不存在像read那样的部分写，也不存在EOF场景。即普通文件不需要循环执行写操作（socket文件需要循环写来保证写入了所有请求的字节）
- 使用O_APPEND模式打开文件，写操作不是从文件描述符当前位置开始，而是从文件末尾开始。常见使用场合有：多进程共享日志！
- 以非阻塞模式打开文件，write()会返回-1，errno被设置为EAGAIN，请求可以稍后发起，但是普通文件通常不会出现该情况。

## 二 文件读写API示例

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> 

int main(){

    // 打开要读取的文件
    int fd_read = open("./read.txt", O_RDONLY);
    if (fd_read == -1) {
        perror("main:");
    }

    // 打开要写入的文件
    int fd_write = open("./write.txt", O_WRONLY);
    if (fd_write == -1) {
        perror("main:");
    } 
    
    // 存储读写数据
    char buf[BUFSIZ];
    memset(&buf, 0, BUFSIZ);

    // 读取文件内容，并写入到另外一个文件
    ssize_t ret_read;
    ssize_t ret_write;
    while ( (ret_read = (read(fd_read, &buf, BUFSIZ))) != 0){

        if (ret_read == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("main:");
            break;
        }

        // 打印读取到的数据
        for (size_t i = 0; i < BUFSIZ; i++) {
            printf("%c", buf[i]);
        }

        // 写入文件
        ret_write = write(fd_write, &buf, (ssize_t)ret_read);
        if (ret_write == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("write:");
            break;
        }

    }
    
    
    // 关闭文件：成功则返回0
    if (close(fd_read) == -1) {
        perror("fd_close:");
    }
    if (close(fd_write) == -1) {
        perror("fd_close:");
    }

    return 0;
}
```

## 三 文件目录操作API概述

#### 3.1 打开目录函数 opendir()

```
# 函数原型
DIR *opendir(const char *name);
```

#### 3.2 读取目录函数 readdir()

```
# 函数原型
struct dirent *readdir(DIR *dirp);

# 函数返回值
读取到的目录指针
```

#### 3.3 关闭目录函数 closedir()

```
# 函数原型
int closedir(DIR *dirp);

# 函数返回值
成功返回0, 失败返回-1
```

#### 3.4 文件目录操作步骤

- 1 DIR *pDir = opendir(“dir”);   //打开目录
- 2 while((p=readdir(pDir))!=NULL){}  //循环读取文件
- 3 closedir(pDir);  //关闭目录

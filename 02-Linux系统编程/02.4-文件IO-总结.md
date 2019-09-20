## 一 I/O概述

#### 1.1 描述符

执行 I/O 操作的系统调用都以文件描述符，一个非负整数（通常是小整数），来指代打开的文件。  

标准文件描述符：  
| 文件描述符 | 用途 | POSIX名称 | stdio流 |
| ------ | ------ | ------ | ------ |
| 0 | 标准输入 | STDIN_FILENO | stdin |
| 1 | 标准输出 | STDOUT_FILENO | stdout |
| 2 | 标准错误 | STDERR_FILENO | stderr |

在程序中指代文件描述符时，使用数字0、1、2表示，或者采用`<unistd.h>`中定义的POSIX标准名称表示（推荐后者）。  

注意：`freopen()`函数无法保证上述描述符还能够一一对应上，因为该函数在流重新打开之际，会更换隐匿其中的文件描述符。  

#### 1.2 I/O操作的四个主要系统调用

- `fd = open(pathname, flags, mode)`
  - 打开pathname文件，返回文件描述符。如果文件不存在，则创建。
  - flags可以决定未存在文件创建方式，以及存在文件的打开方式：只读、只写、读写
  - mode指定了由 open()调用创建文件的访问权限，如果 open()函数并未创建文件，那么可以忽略或省略 mode 参数。
- ` numread = read(fd, buffer, count)`
  - 调用从 fd 所指代的打开文件中读取至多 count 字节数据，并存储到 buffer 中
  - read()调用的返回值为实际读取到的字节数。
  - 如果再无字节可读（例如：读到文件结尾符 EOF 时），则返回值为 0。
- `numwritten = write(fd, buffer, count) `
  - 调用从 buffer 中读取多达 count 字节的数据写入由fd 所指代的已打开文件中
  - write()调用的返回值为实际写入文件中的字节数，且有可能小于 count
- `status = close(fd)`
  - 在所有输入/输出操作完成后，调用 close()，释放文件描述符 fd 以及与之相关的内核资源

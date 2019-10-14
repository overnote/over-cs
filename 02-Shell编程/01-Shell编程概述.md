## 一 Linux程序

Linux的应用程序有两种文件类型：
- 可执行文件：是计算机可以直接运行的程序，相当于win中的exe文件
- 脚本文件：是一组指令的集合，由解释器执行，相当于win中的bat文件、BASIC程序等

Linux对文件的扩展名也没有强制要求，而是使用文件系统属性来指明一个文件是否为可执行程序。   

登入Linux后，用户与一个shell程序进行交互，shell中的命令程序往往遵循一些标准存储地址：
- `/bin`：系统二进制文件目录，存放启动系统时用到的程序
- `/usr/bin`：用户二进制文件目录，存放用户使用的标准程序
- `/usr/local/bin`：本地二进制文件目录，存放软件安装的程序
- `/sbin`：root用户可使用的程序目录
- `/usr/sbin`：root用户可使用的程序目录
- `/opt`：可选的操作系统组件、第三方程序存放目录

贴士：安装程序可以通过脚本配置到PATH环境变量中，要注意的是Linux使用冒号分割PATH，而win使用的是分号！Linux使用`/`分隔文件目录，而win使用反斜线！示例：
```
/usr/local/bin:/bin:/usr/bin
```

使用grep命令查询函数原型：
```
cd /usr/include
grep EXIT_ *.h
```

## 二 shell简介

Unix在设计之初，并不是一个图形化系统。shell命令行程序是Unix上最强大的交互工具。   

适用于Unix的shell有很多，如：
- sh：早期的shell
- ksh：许多商业版UNIX默认shell
- csh/tcsh/zsh：C shell的各种变体
- bash：GNU项目，免费开源，与ksh有许多相似之处。


Linux的shell，默认是GNU的bash，位于`/bin/sh` ：
```
# 查看bash版本
/bin/bash --version
```

## 三 shell语言初步使用

示例：查询系统中包含POSIX的文件，在命令行中输入：
```
$ for file in *         
> do                    # 此时命令行光标会改变为 >
> if grep -l POSIX $file
> then
> more $file
> fi
>done
```

shell脚本其实是shell的标准输入，编辑一个shell文件，myshell.sh(后缀不是必要的，只是笔者的标识)，来实现上述命令：
```sh
#!/bin/sh

# 这里是注释

for file in *
do
    if grep -q POSIX $file
    then
        echo $file
    fi
done

exit 0
```

执行：  
```
/bin/sh myshell.sh
```


注意：第一行`#!/bin/sh`会告诉系统使用哪个应用程序执行shell脚本。  

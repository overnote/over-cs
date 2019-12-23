## 一 shell简介

Linux的应用程序有两种文件类型：
- 可执行文件：是计算机可以直接运行的程序，相当于win中的exe文件
- 脚本文件（shell）：是一组指令的集合，由shell程序执行，相当于win中的bat文件、BASIC程序等

shell程序是Unix上的人机交互工具，Unix现在有许多shell程序，如：
- sh：早期的shell
- ksh：许多商业版UNIX默认shell
- csh/tcsh/zsh：C shell的各种变体
- bash：GNU项目，免费开源，与ksh有许多相似之处。

Linux上默认的shell是GNU的bash，位于`/bin/sh`：
```
bash --version          # 查看 bash 版本
```

`/etc/passwd`文件存储了用户的默认shell。  

## 二 shell语言初步使用

shell脚本其实是shell的标准输入，编辑一个shell文件，myshell.sh(后缀不是必要的，只是笔者的标识)，来实现上述命令：
```sh
#!/bin/sh

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

注意：shell使用 # 进行注释，但是第一行`#!/bin/sh`会告诉系统使用哪个应用程序执行shell脚本。  

## 三 shell执行机制

shell是一种解释性语言，无需编译。shell会fork一个子进程，然后调用exec()执行，exec()会把紫禁城的代码段替换为sh脚本程序的代码段，然后开始执行，父进程等待子进程的终止：
- 1 shell分析命令及参数
- 2 fork拷贝附近成pcb相关资源，为命令执行做准备
- 3 exec在fork得到的子进程中执行命令
- 4 执行完成，返回结果

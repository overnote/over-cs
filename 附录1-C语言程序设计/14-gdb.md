
## gdb
## 一 gdb初识

gdb是GNU 发布的一个强大的程序调试工具，使用方式：

```
# -g 编译
gcc -g main.c -o main 

# 启动gdb 调试
$ gdb main

# 进入gdb命令行，输入help查看帮助
help
help break          # 查看 break 相关子命令

# 退出
q
```

## 二 常用命令

程序运行：
```
run             # 可以缩写为 r
```

设置断点，接受行号或者函数名作为参数：
```
break syaHello if count<1
```

break也可设置条件断点，如查看断点：
```
info break 
```

使用 clear 可以清除断点
```

```
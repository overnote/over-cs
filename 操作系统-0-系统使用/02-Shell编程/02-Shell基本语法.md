## 一 Shell语法

### 1.1 基本使用

shell区分大小写，变量可以使用$获取，在命令行上直接使用shell：
```
name=lisi
echo $name
```

注意：如果字符串内有空格，那么必须使用引号括起来，如：`name="lis i"`，且等号两边不能有空格。  

read命令读取用户输入，并赋值给参数：
```
read name       # 会提示用户输入，并将输入结果赋值给参数name
echo name  
```

#### 1.2 引号

脚本文件中的参数以空白字符分割，如果想在一个参数中包含空白字符，必须给参数加上引号。
```sh
#!/bin/sh

name=lisi

echo $name                  # lisi
echo "$name"                # lisi
echo '$name'                # $name
echo \$name                 # $name
exit 0
```

#### 1.3 环境变量

linux提供了一些已经定义好的变量，通常大写命名（用户自定义变量往往是小写）：
```
$HOME   当前用户的家目录
$PATH   命令的目录列表
$PS1    即符号：[\u@\h \W]$
$PS2    即符号：>
$IFS    输入域分隔符。当shell读取输入时，它给出用来分隔单词的一组字符，通常是空格、制表符和换行符
$0      shell脚本的名字
$$      shell脚本的进程号，脚本程序通常会用它来生成一个唯一的临时文件，如/tmp/tmpfile_$$
$#      传递给脚本的参数个数
$1 $2.. 脚本在执行时传入的各个参数
$*      所有参数
$@      $*的另外一种表现形式，参数不会挤在一起
```

环境变量可以从父进程传给子进程，因此Shell进程的环境变量可以从当前Shell进程传给fork出来的子进程。用printenv命令可以显示当前Shell进程的环境变量。  


#### 1.4 test/[] 命令

检测一个文件是否存在：
```sh
if test -f fred.c       # 等同于 if [-f fred.c]
then
...
```

支持的条件
- =：两个字符串相等，则结果为真
- !=：=两个字符串不相等，则结果为真
- -n：字符串不为空，则为真，用法：`-n string`
- -z：字符串为null（一个空串），则结果为真

一些算术比较：
- -eq：等于
- -ne：不等于
- -gt：大于
- -ge：大于等于
- -lt：小于
- -le：小于等于
- !：如果表达式为假，则结果为真，反之亦然

文件条件测试：
```
-d file 如果文件是一个目录则结果为真
-e file 如果文件存在则结果为真。要注意的是，历史上-e选项不可移植，所以通常使用的是-f选项
-f file 如果文件是一个普通文件则结果为真
-g file 如果文件的set-group-id位被设置则结果为真
-r file 如果文件可读则结果为真
-s file 如果文件的大小不为0则结果为真
-u file 如果文件的set-user-id位被设置则结果为真
-w file 如果文件可写则结果为真
-x file 如果文件可执行则结果为真
```

示例：
```sh
#!/bin/sh

if [ -f /bin/bash ]
then
    echo "/bin/bash exists"                     # 会被打印
fi

if [ -d /bin/bash ]
then    
    echo "/bin/bash is a directory"
else
    echo "/bin/bash is not a directory"         # 会被打印
fi
```
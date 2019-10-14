## 一 管道与重定向

#### 1.1 重定向输出

示例：将shell中的命令输出结果的结果输入到文件中：
```
# 如果该文件已经存在，则会覆盖原有内容
ls -l > myoutput.txt   

# 输出内容会被追加到文件的尾部
ls -l >> myoutput.txt

# 将标准输出和标准错误分别重定向到不同的文件中：0代表标准输入，1代表标准输出，2代表标准错误
kill -HUP 1234 >info.txt 2>err.txt

# 将两组输出去重定向到一个文件
# 将标准输出和标准错误分别重定向到不同的文件中：0代表标准输入，1代表标准输出，2代表标准错误
kill -l 1234 >info.txt 2>&1         # 注意顺序
```

#### 1.2 重定向输入

示例：
```
more < info.txt
```

#### 1.3 管道

管道符可以用来连接进程，示例：使用sort命令对ps命令的输出进行排序
```
# 无管道做法
ps > outinfo.txt
sort outinfo.txt > sortinfo.out

# 利用管道做法
ps | sort > sortinfo.out

# 可以加入多个进程，比如分页显示
ps | sort | more
```



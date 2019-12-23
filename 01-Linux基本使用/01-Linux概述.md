## 一 Linux简介

### 1.1 Linux的发展 

1969 年，贝尔实验室（ AT&T的一个部门）的 Ken Thompson 在 Digital PDP-7 小型机上首次实现了 UNIX 系统，1973 年，UNIX被贝尔实验室的 Dennis Ritchie/Ken Thompson 以C语言重写，至此Unix步入了发展轨道，并产生了两大变种：
- BSD： 在加州伯克利大学，在 Ken Thompson等人努力下，发布的Unix版本。该版本时商业
- System V：AAT&T公司催生的Unix版本

20世纪80年代末， 开源组织 GNU 项目虽然制作了大量Unix实现，但因为Unix收费，GNU缺少一个能够有效运作的内核。  

1991 年，Linus Torvalds 被 Minix 内核（由 AndrewTanenbaum教授编写的教学用操作系统）“灵魂附体”，开发了一款能够在 Intel x86-32 架构上正常运作的内核。  

GUN与Linux共同发展，诞生了今天Linux上的大量的工具与扩展。  

### 1.2 Linux的分支

在Linux内核（linux kernel）基础上，诞生了许多商业的、免费开源的Linux分支，比较著名的有：
- Red Had：红帽系Linux是Linux甚至开源世界的领导者，其拥有两个不同的发行版本，以及一个衍生分支
    - 商用版：即 Red Hat Enterprise Linux ，专注于企业应用，提供全套技术支持
    - 社区版：即 Fedora ，免费开源版
    - CentOS：该版本是通过收集 Red Hat Enterprise 为遵守开源许可协议而必须开放的源代码，行成的“复制品”，免费Linux的最佳选择！
- Debian：保持原始的Linux 精神，由Linux 社区开发，并且完全向用户免费提供，也是桌面 Linux 的代表，其衍生品有：
  - Ubuntu
- SUSE：由美国公司Novell 运作，和Red Hat 一样有两个版本
  -  商业版：即 SUSE Linux Enterprise ，由Novell 提供技术和支持
  -  免费版：即 openSUSE，免费开源，由Linux社区维护

## 二 Linux的文件目录

- `/`：根目录
- `/root`：系统用户-管理员用户的文件目录，管理员账号统一是root
- `/home`：系统用户-普通用户的文件目录，该目录可以简写为 `~`
- `/usr`：应用程序存放目录
  - ：存放应用程序的二进制文件
  - `/usr/share`：存放应用程序的共享数据
  - `/usr/local`：应用程序升级文件目录
  - `/usr/share/doc`：存放系统说明文件存放目录
  - `/usr/share/man`：程序说明文件存放目录
- `/bin`：可执行二进制文件的目录，如常用的命令ls、tar、mv、cat等，类似的目录还有`/usr/bin`
- `/sbin`：可执行二进制文件的目录，但是该目录是给系统管理员使用的命令，一般用户只能查看，类似的目录还有`/usr/sbin`、`/usr/local/sbin`
- `/etc`：系统配置文件存放的目录
- `/lib`：函数库目录，类似的还有`/usr/lib`、`/usr/local/lib`
- `/tmp`：一般用户或正在执行的程序临时存放文件的目录，任何人都可以访问，重要数据不可放置在此目录下
- `/srv`：服务启动之后需要访问的数据目录，如 www 服务需要访问的网页数据存放在 `/srv/www` 内
- `/var`：放置系统执行过程中经常变化的文件，如随时更改的日志文件 `/var/log`

## 三 Linux的命令

Linux的命令包含两种：
- 内部命令：Shell解析器的一部分，如 cd pwd help；
  - 使用 `help 命令`，如 `help cd` 可以查看命令 cd 的帮助文档
- 外部命令：独立于Shell的程序，如 ls mkdir cp。其实外部命令就是外部来操作了Linux底层的库、命令后调用的CPU。
  - 使用 `man 命令`，如 `man ls` 可以查看命令 ls 的帮助文档  

Linux命令格式：
```
格式：  comman [-options] [parameter]   # []表示可选，分别为： 命令 选项 参数    
示例：	ls -l /usr          
```

## 四 常用命令
```
# 三个查看命令
ls			        # 显示当前目录下的文件与文件夹名
cat                 # 显示文件内容，用法：cat test.txt
more                # 分屏显示文件内容，按下q键退出显示，按下h键可以获取帮助，用法：more test.txt

# ls的参数
-a                  # 显示隐藏文件
-l                  # 以行列表方式查看文件
-h                  # 文件大小以合适的单位显示

# 常见用法			
ls /bin			    # 查看根目录下的bin文件夹下的东西（绝对路径）
ls Documents 	    # 查看Documents文件夹下的东西（相对路径）		
ls *.txt			# 显示以txt为后缀的文件
ls *.*
ls *.t?t			# 显示所有以 .t任意字符t 结尾的文件,*表示任意个字符,？表示单个字符
ls *.t[xn]t		    # 显示txt，tnt后缀文件
ls *.t[a-f]t		# 显示tat 到 tft 中所有的后缀的文件
ls \*a			    # 文件名有*需要转义,可以将所有命令合并书写：ls -lah
```

进入命令：
```
cd			        # 进入文件夹
cd .		        # 进入当前路径
cd ..		        # 回到当前路径的上级目录
cd /		        # 回到根目录
cd -		        # 回到上次执行命令的目录
cd ~		        # 进入当前用户的家目录
```

## 附

Linux安装时的一些网络术语：
- NAT：通常一个企业只有一台机器支持对外联机，其他计算机都要通过该机器才能连接到因特网上，此时需要使用IP分享器来让这个机器分享给所有员工使用
- DHCP：用于自动分配IP
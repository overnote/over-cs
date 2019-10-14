## 一 Linux简介

#### 1.1 Unix的发展 

1969 年，贝尔实验室（ AT&T 的一个部门）的 Ken Thompson 在 Digital PDP-7 小型机上首次实现了 UNIX 系统。  

至 1973 年， UNIX 已经被移植到了 PDP-11 小型机上，并以 C 语言对其进行了重写， C 编程语言是由贝尔实验室的 Dennis Ritchie 设计并实现的。  

Unix两大变种：
- 在加州伯克利大学，在 Ken Thompson等人努力下，于1979 年发布了属于自己的 UNIX 发布版—BSD，该版本成为为日后商业Unix的基石之一
- AT&T 公司被获准销售 UNIX，催生了System V，是某些商业Unix的基石之一

20 世纪 80 年代末， GNU 项目已经开发出了一套几乎完备且可以自由分发的UNIX 实现，但独缺一颗能够有效运作的内核。   

#### 1.2 Linux的发展

1991 年， Linus Torvalds 被 Minix 内核（由 AndrewTanenbaum 编写）“灵魂附体”，于是便开发出了一颗能够在 Intel x86-32 架构上正常运作的内核。  

#### 1.3 Linux的分支

Red Hat Linux   
    Red Hat 公司一直是Linux 乃至开源世界的领导者。 其有两个不同的发行版本，一个商用版，称为 Red Hat Enterprise Linux,专注于企业应用，并提供全套技术支持；另一个是免费开源版，称为 Fedora ,其开发依托于Linux 社区。   

SUSE Linux  
   由美国公司Novell 运作，和Red Hat 一样有两个版本：企业级应用 SUSE Linux Enterprise;免费开源版本 openSUSE，前者有Novell 提供技术和支持，后者由Linux 社区维护。

Centos  
    这是很有意思的一个版本，是通过收集 Red Hat Enterprise 为遵守开源许可协议而必须开放的源代码，行成了其的“复制品”，不愿意花钱的企业的最佳选择！

Debian 和 Ubuntu  
    保持原始的Linux 精神，由Linux 社区开发，并且完全向用户免费提供，桌面 Linux 的代表。

Red Flag Linux (红旗Linux)  
    来自北京中科红旗软件技术有限公司的产品，已成为亚洲最大的Linux 产品发行商。04年主导发布了 企业级Linux 系统 Asianux。优势：本地化服务 & 中文支持

发行版选择
- 企业环境首选： Red Hat Enterprise Linux  －>  SUSE Linux Enterprise  －> Centos  
- 个人/桌面首选： Ubuntu & Debian

## 二 Linux安装

#### 2.1 win虚拟机安装方式

一般我们使用虚拟机来安装Linux，常见的虚拟机软件是 VMWare，这里我们使用CentOS7来作为安装示例。  

注意：新手在安装Linux时，最好不选择最小化安装，安装图形化界面。  

安装步骤：
- 0 打开VMWare10以上版本，新建虚拟机，一切按照默认设置即可。
- 1 点击虚拟机顶部导航-文件-新建虚拟机
- 2 选择默认的推荐
- 3 选择默认程序安装的光盘映像
- 4 虚拟机名称（随便起），文件位置（即系统安装到哪里--自己手动选择）
- 5 最大磁盘大小（虚拟机系统可用空间）全部默认即可
- 6 自定义硬件--设置内存、网络等
- 7 进入系统安装环节：将显示的黄色区域全部点开调整即可。

注意：
- 最后一步中，这里注意新手不要选择最小安装，可以点击软件选择：GNOME桌面
- 安装时候要设置root账户密码

Ubuntu分辨率过小问题：解决办法是重新安装虚拟机下的`VMware Tools`，但是很多情况下这个安装按钮是灰色的，解决办法：
```
1 挂载镜像文件:虚拟机->设置->硬件->CD/DVD.右边“连接”下面选择“使用IOS镜像文件”，浏览选择VMware目录下面linux.iso

2 挂载成功后，在虚拟机右下角cd/dvd,图标上单击选择“连接“。这样我装的CentOS7桌面就能看到光盘文件了

3 打开光盘文件，文件夹中有名字为VMwareTools............tar.gz的压缩包，点击上面的解压缩按钮"extract",浏览选择需要解压到的目录，这里选择的的是home/documents目录,开始解压。

4 打开终端界面，输入cd documents/vmware-tools-distrib 回车，再输入sudo ./vmware-install.pl 回车，以后可能需要输入 yes,一直回车，重启虚拟机。
这时候就可以通过VMware的自适应客户机大小来改变分辨率问题。
```

#### 2.2 win wsl安装ubuntu

win10中可以直接在Mircrosoft Store中安装Ubuntu，安装启动后系统自动提示设置一个普通用户和密码，设置完毕后，即可使用。  

但是如果需要使用本地xshell连接wsl，还需要以下设置：
```
# 修改root密码
sudo passwd root

# 设置ssh服务
sudo vim /etc/ssh/sshd_config
Port 2222                       # 设置ssh的端口号, 由于22在windows中有别的用处, 尽量不修改系统的端口号
PermitRootLogin yes             # 可以root远程登录
PasswordAuthentication yes      # 密码验证登录

# 启动ssh服务
sudo service ssh --full-restart

# 如果提示 Could not load host key: /etc/ssh/ssh_host_rsa_key 等信息，则设置一个key即可
su root
ssh-keygen -t rsa -b 2048 -f /etc/ssh/ssh_host_rsa_key
```

此时就可以使用类似xshell的工具登录：
```
地址为：127.0.0.1
端口为：2222
```

安装一些gcc工具，就可以进行C/C++开发了：
```
apt update
apt install gcc g++ gdb --fix-missing
```

## 三 安装注意事项

无法联网：
```
虚拟机软件选择桥接模式
输入 reboot 重启命令即可
```
## 一 Linux简介

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
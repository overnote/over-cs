## 一 文件目录操作API概述

#### 1.1 打开目录函数 opendir()

```
# 函数原型
DIR *opendir(const char *name);
```

#### 1.2 读取目录函数 readdir()

```
# 函数原型
struct dirent *readdir(DIR *dirp);

# 函数返回值
读取到的目录指针
```

#### 1.3 关闭目录函数 closedir()

```
# 函数原型
int closedir(DIR *dirp);

# 函数返回值
成功返回0, 失败返回-1
```

## 二 读取目录的步骤

- 1 DIR *pDir = opendir(“dir”);   //打开目录
- 2 while((p=readdir(pDir))!=NULL){}  //循环读取文件
- 3 closedir(pDir);  //关闭目录
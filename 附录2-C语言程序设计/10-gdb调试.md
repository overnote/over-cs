## 一 gdb调试入门

使用printf()函数固然能够调试出错误，但是需要在不同位置书写打印函数，调试完毕后也不利于清理。  

gdb调试可以完全操控程序的运行。  


新建main.c文件，如下所示：
```c
#include <stdio.h>

int add_range(int low, int high) {
    int i, sum;
    for (i = low; i <= high; i++){
        sum = sum + i;
    }
    return sum;
}

int main(void) {
    int sum1, sum2;
    sum1 = add_range(1, 10);
    sum2 = add_range(1, 100);
    printf("sum1=%d\n sum2=%d\n", sum1, sum2);     
    return 0;
}
```
上述程序输出的结果为：
```
sum1=55
 sum2=5105          # 按照正常的理解这里应该输出 5050
```

使用gdb调试，编译方式为：
```
gcc main.c -o main
gdb ./main
```

`-g`选项的作用是在可执行文件中加入源码信息，比如可执行文件中第几条机器指令对应源码第几行，但并不是把整个源文件嵌入到可执行文件，调试时必须保证gdb能找到源文件。  

使用gdb命令后，会进入一个类似shell的界面，此时可以输入gdb相关的命令，如：
```
help        # 输出gdb帮助信息
list        # 简写为 l，输出当前程序源码信息， l add_range 可以列出该函数信息
直接回车     # 重复上述命令
quit        # 退出gdb    
start       # 执行程序
next        # 简写为 n，继续执行
step        # 简写为 s
```
TODO
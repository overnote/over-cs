## 多文件

往往将函数的声明放在头文件中，其他地方使用该函数，只用引入该头文件即可。

test.h头文件：
```c
extern int min(int x, int y);
extern int max(int x, int y);
```

test.c源文件：
```c
int min(int x, int y) {
    return x>y ? x:y;
}

int max(int x, int y) {
    return x<y ? x:y;
}
```

main函数调用:
```c
#include "test.h"
int mian() {
    int a = 10, b = 20, minNum;
    minNum = min(a, b);
    return 0;
}
```

当一个项目比较大时，往往都是分文件，这时候有可能不小心把同一个头文件 include 多次，或者头文件嵌套包含。常用2种解决方式：
- #ifndef 方式
- #pragma once 方式

方法一：
```c
#ifndef __SOMEFILE_H__
#define __SOMEFILE_H__

// 声明语句

#endif
```

方法二：
```c
#pragma once

// 声明语句
```



 

## makefile
## 一 Make概述

企业级项目往往拥有很多源码文件，手动编译非常困难，Makefile可以确定整个工程的编译规则，只需要一个make命令，就可以实现 `自动化编译`，避免了人工操作大量文件的困难。大多软件都支持make命令，如Visual C++的nmake，Linux下GNU的make。  

通常在一个项目里，我们的规则是：
- 如果这个工程没有编译过，那么我们的所有C文件都要编译并被链接。
- 如果这个工程的某几个C文件被修改，那么我们只编译被修改的C文件，并链接目标程序。
- 如果这个工程的头文件被改变了，那么我们需要编译引用了这几个头文件的C文件，并链接目标程序。

make命令会自动智能地根据当前的文件修改的情况来确定哪些文件需要重编译，从而自己编译所需要的文件和链接目标程序。  

## 二 Make示例

```
hello_demo : hellospeak.o speak.o                                           
        gcc -o hello_demo hellospeak.o speak.o                               

hellospeak.o : hellospeak.c speak.h                                           
        gcc -c hellospeak.c                                                
speak.o : speak.c  speak.h                                                  
        gcc -c speak.c                                                    
clean :                                                                  
        rm hello_demo speak.o  hellospeak.o 
```

- target也就是一个目标文件，可以是Object File，也可以是执行文件。还可以是一个标签（Label），对于标签这种特性，暂不叙述。
- prerequisites就是，要生成那个target所需要的文件或是目标。
- command也就是make需要执行的命令。（任意的Shell命令）

这是一个文件的依赖关系，也就是说，target这一个或多个的目标文件依赖于prerequisites中的文件，其生成规则定义在command中。说白一点就是说，prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行。这就是Makefile的规则。也就是Makefile中最核心的内容。  


## 三 Make编写

#### 3.1 Make工作流程

- 1、make会在当前目录下找名字叫“Makefile”或“makefile”的文件。
- 2、如果找到，它会找文件中的第一个目标文件（target），在上面的例子中，他会找到“hello_demo”这个文件，并把这个文件作为最终的目标文件。
- 3、如果hello_demo文件不存在，或是hello_demo所依赖的后面的 .o 文件的文件修改时间要比hello_demo这个文件新，那么，他就会执行后面所定义的命令来生成hello_demo这个文件。
- 4、如果hello_demo所依赖的.o文件也不存在，那么make会在当前文件中找目标为.o文件的依赖性，如果找到则再根据那一个规则生成.o文件。（像不像堆栈过程？）
- 5、当然，我们的C文件和H文件都存在，于是make会生成 .o 文件，然后再用 .o 文件生命make的终极任务，也就是执行文件hello_demo了。

一个Makefile里我们发现经常会由重复的内容，如上面范例中的前两行中的 hellospeak.o speak.o  。 如果我们的工程需要加入一个新的[.o]文件，那么我们我们好几个地方都需要修改原来的makefile。当然，我们的makefile并不复杂，所以在两个地方加也不累，但如果makefile变得复杂，那么我们就有可能会忘掉一个需要加入的地方，而导致编译失败。所以，为了makefile的易维护，在makefile中我们可以使用变量。makefile的变量也就是一个字符串，完全可以理解成C语言中的宏。  

变量定义:   变量名 = 值   ##使用shell script 的语法如：
```
    objects =  hellospeak.o speak.o  
```

我们上面的makefile 就可以变成:
```
objects = hellospeak.o speak.o                                               
hello_demo : $(objects)                                                   
        gcc -o hello_demo $(objects)                                        

hellospeak.o : hellospeak.c speak.h                                           
        gcc -c hellospeak.c                                                
speak.o : speak.c  speak.h                                                  
        gcc -c speak.c                                                    
clean :                                                                  
        -rm hello_demo  $(objects)    
```

#### 3.2 Make 自动推导


make很强大，它可以自动推导文件以及文件依赖关系后面的命令，于是我们就没必要去在每一个[.o]文件后都写上类似的命令，因为，我们的make会自动识别，并自己推导命令。  

只要make看到一个[.o]文件，它就会自动的把[.c]文件加在依赖关系中，如果make找到一个hello.o，那么hello.c，就会是hello.o的依赖文件。并且 gcc -c hello.c 也会被推导出来，于是，我们的makefile再也不用写得这么复杂。我们的是新的makefile又出炉了。  

```
objects = hellospeak.o speak.o                                                   
hello_demo : $(objects)                                                        
        gcc -o hello_demo $(objects)                                            
                                                                           
$(objects):speak.h                                                             
#hellospeak.o : hellospeak.c speak.h                                              
#       gcc -c hellospeak.c                                                    
#speak.o : speak.c  speak.h                                                     
#       gcc -c speak.c                                                        
                                                                           
.PHONY : clean                                                              
clean :                                                                      
        -rm hello_demo  $(objects) 
```


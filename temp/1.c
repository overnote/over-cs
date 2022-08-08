#include <stdio.h>

int main(){
    // 1.235, +0000789, +789 
    printf("%0.3f, %+08d, %+-8d\n", 1.23456, 789, 789);
}

#include<stdio.h>

int main(){
 int count = 0;
  while(1){
    FILE *fp = fopen("tmp.txt","w+");
    if(!fp){
       printf("fopen open file[%d]",count);
       return 0;
    }
    count++;
  }
 return 0;
}
/*
 * 结果：最多只能打开1024  但是结果为1021 
 * 原因：已经有三个文件在进程创建的时候已经打开了，stdin，stdout stderr
 * /

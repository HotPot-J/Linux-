#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sched.h>
int main(){
 
  pid_t pid = fork();
  if(pid==0){
    //将当前进程绑定在0号cpu上
    cpu_set_t set; 
    CPU_ZERO(&set);//清空初始化
    CPU_SET(0,&set);//将0号cpu添加到set中
    if(sched_setaffinity(0,sizeof(set),&set)==0){
       while(1){
       printf("i am child %d\n",getpid());
       }
    }
    else{
      perror("child sched_setaffinity error\n");
    }
  }
  else{
    //将当前进程绑定在1号cpu上
    cpu_set_t set; 
    CPU_ZERO(&set);//清空初始化
    CPU_SET(1,&set);//将1号cpu添加到set中
    if(sched_setaffinity(0,sizeof(set),&set)==0){
      while(1){
      printf("i am father %d\n",getpid());
      } 
    }
    else{
      perror("father sched_setaffinity error\n");
    }
}

  return 0;
}

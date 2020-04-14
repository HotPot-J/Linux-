#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void* thread_start(void* arg){
(void)arg;
  printf("I am new thread\n");
  sleep(30);
  printf("pthread_exit\n");
  pthread_exit(NULL);
  while(1){
   //如果上一行代码没结束掉我们的线程就打印111
   printf("1111111\n");
  }
  return NULL;
}
int main(){

	pthread_t tid;
    int ret = pthread_create(&tid,NULL,thread_start,NULL);
   printf("%p\n",pthread_self());
   pthread_cancel(pthread_self());//结束线程 该函数可以结束主线程
	while(1){
     printf("i am main thread\n");
	 sleep(1);
   }
return 0;

}

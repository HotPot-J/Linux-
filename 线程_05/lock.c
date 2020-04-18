#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

//定义互斥锁
 pthread_mutex_t lock1;
 pthread_mutex_t lock2;

void* ThreadA(void* arg){
  (void)arg;
  //线程分离
  pthread_detach(pthread_self());
  //加锁
  pthread_mutex_lock(&lock1);
  sleep(3);//等待一下 让线程B一定可以获取到互斥锁2
  pthread_mutex_lock(&lock2); //模拟实现A(1)-->2  B(2)-->1的死锁情况
  return NULL;
}
void* ThreadB(void* arg){
  (void)arg;
  pthread_detach(pthread_self());
  pthread_mutex_lock(&lock2);
  sleep(3);
  pthread_mutex_lock(&lock1);
  return NULL;
}

int main(){
//初始化入口函数
 pthread_mutex_init(&lock1,NULL);
 pthread_mutex_init(&lock2,NULL);
  int a  = 1;
 pthread_t tid[2];
 int ret = 0;
 ret = pthread_create(&tid[0],NULL,ThreadA,NULL);
  if(ret<0){
   //创建失败
    perror("pthread_create");
	return 0;
  }
 ret = pthread_create(&tid[1],NULL,ThreadB,NULL);
  if(ret<0){
   //创建失败
    perror("pthread_create");
	return 0;
  }

  while(1){
	//防止主线程退出看不见死锁现象：
    sleep(1);
	printf("I am main thread!\n");
  }
//销毁
pthread_mutex_destroy(&lock1);
pthread_mutex_destroy(&lock2);
return 0;
 
 }
       

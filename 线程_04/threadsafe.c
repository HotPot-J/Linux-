/*
 *写一个黄牛抢票程序
    1.定义：定义互斥锁为全局变量
	2.初始化：在创建线程之前进行初始化
	3.销毁：在线程推出之后进行销毁
	4加锁：在访问临界资源之前进行加锁操作
	5.解锁：在所有有可能退出的地方进行解锁
 * */

//模拟实现一个抢票系统
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<sched.h>
#include<stdlib.h>
int ticket = 100; //定义票的个数
pthread_mutex_t mutex; //定义互斥锁

//定义入口函数
 void* route(void* arg){
   char* id = (char*)arg;
   while(1){
     //加锁
	 pthread_mutex_lock(&mutex);
	 if(ticket>0){
	   usleep(1000);
	   printf("%s sells ticket:%d\n",id,ticket);
	   --ticket;
	   //解锁
	   pthread_mutex_unlock(&mutex);
	 }
	 else{
	   //在所有有可能退出的地方进行解锁
	   pthread_mutex_unlock(&mutex);
	   break;
	 }
   }
 }
int main(){

	pthread_t t1,t2,t3,t4;//定义四个线程标识符
	pthread_mutex_init(&mutex,NULL);//初始化互斥锁
    
	//创建4个线程
	pthread_create(&t1,NULL,route,"thread 1");
	pthread_create(&t2,NULL,route,"thread 2");
	pthread_create(&t3,NULL,route,"thread 3");
	pthread_create(&t4,NULL,route,"thread 4");
    
	//重置joinable属性（线程分离） 当线程退出时，系统可自动释放线程资源
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);

	//销毁互斥量
	pthread_mutex_destroy(&mutex);
return 0;
}

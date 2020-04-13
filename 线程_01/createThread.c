#include<stdio.h>
#include<pthread.h>
#include<unistd.h>//sleep函数
#include<stdlib.h>

//为arg定义一个自定义类型 并在堆上开辟空间 进行传值
typedef struct thread_info{
     int thread_num_;
	 thread_info(){ 
	 thread_num_ = -1;
	 }
 }thread_info;
void* thread_start(void* arg){
	thread_info* ti = (thread_info*)arg;
   while(1){
	printf("I am thread%d\n",ti->thread_num_);
    sleep(1);
   }
   //对于传递进来的堆上开辟的内存，可以在线程入口函数结束的时候进行释放，不会导致
   //内存泄漏的风险 
   delete ti;
   return NULL;
}
int main(){

	pthread_t tid;
	//int i = 10;//i是临时变量 传不过去的
	//int ret = pthread_create(&tid,NULL,thread_start,(int*)&i);
	for(int i = 0;i<3;i++){
	thread_info* threadinfo = (thread_info*)malloc(sizeof(thread_info));
	threadinfo->thread_num_ = i;
    int ret = pthread_create(&tid,NULL,thread_start,(void*)threadinfo);
	if(ret!=0){
		//线程创建失败
	 perror("pthread_create");
	 return 0;
	}
	}
	while(1){
	 printf("I am main thread\n");
	 sleep(1);
	}
return 0;

}

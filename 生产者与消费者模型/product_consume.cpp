#include<unistd.h>
#include<pthread.h>
#include<iostream>
#include<cstdio>
#include<queue>
using namespace std;
//线程安全的队列

//安全队列的容量
#define CAPACITY 10

//线程个数
#define THREADCOUNT 1
class BlockQueue{
public:
	BlockQueue(size_t Capacity)
	{
	 _Capacity = Capacity;
	 pthread_mutex_init(&mutex_,NULL);
	 pthread_cond_init(&consumeCond,NULL);
	 pthread_cond_init(&productCond,NULL);
	}

	~BlockQueue()
	{
	 pthread_mutex_destroy(&mutex_);
     pthread_cond_destroy(&consumeCond);
	 pthread_cond_destroy(&productCond);
	}

	void Push(int& Data){
	  pthread_mutex_lock(&mutex_);
	  while(IsFull()){
	     pthread_cond_wait(&productCond,&mutex_);
	  }
	  Queue.push(Data);
	  pthread_mutex_unlock(&mutex_);
	  pthread_cond_signal(&consumeCond);
	}

	void Pop(int* Data){
	  pthread_mutex_lock(&mutex_);	
	  while(Queue.empty()){
	    pthread_cond_wait(&consumeCond,&mutex_);
	  }
	   *Data = Queue.front();
	   Queue.pop();
	  pthread_mutex_unlock(&mutex_);
	  pthread_cond_signal(&productCond);
	}
private:
	bool IsFull(){
	  if(Queue.size()==_Capacity){
	    return true;
	  }
	  return false;
	}
	  size_t _Capacity;
	  queue<int> Queue;
      //互斥
      pthread_mutex_t mutex_;
	  //同步
	  pthread_cond_t consumeCond;
	  pthread_cond_t productCond;
};
void* ConsumeStart(void* arg){
   BlockQueue* bq = (BlockQueue*)arg;
   while(1){
    int Data;
	bq->Pop(&Data);
	printf("ConsumeStart[%p][%d]\n",pthread_self(),Data);
   }
	return NULL;
}

void* ProductStart(void* arg){
   BlockQueue* bq = (BlockQueue*)arg;
   int i =0;
   while(1){
     bq->Push(i);
	printf("ProductStart[%p][%d]\n",pthread_self(),i);
	++i;
   }
	return NULL;
}
int main(){
	BlockQueue* bq = new BlockQueue(1);//创建一个线程安全队列对象
    pthread_t com_tid[THREADCOUNT],pro_tid[THREADCOUNT];
	int i = 0,ret = 0;
	for(;i<THREADCOUNT;++i){
     ret =  pthread_create(&com_tid[i],NULL,ConsumeStart,(void*)bq);
	 if(ret<0){
	   printf("create thread failed\n");
	   return 0;
	 }
	}
	for(i = 0;i<THREADCOUNT;++i){
      ret =  pthread_create(&pro_tid[i],NULL,ProductStart,(void*)bq);
	 if(ret<0){
	   printf("create thread failed\n");
	   return 0;
	 }
	}
   //线程分离
   for(i = 0;i<THREADCOUNT;++i){
     pthread_join(com_tid[i],NULL);
     pthread_join(pro_tid[i],NULL);    
   }
   delete bq;
   bq = NULL;
	return 0;
}

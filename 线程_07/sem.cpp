#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<vector>
#include<semaphore.h>
#include<cstdio>
using namespace std;


/*
 *  5. 使用Posix信号量实现一个生产者消费模型
 *    1.队列--》环形队列--》数组实现
 *       插入：
 *           1.插入
 *           2.计算下一个插入的位置 ：模运算即可实现环形队列
 *       
 *     2.安全队列
 *           同步
 *           写:sem_init(&ProWrite_,0,数组元素的个数);
 *              sem_wait(&ProWrite_);
 *              arr[ProWrite] = 10;
 *              sem_post(&ConRead_);
 *          
 *           读：sem_init(&conRead_,0,0);
 *               sem_wait(&conRead_);
 *               *Data = arr[PosRead_];
 *               sem_post(&ProWrite_);
 *                   
 *           互斥
 *              sem_init(&lock,0,1);
 * */
#define SIZE 1
#define THREADCOUNT 1
class RingQueue{
	public:
		RingQueue()
	     	: Vec_(SIZE)
		{
			Capacity_ = SIZE;
			PosWrite_ = 0;
			PosRead_ = 0;
         //同步：
		 //初始化策略：信号量计数器和数组空间的空间空间一样大
			sem_init(&ProSem_,0,SIZE);
         //初始化策略是:在初始化的时候，由于数组没有一个有效的元素，所有
		 //的初始化资源数为0，后边生产线程在唤醒消费现成的时候，会对消费者
		 //信号量当中的计数器进行++操作，从而消费者线程可以获取到消费信号量，进而去访问数组
            sem_init(&ConSem_,0,0);
  
		  //互斥  初始化资源数为1，及时只有一个线程在同一时刻能够拥有的信号量
			sem_init(&lock_,0,1);
		}

		~RingQueue(){
		   sem_destroy(&ProSem_);
		   sem_destroy(&ConSem_);
		   sem_destroy(&lock_);
		}

		void Push(int& Data){
		   sem_wait(&ProSem_);
		   sem_wait(&lock_);
		   Vec_[PosWrite_] = Data;
		   PosWrite_ = (PosWrite_+1)%Capacity_;
           sem_post(&lock_);
		   sem_post(&ConSem_);
		}

		void Pop(int* Data){
		 sem_wait(&ConSem_);
		 sem_wait(&lock_);
		 *Data = Vec_[PosRead_];
		 PosRead_ = (PosRead_+1)%Capacity_;
		 sem_wait(&lock_);
		 sem_wait(&ProSem_);

		}
	
	private:
      vector<int> Vec_;
	  size_t Capacity_;
	  
	//读写位置
	  int PosWrite_;
	  int PosRead_;
    //同步
	  sem_t ProSem_;
	  sem_t ConSem_;
    //互斥
	  sem_t lock_;
};


void* ConsumeStart(void* arg){
    RingQueue* rq = (RingQueue*)arg;
	int Data;
	while(1){
     rq->Pop(&Data);
     printf("ConsumeStart[%p][%d]\n",pthread_self(),Data);
    }
     return NULL;
 }
	 
  void* ProductStart(void* arg){
  RingQueue* rq = (RingQueue*)arg;
  int i = 0;
  while(1){
	rq->Push(i);
    printf("ProductStart[%p][%d]\n",pthread_self(),i);
	++i;
  }
   return NULL;
 }
int  main(){
   RingQueue* rq = new RingQueue();//创建一个线程安全队列对象
   pthread_t com_tid[THREADCOUNT],pro_tid[THREADCOUNT];
  int i = 0,ret = 0;
  for(;i<THREADCOUNT;++i){
	 ret =  pthread_create(&com_tid[i],NULL,ConsumeStart,(void*)rq);
	 if(ret<0){
	   printf("create thread failed\n");
	   return 0;
     }

   ret =  pthread_create(&pro_tid[i],NULL,ProductStart,(void*)rq);
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
  delete rq;
  rq = NULL;
  return 0;
}
	

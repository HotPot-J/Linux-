#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<queue>
using namespace std;
/*
 * 线程池= 线程安全队列+一堆线程
 * 线程安全队列： 元素 = 数据+处理数据的函数地址
 * */

//定义一个函数指针
typedef void* (*Handler_t)(int); //返回值内心为void  参数为int
#define  THREADCOUNT 4
class ThreadTask
{
	public:
		ThreadTask(int data,Handler_t handler)
		{
			data_ = data;
			handler_ = handler;
		}

		~ThreadTask()
		{
		
		}
       
		void Run()
		{
		 
			handler_(data_);
		}
	private:
		int data_;
		Handler_t handler_;
};

class ThreadPool
{
    public:
		ThreadPool()
		{
			capacity_ = 10;
			Thread_capacity_ = THREADCOUNT;

			pthread_mutex_init(&lock_,NULL);
			pthread_cond_init(&cond_,NULL);

			//创建线程
			bool is_create = ThreadCreate();
			if(!is_create)
			{   
				printf("Threadpool Create thread failed\n");
			
			}

		}

		~ThreadPool()
		{
		 
		}

		bool Push(ThreadTask* t)
		{
		  //生产者调用
		  pthread_mutex_lock(&lock_);
		  que_.push(t);
		  pthread_mutex_unlock(&lock_);
		  //当插入数据之后 通知线程池当中的线程(消费者)
		  pthread_cond_signal(&cond_);
		  return true;
		}
			
		bool Pop(ThreadTask** tt)
		{
		  //线程池调用
		  *tt = que_.front();
		  que_.pop();
		  return true;
		}
		
		void ThreadJoin(){
		  for(int i = 0;i<THREADCOUNT;++i){
		     pthread_join(tid[i],NULL);
		  }
		}

		/*
		 *如果直接退出线程，则有可能队列当中还有数据没有退出完毕
		 我们不该调用这样的接口去结束我们的线程
		 * */
		void ThreadExit(){
		  for(int i = 0;i<THREADCOUNT;++i){
		    pthread_cancel(tid[i]);
		  }
		}
	private:
	static void* ThreadStart(void* arg) 
		{
		     ThreadPool* tp = (ThreadPool*)arg;
			 while(1){
			   //从队列当中获取数据 进行消费 对于不同的线程 这里可以保证
			   //在获取数据的时候是互斥的
			   pthread_mutex_lock(&tp->lock_);
			   ThreadTask* tt;
			   while(tp->que_.empty()){
					//当队列为空 调用条件变量等待接口
					pthread_cond_wait(&tp->cond_,&tp->lock_);
			   }
			   tp->Pop(&tt);
			   pthread_mutex_unlock(&tp->lock_);
			   //调用队列当中元素提供的函数，去处理数据 对于不同的线程
			   //走到该位置就可以并行处理
			   tt->Run();
			   delete tt;
			 }
		}
		bool ThreadCreate()
		{
		  for(int i = 0;i<THREADCOUNT;++i)
		  {
		  int ret = pthread_create(&tid[i],NULL,ThreadStart,(void*)this);
		   if(ret!=0)
		   {
		    perror("pthread_create");
			return false;
		   }
		  }
			return true;
		}
	private:
		queue<ThreadTask*> que_;
		size_t capacity_;
		//互斥
		pthread_mutex_t lock_;
		//同步  消费线程的条件变量，但是并没有生产线程的条件变量
		//由于，客户端的请求是我们无法控制的。所以就不需要通知生产者
		//来进行生产，当生产线程有了数据，就直接往线程池当中抛入即可，
		//然后再通知消费者来进行消费
		pthread_cond_t cond_;

		//线程池当中的初始化的时候 线程数量
		size_t Thread_capacity_;

		//保存线程池当中的线程的线程标识符
		pthread_t tid[THREADCOUNT];
};

void* DealData(int data){
  printf("consume data is:%d\n ",data);
  return NULL;
}
int main(){
 ThreadPool* tp = new ThreadPool();
 //在这个代码中main函数的线程，就充当生产线程，往线程池的线程安全队列当中push数据
 for(int i = 1;i<= 50;++i){
     ThreadTask* tt = new ThreadTask(i,DealData);
	 tp->Push(tt);
 }
 //等待线程池当中线程退出
 tp->ThreadJoin();
 delete tp;
 return 0;
}

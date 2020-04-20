#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

//利用同步机制 实现一个顾客和厨师 吃面/做面的多线程代码
/*
 * 1.整个的逻辑是 临界资源noodle由生产线程生产 由消费线程消费
 * 2.当noodle被生产后 生产线程进入其PCB等待队列 去消费者PCB等待队列中唤醒消费者线程
 * 3.当noodle被消费后 消费线程进入其PCB等待队列 去生产者PCB等待队列中唤醒生产者线程
 * 生产者一个PCB等待队列，消费者一个PCB等待队列这样的逻辑规避掉了：如果只有一个PCB等待队列，当消费者消费完成后，通知PCB等待队列的时候，唤醒的同样也是一个消费者，导致唤醒线程和被唤醒线程都卡死在pthread_cond_wait，同时这里面的生产者线程永远收不到消费线程的唤醒了。进而，生产者线程和消费者线程都卡死在了pthread_cond_wait这个接口当中。
 * 4.从PCB等待队列当中唤醒的线程，需要*循环判断*当前资源的适量是否可用（程序员自己定义的）
 * */
//定义一个宏 代表一类线程个数
#define THREADCOUNT 2
//定义互斥锁
pthread_mutex_t lock;
//定义条件变量
pthread_cond_t customer_cond;
pthread_cond_t chef_cond;
//定义面条 0为无面 1为有面
int noodle = 0;

void* customer(void* arg){
  (void)arg;
  while(1){
    //永远都在进行吃面
	
	//加锁
	pthread_mutex_lock(&lock);
    while(noodle==0){
	  //没面 将该线程放入PCB等待队列中
	  pthread_cond_wait(&customer_cond,&lock);
	  //唤醒以后循环判断其是否有面 若没有继续进PCB等待队列
	}
	--noodle;
	printf("I  eat %d!\n",noodle);
	//解锁
	pthread_mutex_unlock(&lock);
	//唤醒厨师
	pthread_cond_signal(&chef_cond);
  }
  return NULL;
}

void* chef(void* arg){
  (void)arg;
  while(1){
    //永远都在进行做面
	
	//加锁
	pthread_mutex_lock(&lock);
	while(noodle==1){
	  //有面 将该线程放入PCB等待队列中
	  pthread_cond_wait(&chef_cond,&lock);
	}
	++noodle;
	printf("I made %d!\n",noodle);
	//解锁
	pthread_mutex_unlock(&lock);
	//唤醒顾客
	pthread_cond_signal(&customer_cond);
  }
  return NULL;
}

int main(){
 //分别初始化互斥锁和条件变量
 pthread_mutex_init(&lock,NULL);
 pthread_cond_init(&customer_cond,NULL);
 pthread_cond_init(&chef_cond,NULL);
 int i = 0;
 pthread_t customer_tid[THREADCOUNT];
 pthread_t chef_tid[THREADCOUNT];
 //pthread_t t[2];//定义两个线程标识符
 //分别创建一个顾客线程和厨师线程
 int ret = 0;
 for(i = 0;i<THREADCOUNT;++i){
 ret = pthread_create(&customer_tid[i],NULL,customer,NULL);
 if(ret!=0){
   perror("pthread_create\n");
   return 0;
 }
}
 for(i = 0;i<THREADCOUNT;++i){
 ret = pthread_create(&chef_tid[i],NULL,chef,NULL);
 if(ret!=0){
   perror("pthread_create\n");
   return 0;
 }
}

 //线程分离 改变其joinable属性
 for(i = 0;i<THREADCOUNT;++i){
   pthread_join(customer_tid[i],NULL);
   pthread_join(chef_tid[i],NULL);
 }
 //销毁互斥锁及条件变量
 pthread_mutex_destroy(&lock);
 pthread_cond_destroy(&customer_cond);
 pthread_cond_destroy(&customer_cond);
return 0;

} 

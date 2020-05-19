#include"tcpsvr.hpp"
#include<pthread.h>

void* ThreadStart(void* arg){
  pthread_detach(pthread_self());//线程分离（为了不需要主线程进行线程等待）
  TcpSvr* ts = (TcpSvr*)arg;
  while(1){
    //与客户端沟通
		   std::string buf;
		   ts->Recv(&buf);
		   printf("Client say: %s\n",buf.c_str());

		   printf("Please return to client  msg:");
		   fflush(stdout);
		   std::cin>>buf;
		   ts->Send(buf);
  }
  ts->Close();
  delete ts;
}
int main(int argc,char* argv[]){
	
	if(argc!=3){
	printf("Start Server: ./svr [ip] [port]\n");
	return 0;
	}
	
	std::string ip = argv[1];
	//atoi函数在stdlib.h中 作用是将字符串转换为整型数
	uint16_t port = atoi(argv[2]);

	TcpSvr ts;
	if(!ts.CreatSocket()){
		return 0;
	}

	if(!ts.Bind(ip,port)){
		return 0 ;
	}

	if(!ts.Listen()){
		return 0;
	}

	while(1){
	TcpSvr* ser_ts = new TcpSvr(); //(这里与之前不同的是在堆上开辟的内存
	//由于需要将该实例化指针传递给多线程的入口函数，若传临时变量出了作用域会失效的)
	struct  sockaddr_in clientaddr;
	if(!ts.Accept(&clientaddr,ser_ts)){
		continue;//与之前直接return的逻辑不同，continue是为了让其他想要
		          //连接服务器的客户端可以完成请求
	}
		printf("Have a new connect ip = %s,port = %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		//inet_ntoa()让无符号int变为点分十进制和主机字节序
		
		//此时拥有了新的套接字，创建工作线程 让其完成与客户端的交互
            pthread_t tid;
			int ret = pthread_create(&tid,NULL,ThreadStart,(void*)ser_ts);
			if(ret<0){
				perror("create");
				return 0;
			}
	}
	return 0; 
}


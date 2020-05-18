#include"tcpsvr.hpp"
#include<signal.h>
#include<sys/wait.h>

void sigcb(int signo){
	(void)signo;
	while(1){
		waitpid(-1,NULL,WNOHANG);
	}
}
int main(int argc,char* argv[]){
	
	if(argc!=3){
	printf("Start Server: ./svr [ip] [port]\n");
	return 0;
	}
	
	signal(SIGCHLD,sigcb);
	/*
	 *为了防止子进程变为僵尸进程，父进程需要等待子进程结束，而这种等
	 * 待分为阻塞和非阻塞。
	 * 阻塞：会导致父进程无法完成自己的工作（建立新的连接）
	 * 非阻塞：使用信号来解决，父进程在处理自己工作的同时还会时不时的轮询
	 *
	 * 原理: 子进程在终止时，会给父进程发SIGCHLD信号，该信号的默认值处理
	 * 动作为忽略，父进程可以自定义SIGCHLD信号处理函数，这样父进程只需要
	 * 专心处理自己的工作，不必关心子进程，子进程终止时会通知父进程，父进
	 * 程在信号处理函数中调用wait清理子进程即可。（本代码信号处理函数为
	 * sigcb）；
	 * */
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
	TcpSvr ser_ts; //该对象是为accept函数接收回来的新创建的socket准备的
	struct  sockaddr_in clientaddr;
	if(!ts.Accept(&clientaddr,&ser_ts)){
		continue;//与之前直接return的逻辑不同，continue是为了让其他想要
		          //连接服务器的客户端可以完成请求
	}
		printf("Have a new connect ip = %s,port = %d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		//inet_ntoa()让无符号int变为点分十进制和主机字节序
		
		//此时拥有了新的套接字，fork子进程，让其完成与客户端的交互
		int pid = fork();
		if(pid<0){
			//失败
			perror("fork");
			exit(1);//异常退出
		}
		else if(pid == 0){
		  //子进程
		  while(1){
		   std::string buf;
		   ser_ts.Recv(&buf);
		   printf("Client say: %s\n",buf.c_str());

		   printf("Please return to client  msg:");
		   fflush(stdout);
		   std::cin>>buf;
		   ser_ts.Send(buf);
			
		  }
		  ser_ts.Close();
		  exit(1);
		}
		else{
		   //父进程
		   /*
			*已经用信号解决了等待的问题，此时父进程只需在父进程中关闭掉
			*新创建出来的套接字即可，而该新套接字已经拷贝到子进程中了
			*/
		   ser_ts.Close();
		}
	}
	return 0; 
}

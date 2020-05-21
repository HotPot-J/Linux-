#pragma once
#include<cstdio>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string>
#include<iostream>
#include<cstdlib>
class TcpSvr
{
	public:
	TcpSvr(){
		sockfd_ = -1;
	}

	~TcpSvr(){

	}
	//创建套接字
	bool CreatSocket(){
		sockfd_ = socket(AF_INET,SOCK_STREAM,6);
		if(sockfd_<0){
			perror("creatSocket");
			return false;
		}
		return true;
	}
	//绑定地址信息
	bool Bind(std::string& ip,uint16_t port){
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		int flag = bind(sockfd_,(struct sockaddr*)&addr,sizeof(addr));
		if(flag<0){
			perror("bind");
			return false;
		}
		return true;
	}
	//侦听
	bool Listen(int backlog = 5){
		int	flag = listen(sockfd_,backlog);
		if(flag<0){
			perror("listen");
			return false;
		}
		return true;
	}
	//获取连接
	//peeraddr:出参，返回客户端的地址信息
	//ts:出参，返回一个TcpSvr类的实例化指针，在这个类的实例化指针当中保
	//存新创建出来的套接字描述符，上层调用者可以使用返回的类的实例化指针
	//和客户端通信
	bool Accept(struct sockaddr_in* peeraddr,TcpSvr* ts){
       socklen_t addrlen = sizeof(struct sockaddr_in);
      int serverfd =  accept(sockfd_,(struct sockaddr*)peeraddr,&addrlen);
	 if(serverfd<0){
		perror("accepet");
		return false;
	 }
	  ts->sockfd_ = serverfd;
	  return true;
	}
	//发起连接（client）
	bool Connect(std::string& ip,uint16_t port){
		struct sockaddr_in destaddr;
		destaddr.sin_family = AF_INET;
		destaddr.sin_port = htons(port);
		destaddr.sin_addr.s_addr = inet_addr(ip.c_str());
	  int flag = connect(sockfd_,(struct sockaddr*)&destaddr,sizeof(destaddr));
		if(flag<0){
			perror("connect");
			return false;
		}
		return true;
	}
	//发送数据
	bool Send(std::string& data){
		int flag = send(sockfd_,data.c_str(),data.size(),0);
		if(flag<0){
			perror("send");
			return false;
		}
		return true;
	}
	//接收数据
	//data是一个出参，将接收到的数据返回给调用者
	bool Recv(std::string* data){
		char buf[1024] = {0};
		int recvsize = recv(sockfd_,buf,sizeof(buf)-1,0);
		if(recvsize<0){
			perror("recv");
			return false;
		}
		else if(recvsize == 0){
			printf("peer shutdown connect\n");
			return false;
		}
		(*data).assign(buf,recvsize);
		return true;
	}
	//关闭套接字
	void Close(){
		close(sockfd_);
		sockfd_ = -1;
	}

	private:
	int sockfd_;
};

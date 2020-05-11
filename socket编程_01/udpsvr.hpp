/*
 *实现UDP通信小demo
	服务器：创建套接字，绑定地址信息，接收数据，发送数据，关闭套接字
	客户端：创建套接字，发送数据，接收数据，关闭套接字

	共同点：创建套接字，绑定地址信息(server)，接收数据，发送数据，关闭套接
	字。

	为了方便和可读性，将这些共同点封装成一个接口，对于客户端或服务器程序而
	言只需要调用封装的接口即可。
 * */

#pragma once
#include<cstdio>
#include<iostream>
#include<sys/socket.h>//套接字编程相关头文件
#include<netinet/in.h>//协议相关
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<cstdlib>
class UdpSvr
{
 public:
	UdpSvr()
	{
		sock_ = -1;
	}

	~UdpSvr()
	{
	
	}
	//1.Socket
	bool Socket(){
	  sock_  = socket(AF_INET,SOCK_DGRAM,17);
	  if(sock_<0){
	   //创建失败
	   perror("Socket");
	   return false;
	  }
	  return true;
	}
	//2.Bind
	bool Bind(std::string& IP,uint16_t port){
	  struct sockaddr_in addr;
	   addr.sin_family = AF_INET;
	   //inet_addr() : 将点分十进制转换成uint32
	   addr.sin_addr.s_addr = inet_addr(IP.c_str());
	   //将端口号由主机字节序转换为网络字节序
	   addr.sin_port = htons(port);
	   int flag = bind(sock_,(struct sockaddr*)&addr,sizeof(addr));
	   if(flag<0){
	    perror("Bind");
		return false;
	   }
	   return true;
	}
	//3.Send
	bool Send(std::string& data,struct sockaddr_in* destaddr){
		int sendsize = sendto(sock_,data.c_str(),data.size(),0,(struct sockaddr*)destaddr,sizeof(struct sockaddr_in));
		if(sendsize<0){
		 //发送失败
		 perror("Send");
		 return false;
		}
		return true;
	}
	//4.Recv
	/*
	 *buf:出参，返回给调用者接收到了什么样的参数
	 srcadd：这条数据从哪来的，地址信息是数据源端主机的地址信息
	 * */
	bool Recv(std::string* buf,struct sockaddr_in* srcaddr){
	   char tmp[1024];
	   memset(tmp,'\0',sizeof(tmp));
	   socklen_t socklen = sizeof(struct sockaddr_in);
	   //如果接收缓冲区当中没有数据，且flags为0，则recvfrom会阻塞等待
	   int recvsize = recvfrom(sock_,tmp,sizeof(tmp)-1,0,(struct sockaddr*)srcaddr,&socklen);
	   if(recvsize<0){
			perror("Recv");
			return false;
	   }
	   (*buf).assign(tmp,recvsize);
	   return true;
	}
	//5.Close
	void Close(){
		close(sock_);
		sock_ = -1;
	}
 private:
	int sock_;
};

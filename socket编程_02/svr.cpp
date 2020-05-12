#include<cstdio>
#include<unistd.h>
#include<iostream>
#include<sys/socket.h>//提供socket函数及数据结构
#include<netinet/in.h>//定义数据结构sockaddr_in 
#include<arpa/inet.h>//提供IP地址转换函数

int main(){
	//创建套接字
	int sockfd = socket(AF_INET,SOCK_STREAM,6);
	if(sockfd<0){
		perror("sockfd");
		return 0;
	}
	printf("sockfd = %d\n",sockfd);
	//绑定地址信息
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(19999);
	addr.sin_addr.s_addr = inet_addr("192.168.211.129");
	int flag = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	if(flag<0){
		perror("bind");
		return 0;
	}
	//监听
	//已完成连接队列大小可以认为是同一时间最大接收连接的数量
	flag = listen(sockfd,5);
	if(flag<0){
		perror("listen");
		return 0;
	}
	//获取连接
	int serverfd = accept(sockfd,NULL,NULL); //获取新创建出来的套接字描述符
	printf("serverfd = %d\n",serverfd);
	//收发数据
	
	//关闭套接字

	return 0;
}

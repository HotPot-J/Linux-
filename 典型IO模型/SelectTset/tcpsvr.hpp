#pragma once
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
using namespace std;
namespace jxy{
class TcpSvr{
  public:
  //构造函数  
  TcpSvr(){
    _socket = -1;
  }
      
  //析构
  ~TcpSvr(){}
  //创建套接字
  bool CreateSocket(){
    _socket = socket(AF_INET,SOCK_STREAM,6);
    if(_socket<0){
      perror("CreateSocket\n");
      return false;
    }
    return true;
  } 
  //绑定地址信息
  bool Bind(const string& ip,uint16_t port){
    struct sockaddr_in addr;
    addr.sin_family  = AF_INET;
    addr.sin_port = htons(port); //将主机字节序转换为网络字节序
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    int ret = bind(_socket,(struct sockaddr*)&addr,sizeof(addr));
    if(ret<0){
      perror("Bind\n");
      return false;
    }
    return true;
  }
    
  //侦听
  bool Listen(int backlog = 5){
      int ret = listen(_socket,backlog);
      if(ret<0){
        perror("Listen\n");
        return false;
      }
      return true;
  }
  //获取新连接
  bool Accept(TcpSvr& ts,struct sockaddr_in* addr){
      socklen_t addrlen = sizeof(struct sockaddr_in);
       int newSocket = accept(_socket,(struct sockaddr*)addr,&addrlen);
       if(newSocket<0){
        perror("Accept\n");
        return false;
      }
      ts._socket = newSocket;
      return true;
  }
  
  //请求连接
  bool Connect(string& ip,uint16_t port){
    struct sockaddr_in addr;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    int ret = connect(_socket,(struct sockaddr*)&addr,sizeof(addr));
    if(ret<0){
      perror("Connect\n");
      return false;
    }
    return true;
  }
  
  //接收数据
  bool Recv(string& res){
    char buf[1024] = {0};
    int dataSize = recv(_socket,buf,sizeof(buf)-1,0); //0代表阻塞接收
    if(dataSize<0){
      perror("Recv\n");
      return false;
    }
    else if(dataSize==0){
      perror("peer shutdown connect\n");
      return false;
    }
    res.assign(buf,dataSize);
    return true;
  }

  //发送数据
  bool Send(string& datas){
    int ret = send(_socket,datas.c_str(),datas.size(),0);
    if(ret<0){
      perror("send\n");
      return false;
    }
    return true;
  }
  //关闭套接字
  void Close(){
    close(_socket);
    _socket = -1;
  }
  //获取套接字
  int GetSocket(){
    return _socket;
  }
  void SetSocket(int fd){
    _socket = fd;
  }
  private:
    int _socket;
};
}

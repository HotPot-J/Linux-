/*
 *1.epoll的封装：
  （1）创建epoll操作句柄首
       epoll_create
  （2）添加对应事件结构到epoll中去
       epoll_ctl
  （3）从epoll当中删除对应的事件结构
       epoll_ctl
  （4）监控
       epoll_wait
      
      成员变量int epollfd //操作句柄
 * */

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<sys/epoll.h>
#include<vector>
#include"tcpsvr.hpp"
using namespace std;
using namespace jxy;

class EpollSvr{
public:
  EpollSvr()
    :epoll_fd(-1)
  {}
  //初始化操作句柄
  bool EpollInit(){
    epoll_fd = epoll_create(10);
    if(epoll_fd<0){
      perror("epollInit\n");
      return false;
    }
    return true;
  }

 //添加事件
  bool AddEvent(int _fd){
      //构造一个epoll_event结构体
      struct epoll_event ee;
      ee.events = EPOLLIN;
      ee.data.fd = _fd;
      int ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,_fd,&ee);
      if(ret<0){
        perror("AddEvent\n");
        return false;
      }
      return true;
  }

 //删除事件
  bool DeleteEvent(int _fd){
    int ret = epoll_ctl(epoll_fd,EPOLL_CTL_DEL,_fd,NULL);
    if(ret<0){
      perror("DeleteEvent\n");
      return false;
    }
    return true;
  }

  //监听
  bool Wait(vector<TcpSvr>& arr){
      struct epoll_event ee[10];
      int ret = epoll_wait(epoll_fd,ee,sizeof(ee)/sizeof(ee[0]),3000);
      if(ret<0){
          perror("Wait\n");
          return false;
      }
      else if(ret==0){
        perror("timeOut\n");
        return false;
      }
      if(ret>10){
        //防止返回事件多余10个会访问越界 则最多只能访问10个
        ret=10;
      }
      for(int i = 0;i<ret;++i){
        TcpSvr ts;
        ts.SetSocket(ee[i].data.fd);
        arr.push_back(ts);
      }
      return true;
  }

  ~EpollSvr(){}

private:
  //一个eventepoll操作句柄
  int epoll_fd;
};

#pragma once
#include"tcpsvr.hpp"
#include<sys/select.h>
#include<vector>
using namespace std;
using namespace jxy;
class Select{
  public:
    Select(){
      _max_fd = -1;
      FD_ZERO(&readfds);//初始化 先清空事件 （底层为位图）
    }
    //添加事件
    void Set(int fd){
        FD_SET(fd,&readfds);
        if(fd>_max_fd){
          _max_fd = fd;
        }
    }
    //删除事件
    void Delete(int fd){
      //删除
      FD_CLR(fd,&readfds);
      //更新最大描述符
      for(int i = _max_fd;i>=0;--i){
        if(FD_ISSET(i, &readfds)==0){
          //不在，则最大描述符可以进行缩小，否则就直接不用更i新
          continue;
        }
        _max_fd = i;
        break;
      }
    }

    //select监控集合中的事件
    bool SelectFd(vector<TcpSvr>& arr){
      //设置超时时间
      struct timeval s;
      s.tv_sec = 0;
      s.tv_usec = 3000;
      fd_set tmp = readfds; 
      //由于select函数最终返回时，会将未就绪事件从位图中去掉
      //所以将原事件集合保留下来
      int ret = select(_max_fd+1,&tmp,NULL,NULL,&s);
      if(ret<0){
        perror("select error\n");
        return false;
      }
      else if(ret==0){
      //perror("select timeout\n");
        return false;
      }
      //成功返回事件
      for(int i = 0;i<=_max_fd;++i){
        if(FD_ISSET(i,&tmp)){
           //返回就绪文件描述符
           //直接返回一个TcpSvr对象数组，将这些对象的套接字进行修改，
           //那么在上层调用就直接可以使用这样的对象进行操作（侦听或处理新连接）
          TcpSvr ts;
           ts.SetSocket(i);
           arr.push_back(ts);
        } 
      }
      return true;
    }
  private:
    int _max_fd;//最大监控描述符
    fd_set readfds;//监控可读事件集合
};

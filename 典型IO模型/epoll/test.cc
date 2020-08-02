#include"epoll.hpp"
#define CHECK(P) if(P!=true){return -1;}

int main(){
  TcpSvr ts;
  //创建套接字
  CHECK(ts.CreateSocket());
  //绑定地址信息
  CHECK(ts.Bind("192.168.211.129",8080));
  //侦听
  CHECK(ts.Listen());
  
  //创建eventepoll
  EpollSvr es;
  CHECK(es.EpollInit());
  //添加侦听套接字到事件中
  CHECK(es.AddEvent(ts.GetSocket()));
  
  //侦听
  while(1){
    vector<TcpSvr> arr;
    bool ret = es.Wait(arr);
    if(ret==false){
      //没有事件准备就绪
      continue;
    }
    //有事件准备就绪 判断是侦听套接字还是已建立连接发送来消息了
    int size = arr.size();
    for(int i = 0;i<size;++i){
      if(arr[i].GetSocket()==ts.GetSocket()){
        //有新连接到来
        //接受连接请求
        struct sockaddr_in addr;
        TcpSvr newfd;
        printf("Have a new connection:[ip]:%s port:%d\n",inet_ntoa(addr.sin_addr),htons(addr.sin_port));
        ts.Accept(newfd,&addr);
        //将其添加到红黑树当中
        es.AddEvent(newfd.GetSocket());
      }
      else{
        //已建立连接接收消息
        //让其与新连接通信
        string buf;
        bool ret = arr[i].Recv(buf);
        //该连接被关闭 则结束通信 从红黑树中删除该事件结构
        if(ret==false){
        es.DeleteEvent(arr[i].GetSocket());
        arr[i].Close();
             
        }
       cout<<buf<<endl;
    }
  }
}
  return 0;

}

#include"select.hpp"
#define CHECK_RET(a) if(a!=true){return -1;}
int main(){
  //创建一个，TCP服务器对象及侦听套接字
  TcpSvr TS;
 CHECK_RET(TS.CreateSocket());
 //绑定地址信息
 CHECK_RET(TS.Bind("0.0.0.0",8080));
 //开始侦听
 CHECK_RET(TS.Listen());

 Select S;
 //将侦听套接字放入监控事件中
 S.Set(TS.GetSocket());
 while(1){
  vector<TcpSvr> arr;
  if(!S.SelectFd(arr)){
    //如果未发生事件 则结束本次循环 继续监控当前事件集
    continue;
  }
  //有事件发生
  int len = arr.size();
  for(int i = 0;i<len;++i){
    if(arr[i].GetSocket()==TS.GetSocket()){
      //侦听套接字就有事件发生 则建立连接
      struct sockaddr_in addr;
      TcpSvr newTs;
      TS.Accept(newTs,&addr);
      printf("Have a new connection:[ip]:%s port:%d",inet_ntoa(addr.sin_addr),htons(addr.sin_port));
      //将新建立连接套接字放入所监控的事件集中
      S.Set(newTs.GetSocket());
    }
    else{
      //已建立连接有信息发送
      string massage;
     bool ret =  arr[i].Recv(massage);
     if(ret==false){
       //接收失败 或者连接断开
       //从监听事件中拿出当前套接字
       S.Delete(arr[i].GetSocket());
      //关闭套接字
       arr[i].Close();
     }
      cout<<massage<<endl;

    }
  }

 }

  return 0;
}
  

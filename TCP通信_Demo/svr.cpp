#include"tcpsvr.hpp"

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

	TcpSvr ser_ts; 
	struct  sockaddr_in clientaddr;
	if(!ts.Accept(&clientaddr,&ser_ts)){
		return 0;
	}
	while(1){
		std::string buf;
		ser_ts.Recv(&buf);
		printf("Client say: %s\n",buf.c_str());

		printf("Please return to client  msg:");
		fflush(stdout);
		std::cin>>buf;
		ser_ts.Send(buf);
	}
	ser_ts.Close();//关闭新创建出来的套接字
	ts.Close();//关闭了侦听套接字
	printf("1");
	return 0; 
}

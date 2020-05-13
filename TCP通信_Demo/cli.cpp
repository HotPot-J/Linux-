#include"tcpsvr.hpp"

int main(int argc,char* argv[]){
	if(argc!=3){
		printf("please ./ [ip] [port]\n");
		return 0;
	}
	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);
	TcpSvr ts;

	//创建套接字
	if(!ts.CreatSocket()){
		return 0;
	}
	//发起连接
	if(!ts.Connect(ip,port)){
		return 0;
	}

	while(1){
		printf("Please enter msg :");
		fflush(stdout);
		std::string buf;
		std::cin>>buf;
		ts.Send(buf);

		ts.Recv(&buf);
		printf("Ser say:%s\n",buf.c_str());
	}
	//关闭套接字
	ts.Close();
	printf("1");
	return 0;
}

#include "udpsvr.hpp"
//对于客户端程序而言，命令行参数当中的ip和port需要指定为服务端的ip和端口
int main(int argc,char* argv[]){
	if(argc!=3){
		printf("./cli[svr_ip][svr_port]\n");
		return 0;
	}
	std::string svr_ip = argv[1];
	uint16_t svr_port = atoi(argv[2]);
	UdpSvr us;
	if(!us.Socket()){
		return 0;
	}
		//组织服务端的地址信息 以便sendto使用
		struct sockaddr_in destaddr;
		destaddr.sin_family = AF_INET;
		destaddr.sin_port = htons(svr_port);
		destaddr.sin_addr.s_addr = inet_addr(svr_ip.c_str());

	while(1){
		//发送数据
		printf("Client Say: ");
		fflush(stdout);
		std::string buf;
		std::cin >> buf;

		//接收数据
		us.Send(buf,&destaddr);
		struct sockaddr_in peeraddr;
		us.Recv(&buf,&peeraddr);
		printf("svr say:%s\n",buf.c_str());
	}
	us.Close();
	printf("1");
	return 0;
}

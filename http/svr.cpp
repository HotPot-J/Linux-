#include"tcpsvr.hpp"
#include<cstring>
#include<sstream>
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
		printf("chrome say: %s\n",buf.c_str());
	//正文信息
		//std::string body = "<html><h1>linux-JXY<h1></html>";
		//C风格的格式化字符串
		//响应首行\r\n
		//响应头部
		//	Content-Length：\r\n
		//	Content-Type：\r\n
		//	\r\n
		//	正文
		/*char buffer[10240] = {0};
		snprintf(buffer,sizeof(buffer)-1,"%s %s %s\r\nConten-Length：%lu\r\nContent-Type：%s\r\n\r\n","HTTP/1.1","200","ok",body.size(),"text/html");
		printf("%s\n",buffer);
		std::string header;
		header.assign(buffer,strlen(buffer));*/

	//c++风格
		std::string body = "<html><h1>linux-JXY<h1></html>";
		std::stringstream ss;
		ss<<"HTTP/jxy\r\n";
		ss<<"Content-Type: text/html\r\n";
		ss<<"Content-Length: "<<body.size()<<"\r\n";
		ss<<"\r\n";
		std::string header = ss.str();

		ser_ts.Send(header);
		ser_ts.Send(body);
	}
	ser_ts.Close();//关闭新创建出来的套接字
	ts.Close();//关闭了侦听套接字
	printf("1");
	return 0; 
}

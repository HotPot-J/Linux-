#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){
	//flags: 
	//必选项：（且三者只能选其一）
	//O_RDONLY:只读
	//O_WRONLY:只写
	//O_RDWR：可读可写
	//可选项:(可选择多个)
	//O_CREAT:如果不存在则创建文件
	//O_TRUNC:截断文件（清空）
	//O_APPEND:以追加方式打开
	
	//打开文件名为tmp_file的文件  若存在以可读可写方式打开 
	//不存在则创建，权限为664  0是代表其为8进制
	int fd = open("./tmp_file",O_RDWR | O_CREAT,0664);
	if(fd<0){
		//创建/打开失败
		perror("open");
		return 0;
	}
	printf("fd = %d\n",fd);
	int writeSize = write(fd,"jjxxyy",7);
	if(writeSize<0){
		//写失败
		perror("write");
		return 0;
	}
	printf("writeSzie = %d\n",writeSize);
      /*whence: SEEK_SET(文件指针开始) SEEK_CUR(文件指针当前位置)
	  SEEK_END(文件指针尾)*/
	lseek(fd,0,SEEK_SET); 

	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	ssize_t readSize = read(fd,buf,sizeof(buf)-1);//-1是给\0预留位置
	if(readSize<0){
		perror("read");
		return 0;
	}

	printf("readSize = %d\n",readSize);
	printf("read buf is = %s\n",buf);
	close(fd);
	return 0;
}

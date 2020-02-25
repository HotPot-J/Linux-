#include<stdio.h>

int main(){
  
 FILE *fp = fopen("tmp.txt","w+");
  if(!fp){
    printf("创建文件失败！");
    return 0;
  } 
  const char *ptr = "jxygood!";
  int size =  fwrite(ptr,1,8,fp);
  if(size==8){//代表写入成功
     printf("write size[%d]\n",size);
  }  

  //若此时直接读该文件的内容，会无法读入 因为此刻的文件流指针指向文件末尾
  //需要用fseek函数将文件流指针调整到文件起始位置进行读入
  fseek(fp,0,SEEK_SET);
  int arr[1024] = {0};
  int ret = fread(arr,1,sizeof(arr)-1,fp);
  if(ret==8){
     printf("read size[%d]\n",ret);
  }
  fclose(fp);
  return 0;
}

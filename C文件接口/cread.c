#include<stdio.h>
 

  int main(){
     
     FILE *fp = fopen("tmp.txt","r+");
     if(!fp){
       printf("打开失败！");
       return 0;
    }
    char arr[1024] = {0};
    int ret = fread(arr,1,sizeof(arr)-1,fp);
    if(ret==9){//读出来了
       printf("read size[%d]\n",ret);
    }
    
     return 0;
  }

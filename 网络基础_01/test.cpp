#include<iostream>
#include<stdio.h>
using namespace std;
  union Data
  {
    int a;
	char b;
  };
int main(){
  union Data data_;
  data_.a = 1; //0x00000001
  if(data_.b==1){
	cout<<"小端\n";
  }
  else{
   cout<<"大端\n";
  }
  return 0;
}

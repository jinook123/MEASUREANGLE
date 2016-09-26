#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
int InitSocketFd(void);
 
void error_handling(char* message){
 fputs(message,stderr);
 fputc('\n',stderr);
 exit(1);
}
 
void SendImage(char *filenm)
{
 int Socket = InitSocketFd();
 if( Socket == -1 ) {
  return ;
 }
 
 printf("get int picture size\n");
 FILE *picture;
 FILE *picture2;
 picture=fopen(filenm,"r");
 int size;
 fseek(picture,0,SEEK_END);
 size=ftell(picture);
 fseek(picture,0,SEEK_SET);
 printf("jyeon.png size: %d\n",size);
 
 printf("Sending Picture Size\n");
 write(Socket,&size,sizeof(size));
 
 printf("sending picture as byte array\n");
 char send_buffer[size];
 
 while(!feof(picture)){
  fread(send_buffer,1,sizeof(send_buffer),picture);
  write(Socket,send_buffer,sizeof(send_buffer));
  bzero(send_buffer,sizeof(send_buffer));
 }
 
 fclose(picture);
//  close(Socket);
}
int InitSocketFd(void) {
 int sock;
 struct sockaddr_in serv_addr;
 if( (sock = socket(PF_INET,SOCK_STREAM,0)) < 0 ) { 
  error_handling("socket() error");
  return -1;
 }
 
 memset(&serv_addr,0,sizeof(serv_addr));
 serv_addr.sin_family=AF_INET;
 serv_addr.sin_addr.s_addr=inet_addr("203.237.94.63");
 serv_addr.sin_port=htons(atoi("5500"));
 
 if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1) {
  error_handling("connect() error!");
  return -1;
 }
 return sock;
}
int main(void){
  /*
     if(argc!=3){
     printf("Usage:%s <IP> <port>\n",argv[0]);
     exit(1);
     }*/
 int i=0;
// for(i=0;i<2;i++){
  SendImage("leekanghee1.png");  // 8k
  SendImage("leekanghee2.png");
// }
  //SendImage("lgh.png");
  //SendImage("jyeon.png");
  return 0;
}

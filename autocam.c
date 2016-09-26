#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<errno.h>
#include<wiringPi.h>
 
int main(void)
{
FILE *picture;
int size;
int i;
sleep(5);
for(i=0;i<999999999;i++)
{
system("sudo fswebcam -r 640x360 -d /dev/video0 leekanghee1.png");
system("sudo fswebcam -r 640x360 -d /dev/video1 leekanghee2.png");
system("sudo ./home/pi/client3");
sleep(1);







 


 

}
return 0;
}

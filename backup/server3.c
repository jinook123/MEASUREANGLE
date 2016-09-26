#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define BUF_SIZE 1024
int gFileCnt = 0;

void error_handling(char* message){
	// fputs(message,stderr);
	// fputc('\n',stderr);
	fprintf(stderr, "%s", message);
	exit(1);
}

int recvn(int _sockFd) {
	FILE * image;
	int	recvSize;
	int received;

	recv(_sockFd, &recvSize, sizeof(int), 0);
	fprintf(stderr, "recvSize : %d\n", recvSize);

	char buf[recvSize];
	char *ptr = buf;
	int  left = recvSize;

	while( left > 0 ) {
		if( (received = recv(_sockFd, ptr, left, 0)) <= 0 ) {
			fprintf(stderr, "recv error : [%02d] %s\n", errno, strerror(errno));
			return -1;

		} else if( received == 0 ) {
			fprintf(stderr, "recv end\n");
			break;
		}

		left -= received;
		ptr += received;
	}
	char pathImage[1024];
	sprintf(pathImage, "/var/www/html/jiyeon/file/c1_%d.png", gFileCnt++);
	fprintf(stderr, "Open file : %s\n", pathImage);
	image = fopen(pathImage, "wb");
	fwrite(buf, 1, recvSize, image);
	fclose(image);

	fprintf(stderr, "Write file size : [%d]\n", recvSize);

	return 0;
}

int main(void){
	int serv_sock;
	int clnt_sock;
	int nSockOpt;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;

	serv_sock=socket(PF_INET,SOCK_STREAM,0);

	if(serv_sock==-1) {
		error_handling("socket() error");
		return -1;
	}

	memset(&serv_addr,0,sizeof(serv_addr));

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi("5500"));

	nSockOpt=1;
	setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&nSockOpt,sizeof(nSockOpt));

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1) {
		error_handling("bind() error");

		return -1;
	}
	if(listen(serv_sock,5)==-1) {
			error_handling("listen() error");
		return -1;
	}
	clnt_addr_size=sizeof(clnt_addr);

	while( 1 ) {
		if( (clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size)) == -1 ) {
			error_handling("accept() error");
		}

		if( recvn(clnt_sock) == -1 ) {
			error_handling("recv() error");
			break;
		}

		close(clnt_sock);
	}
	close(clnt_sock);

	return 0;
}

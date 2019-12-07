#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <math.h>
#include <unistd.h>
//https://tjcplpllog.blogspot.com/2015/12/c-http.html
//http://bbs.nicklib.com/application/1539

char * int2Char(int src){
	int lenSrc = 0;
    int i;
	//길이재기
    int cpIn = src;
	for(i = 10; cpIn > 0;){
        cpIn = (int)cpIn/i;
		lenSrc++;
	}
	//동적할ㅇ당하기
	char * out = (char *) malloc((lenSrc+1)*sizeof(char));
	int j;
	//ascii로 변환
	for(j = 0; lenSrc > j; j++ ){
		out[j] = (src/(int)pow(10, lenSrc-1-j))+48;
		src = src%((int)pow(10, lenSrc-1-j));
	}
	out[lenSrc] = NULL;
	return out;
}

int server(){

	int sock;
	int client;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(addr);
	int bind_port = 8080;
	int on = 1;
	char buffer[1024] = {0,};
	size_t read_len;
	const char * header_packet = "HTTP/1.0 200 OK\r\n Server: YourServerName\r\n Connection: close\r\n Content-Type: multipart/x-mixed-replace;boundary=myBoundary\r\n\r\n";

	const char * header_packet_1 = "HTTP/1.0 200 OK\r\n Server: YourServerName\r\n Connection: close\r\n Content-Type: image/jpeg\r\n\r\n";
	const char * mjpegHeaderPacket = "--myBoundary\r\nContent-Type: image/jpeg\r\nContent-length: 3656\r\n\r\n";

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket() error");
		return 1;
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on)) != 0) {
		perror("setsockopt() error");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bind_port);
	if (bind(sock, (struct sockaddr*)&addr, addr_len) != 0) {
		perror("bind() error");
		return 1;
	}

	if (listen(sock, 5) != 0) {
		perror("listen() error");
		return 1;
	}

	printf("listening... %d\n", bind_port);

	//jpeg 데이터
	FILE *fp = fopen("1.jpeg","rb"); 
	fseek(fp, 0, SEEK_END); 
	int size = ftell(fp);
	char * data = calloc(size+1,sizeof(char)); 
	fseek(fp, 0, SEEK_SET);
	fread(data,size,1,fp);
	while(1){
		client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
		if (client < 0) {
			perror("accept() error");
			return 1;
		}
		read_len = recv(client, buffer, sizeof(buffer), 0);
		if (read_len > 0) {
			printf("RECV: %s", buffer);
		}
		send(client, header_packet_1, strlen(header_packet), 0);
		
		//while(1){
			sleep(1);
			//printf("1\n");
			
			//send(client, mjpegHeaderPacket, strlen(mjpegHeaderPacket), 0);
			printf(mjpegHeaderPacket);
			char * dataLen = int2Char(sizeof(data));
			//printf("1\n");
			//content length
			//send(client, int2Char(size), strlen(int2Char(size)), 0);
			printf(int2Char(size));
			//printf(int2Char(size));
			//send(client, "\r\n\r\n",strlen("\r\n\r\n"),0);
			send(client, data,size,0);
			read_len = recv(client, buffer, sizeof(buffer), 0);
			//send(client, "\r\n",strlen("\r\n"),0);
			//send(client, "\r\n\r\n",strlen("\r\n\r\n"),0);
		//}
	
	}
	close(client);
	close(sock);
	
}

int main(int argc, char *args[]) {

	// int pid;
	// //fork 를 해서 자식 프로세스를 생성하고 부모프로세스는 종료됨.
	// pid = fork();
	// switch(pid){
	// case -1:
	// 	fprintf(stderr,"Fork Error \n");
	// 	break;
	// case 0 :
	// 	break;
	// default : 
	// 	return 0;
	// }
	// //chdir("/");
	// //setsid();

	server();
    return 0;
}


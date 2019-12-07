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
//https://tjcplpllog.blogspot.com/2015/12/c-http.html
//http://bbs.nicklib.com/application/1539

void int2Char(int src, char * out, int * stringSize){
	int lenSrc = 0;
    int i;
	//길이재기
    int cpIn = src;
	for(i = 10; cpIn > 0;){
        cpIn = (int)cpIn/i;
		lenSrc++;
	}
	//동적할ㅇ당하기
	out = (char *) malloc((lenSrc+1)*sizeof(char));
	int j;
	//ascii로 변환
	for(j = 0; lenSrc > j; j++ ){
		out[j] = (src/(int)pow(10, lenSrc-1-j))+48;
		src = src%((int)pow(10, lenSrc-1-j));
	}
	*stringSize = lenSrc;
	out[lenSrc] = '\0';
	return;
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
	const char header_packet[] = "HTTP/1.0 200 OK\r\nServer: YourServerName\r\n Connection: close\r\nContent-Type:multipart/x-mixed-replace; boundary=myBoundary\r\n";

	//const char * header_packet_1 = "HTTP/1.0 200 OK\r\n Server: YourServerName\r\n Connection: close\r\n Content-Type: image/jpeg\r\nContent-length: ";
	const char mjpegHeaderPacket[] = "--myBoundary\r\nContent-Type: image/jpeg\r\nContent-length: ";
	//const char * jpegSize = "3656\r\n\r\n";
	const char * end = "\r\n\r\n";

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


	client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
	if (client < 0) {
		perror("accept() error");
		return 1;
	}
	read_len = recv(client, buffer, sizeof(buffer), 0);
	if (read_len > 0) {
		printf("RECV: %s", buffer);
	}
		
	//jpeg 데이터
	
		char fileName[256];
		//char * fileSizeString;
		int fileSizeStringSize;
	int count = 1;
	send(client, header_packet, strlen(header_packet), 0);
	printf(header_packet);	
	while(1){
		sprintf(fileName, "%d.jpeg",count++);
		if (count == 3){
			count = 2;
		}
		FILE *fp = fopen(fileName,"rb"); 
		fseek(fp, 0, SEEK_END); 
		int size = ftell(fp);
		char * data = (char *) malloc((size + 1)* sizeof(char)); 
		fseek(fp, 0, SEEK_SET);
		fread(data,size,1,fp);
		//sprintf(fileSize,"%d",size);
		//printf("%d", strlen(fileSize));
		char * fileSizeString;
		int2Char(size, fileSizeString, &fileSizeStringSize);
		sleep(1);
		send(client, mjpegHeaderPacket, strlen(mjpegHeaderPacket), 0);
		printf(mjpegHeaderPacket);
		//char * dataLen = int2Char(sizeof(data));
		//printf("1\n");
		//content length
		//printf(int2Char(size));
		send(client, fileSizeString, fileSizeStringSize, 0);
		send(client, end ,strlen(end),0);
		
		printf("%s",fileSizeString);
		printf(end);
		//printf(int2Char(size));
		//send(client, "\r\n\r\n",strlen("\r\n\r\n"),0);
		//send(client, jpegSize,strlen(jpegSize),0);
		send(client, data, size,0);
		send(client, "\r\n", strlen("\r\n"),0);
		//send(client, end ,strlen(end),0);
		//printf(end);
		// read_len = recv(client, buffer, sizeof(buffer), 0);
		// if (read_len > 0) {
		// 	printf("RECV: %s", buffer);
	
		// 	//send(client, "\r\n",strlen("\r\n"),0);
		// 	//send(client, "\r\n\r\n",strlen("\r\n\r\n"),0);
		// }
		free(data);
		//free(fileSizeString);
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


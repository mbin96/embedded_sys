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
#include <signal.h>
//https://tjcplpllog.blogspot.com/2015/12/c-http.html
//http://bbs.nicklib.com/application/1539
//https://stackoverflow.com/questions/47729941/mjpeg-over-http-specification



// string for socket
const char* header_packet = "HTTP/1.0 200 OK\r\nServer: EMPOSIII\r\n Connection: close\r\nContent-Type:multipart/x-mixed-replace; boundary=myBoundary\r\n\r\n";
const char* mjpegHeaderPacket = "--myBoundary\r\nContent-Type: image/jpeg\r\n\r\n";
FILE* fp;
char* jpegData;
int on = 1;
char buffer[1024] = { 0, };
size_t read_len;
int jpegSize;
int client;
int sock;
struct sockaddr_in addr;
struct sockaddr_in client_addr;
socklen_t addr_len = sizeof(addr);

void cpJpegData(char* Data, int Size);
void openJpegData(char * fileName);
void initSocket(int port);
void createConnetion();
void closeConnection();
void closeSocket();
int sendFrame();

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


#define DEBUG


void cpJpegData(char * Data, int Size){
	memcpy(jpegData,Data,Size);
	jpegSize = Size;
}

void openJpegData(char * fileName){

    fp = fopen(fileName, "rb");
    //check file size
    fseek(fp, 0, SEEK_END);
	jpegSize =ftell(fp);
    fseek(fp, 0, SEEK_SET);

	jpegData = (char *)malloc((jpegSize+1) * sizeof(char));
    fread(jpegData,jpegSize,1,fp);
    fclose(fp);
}

void createConnetion(){

	client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
	if (client < 0) {
		perror("accept() error");
        return;
	}
	//응답 받아오기
	read_len = recv(client, buffer, sizeof(buffer), 0);
	
	#ifdef DEBUG
	if (read_len > 0) {
		printf("RECV: %s", buffer);
	}
	#endif //DEBUG
	//헤더 패킷
	send(client, header_packet, strlen(header_packet), 0);
    printf("header send\n");
}

void closeConnection(){
	close(client);
#ifdef DEBUG
	printf("closeConnection\n");
#endif //DEBUG

}
void closeSocket(){
	close(sock);
#ifdef DEBUG
	printf("closeSocket\n");
#endif //DEBUG

}

void initSocket(int port){
	//매개변수로 포트 설정
	int bind_port = port;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket() error");
        return;
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on)) != 0) {
		perror("setsockopt() error");
        return;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bind_port);
	if (bind(sock, (struct sockaddr*)&addr, addr_len) != 0) {
		perror("bind() error");
        return;
	}

	if (listen(sock, 5) != 0) {
		perror("listen() error");
        return;
	}
	printf("listening... %d\n", bind_port);
}
		
	


int sendFrame(){
	int err;

	err = send(client, mjpegHeaderPacket, strlen(mjpegHeaderPacket), MSG_NOSIGNAL);
	if (err < 0){
        return -1;
	}

	err = send(client, jpegData, jpegSize, MSG_NOSIGNAL);
	if (err < 0){
        return -1;
	}

	err = send(client, "\r\n", strlen("\r\n"), MSG_NOSIGNAL);
	if (err < 0){
        return -1;
	}
	
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
	//chdir("/");
	//setsid();
	initSocket(8080);
	createConnetion();
    int err;
	int count = 1 ;
	while(1){
		usleep(30000);// wait 30ms
        if (count == 1) {
            openJpegData("1.jpeg");
            #ifdef DEBUG
            //printf("fileopen\n");
            #endif
            count++;
        }
        else {
            openJpegData("2.jpeg");
            count = 1;
            #ifdef DEBUG
            //printf("fileopen\n");
            #endif
        }

		err = sendFrame();
		if (err == -1){
			closeConnection();
#ifdef DEBUG
            printf("connection closed puri");
#endif //DEBUG
			createConnetion();
		}
	}
	closeSocket();
	//정상적인 경우 여기로 오면 안됨.
	printf("quit mjpeg server");
    return 0;
}


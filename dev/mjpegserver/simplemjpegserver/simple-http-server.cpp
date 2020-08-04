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
#include <iostream>
#include <string>
#include <fstream>
//https://tjcplpllog.blogspot.com/2015/12/c-http.html
//http://bbs.nicklib.com/application/1539
//https://stackoverflow.com/questions/47729941/mjpeg-over-http-specification

// void int2Char(int src, char * out, int * stringSize){
// 	int lenSrc = 0;
//     int i;
// 	//길이재기
//     int cpIn = src;
// 	for(i = 10; cpIn > 0;){
//         cpIn = (int)cpIn/i;
// 		lenSrc++;
// 	}
// 	//동적할ㅇ당하기
// 	out = (char *) malloc((lenSrc+1)*sizeof(char));
// 	int j;
// 	//ascii로 변환
// 	for(j = 0; lenSrc > j; j++ ){
// 		out[j] = (src/(int)pow(10, lenSrc-1-j))+48;
// 		src = src%((int)pow(10, lenSrc-1-j));
// 	}
// 	*stringSize = lenSrc;
// 	out[lenSrc] = '\0';
// 	return;
// }

#define __THROW_SEND_ERROR 1
#define __THROW_SOCKET_ERROR 2
#define __THROW_SET_SOCK_OPT_ERROR 3
#define __THROW_BIND_ERROR 4
#define __THROW_LISTEN_ERROR 5
#define __THROW_ACCEPT_ERROR 6
#define DEBUG

class mJpegServer{
	private :
	int client;
	int sock;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(addr);
	int bind_port = 8080;
	int on = 1;
	char buffer[1024] = {0,};
	size_t read_len;
	const char * header_packet = "HTTP/1.0 200 OK\r\nServer: EMPOSIII\r\n Connection: close\r\nContent-Type:multipart/x-mixed-replace; boundary=myBoundary\r\n\r\n";
	const char * mjpegHeaderPacket = "--myBoundary\r\nContent-Type: image/jpeg\r\n\r\n";
	char * jpegData;
	int jpegSize;
	std::ifstream fileJpeg;

	public:
	void cpJpegData(char * Data, int Size);
	void openJpegData(std::string fileName);

	void initSocket(int port=8080);
	void createConnetion();
	void closeConnection();
	void closeSocket();

	void sendFrame(char * jpegData, int jpegSize);
	void sendFrame();
	
};

void mJpegServer::cpJpegData(char * Data, int Size){
	memcpy(jpegData,Data,Size);
	jpegSize = Size;
}

void mJpegServer::openJpegData(std::string fileName){
	delete[] jpegData;
	fileJpeg.close();


	fileJpeg.open(fileName, std::ios::binary);
	if (!fileJpeg.is_open()){
		throw std::__throw_ios_failure;
	}
	
	fileJpeg.seekg(0, std::ios::end);
	jpegSize = fileJpeg.tellg();
	fileJpeg.seekg(0, std::ios::beg);

	jpegData = new char[jpegSize];
	fileJpeg.read(jpegData,jpegSize);

}

void mJpegServer::createConnetion(){

	client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
	if (client < 0) {
		perror("accept() error");
		throw __THROW_ACCEPT_ERROR;
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
}

void mJpegServer::closeConnection(){
	close(client);
#ifdef DEBUG
	printf("closeSocket\n");
#endif //DEBUG

}
void mJpegServer::closeSocket(){
	close(sock);
#ifdef DEBUG
	printf("closeSocket\n");
#endif //DEBUG

}

void mJpegServer::initSocket(int port){
	//매개변수로 포트 설정
	bind_port = port;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("socket() error");
	 	throw __THROW_SOCKET_ERROR;
	}
	
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on)) != 0) {
		perror("setsockopt() error");
		throw __THROW_SET_SOCK_OPT_ERROR;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bind_port);
	if (bind(sock, (struct sockaddr*)&addr, addr_len) != 0) {
		perror("bind() error");
		throw __THROW_BIND_ERROR;
	}

	if (listen(sock, 5) != 0) {
		perror("listen() error");
		throw __THROW_LISTEN_ERROR;
	}
	printf("listening... %d\n", bind_port);
}
		
	


void mJpegServer::sendFrame(){
	int err;

	err = send(client, mjpegHeaderPacket, strlen(mjpegHeaderPacket), MSG_NOSIGNAL);
	if (err < 0){
		throw __THROW_SEND_ERROR;
	}

	err = send(client, jpegData, jpegSize, MSG_NOSIGNAL);
	if (err < 0){
		throw __THROW_SEND_ERROR;
	}

	err = send(client, "\r\n", strlen("\r\n"), MSG_NOSIGNAL);
	if (err < 0){
		throw __THROW_SEND_ERROR;
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
	mJpegServer server;
	server.initSocket(8080);
	server.createConnetion();

		int count = 1 ;
	while(1){
		usleep(30000);
		server.openJpegData(std::to_string(count)+".jpeg");
		count++;
		if(count == 3){
			count = 1;
		}
		try{
			server.sendFrame();
		}catch(int x){
			server.closeConnection();
#ifdef DEBUG
			std::cout<<"connection cut off puri"<< std::endl;
#endif //DEBUG
			server.createConnetion();
		}
	}
	server.closeSocket();
	//정상적인 경우 여기로 오면 안됨.
	printf("quit mjpeg server");
    return 0;
}


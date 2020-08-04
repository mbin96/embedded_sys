#include "mjpegServer.h"
//https://tjcplpllog.blogspot.com/2015/12/c-http.html
//http://bbs.nicklib.com/application/1539
//https://stackoverflow.com/questions/47729941/mjpeg-over-http-specification



// string for socket
const char* header_packet = "HTTP/1.0 200 OK\r\nServer: EMPOSIII\r\nConnection: close\r\nContent-Type: multipart/x-mixed-replace; boundary=myboundary\r\n\r\n";
const char* mjpegHeaderPacket = "--myboundary\r\nContent-Type: image/jpeg\r\nContent-Length: ";
FILE* fp;
//char* jpegData;
int on = 1;
char buffer[1024] = { 0, };
size_t read_len;
//int jpegSize;
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

// void int2Char(int src, char * out, int * stringSize){
// 	int lenSrc = 0;
//     int i;
// 	//湲몄씠?ш린
//     int cpIn = src;
// 	for(i = 10; cpIn > 0;){
//         cpIn = (int)cpIn/i;
// 		lenSrc++;
// 	}
// 	//?숈쟻?졼뀋?뱁븯湲?	out = (char *) malloc((lenSrc+1)*sizeof(char));
// 	int j;
// 	//ascii濡?蹂??	for(j = 0; lenSrc > j; j++ ){
// 		out[j] = (src/(int)pow(10, lenSrc-1-j))+48;
// 		src = src%((int)pow(10, lenSrc-1-j));
// 	}
// 	*stringSize = lenSrc;
// 	out[lenSrc] = '\0';
// 	return;
// }


#define DEBUG


// void cpJpegData(char * Data, int Size){
// 	memcpy(jpegData,Data,Size);
// 	jpegSize = Size;
// }

// void openJpegData(char * fileName){

//     fp = fopen(fileName, "rb");
//     //check file size
//     fseek(fp, 0, SEEK_END);
// 	jpegSize =ftell(fp);
//     fseek(fp, 0, SEEK_SET);

// 	jpegData = (char *)malloc((jpegSize+1) * sizeof(char));
//     fread(jpegData,jpegSize,1,fp);
//     fclose(fp);
// }

void createConnetion(){

	client = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
	if (client < 0) {
		perror("accept() error");
        return;
	}
	//?묐떟 諛쏆븘?ㅺ린
	read_len = recv(client, buffer, sizeof(buffer), 0);
	
	#ifdef DEBUG
	if (read_len > 0) {
		printf("RECV: %s", buffer);
	}
	#endif //DEBUG
	//?ㅻ뜑 ?⑦궥
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
	//留ㅺ컻蹂?섎줈 ?ы듃 ?ㅼ젙
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
		
	


int sendFrame(char * jpegData, int jpegSize){
	int err;

	err = send(client, mjpegHeaderPacket, strlen(mjpegHeaderPacket), MSG_NOSIGNAL);
	if (err < 0){
        return -1;
	}
    char size[20];
    sprintf(size,"%d", jpegSize);
    send(client, size, strlen(size), MSG_NOSIGNAL);
    // printf(size);
    // printf("%d",sizelen);
    send(client, "\r\n\r\n", strlen("\r\n\r\n"), MSG_NOSIGNAL);
    
	err = send(client, jpegData, jpegSize, MSG_NOSIGNAL);
	if (err < 0){
        return -1;
	}
    //send(client, "\r\n\r\n", strlen("\r\n\r\n"), MSG_NOSIGNAL);
	err = send(client, "\r\n", strlen("\r\n"), MSG_NOSIGNAL);
	if (err < 0){
        return -1;
	}

	
}

// example
// int main(int argc, char *args[]) {

// 	// int pid;
// 	// //fork 瑜??댁꽌 ?먯떇 ?꾨줈?몄뒪瑜??앹꽦?섍퀬 遺紐⑦봽濡쒖꽭?ㅻ뒗 醫낅즺??
// 	// pid = fork();
// 	// switch(pid){
// 	// case -1:
// 	// 	fprintf(stderr,"Fork Error \n");
// 	// 	break;
// 	// case 0 :
// 	// 	break;
// 	// default : 
// 	// 	return 0;
// 	// }
// 	//chdir("/");
// 	//setsid();
// 	initSocket(8080);
// 	createConnetion();
//     int err;
// 	int count = 1 ;
// 	while(1){
// 		usleep(2000000);// wait 30ms
//         if (count == 1) {
//             openJpegData("1.jpeg");
//             #ifdef DEBUG
//             //printf("fileopen\n");
//             #endif
//             count++;
//         }
//         else {
//             openJpegData("2.jpeg");
//             count = 1;
//             #ifdef DEBUG
//             //printf("fileopen\n");
//             #endif
//         }

// 		err = sendFrame();
// 		if (err == -1){
// 			closeConnection();
// #ifdef DEBUG
//             printf("connection closed puri");
// #endif //DEBUG
// 			createConnetion();
// 		}
// 	}
// 	closeSocket();
// 	//?뺤긽?곸씤 寃쎌슦 ?ш린濡??ㅻ㈃ ?덈맖.
// 	printf("quit mjpeg server");
//     return 0;
// }


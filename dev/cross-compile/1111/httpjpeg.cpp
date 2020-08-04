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

const char HEAD_RESPONSE[] =
	{
		"HTTP Code: 200 OK\r\n"
		"Content-Type: multipart/x-mixed-replace;boundary=myboundary\r\n"
		"--myboundary\r\n\r\n"};
const char HEAD_RESPONSE_PER_IMAGE[] =
	{
		"Content-Type: image/jpeg\r\n"
		"Content-Length: "};
const char SEPARATOR[] =
	{
		"\r\n"
		"--myboundary\r\n"};
char *int2Char(int src)
{
	int lenSrc = 0, i;
	for (i = 1; (src / i) > 0; i = i * 10)
		lenSrc++;
	char *out = (char *)malloc(lenSrc * sizeof(char));
	int j;
	for (j = 0; lenSrc > j; j++)
	{
		out[j] = src / pow(1, lenSrc - j);
		src = src % pow(1, lenSrc - j);
	}
	return out;
}

int main(int argc, char *args[])
{

	int sock;
	int client;
	struct sockaddr_in addr;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(addr);
	int bind_port = 8080;
	int on = 1;
	char buffer[1024] = {
		0,
	};
	size_t read_len;
	const char *response_packet = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\nContent-Type: text/plain\r\n\r\nHello";

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
	{
		perror("socket() error");
		return 1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on)) != 0)
	{
		perror("setsockopt() error");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bind_port);
	if (bind(sock, (struct sockaddr *)&addr, addr_len) != 0)
	{
		perror("bind() error");
		return 1;
	}

	if (listen(sock, 5) != 0)
	{
		perror("listen() error");
		return 1;
	}

	printf("listening... %d\n", bind_port);
	client = accept(sock, (struct sockaddr *)&client_addr, &addr_len);
	if (client < 0)
	{
		perror("accept() error");
		return 1;
	}

	read_len = recv(client, buffer, sizeof(buffer), 0);
	if (read_len > 0)
	{
		printf("RECV: %s", buffer);
	}

	//Send header to client
	//---------------------
	/*
            HTTP Code: 200 OK
            Content-Type: multipart/x-mixed-replace; boundary=myboundary
            --myboundary
            */
	/*
                Read image data, just used a single image
                to keep things simple [Binary format]
                */
	FILE *picture;
	int size, read_size, stat, packet_index;
	char send_buffer[10240], read_buffer[256];
	packet_index = 1;

	picture = fopen("capture.jpeg", "r");
	printf("Getting Picture Size\n");

	if (picture == NULL)
	{
		printf("Error Opening Image File");
	}

	fseek(picture, 0, SEEK_END);
	size = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	printf("Total Picture size: %i\n", size);
	send(client, HEAD_RESPONSE, strlen(HEAD_RESPONSE), 0);
	//send(client, response_packet, strlen(response_packet), 0);
	send(client, HEAD_RESPONSE_PER_IMAGE, strlen(HEAD_RESPONSE_PER_IMAGE), 0);
	send(client, , , 0);
	send(client, HEAD_RESPONSE_PER_IMAGE, strlen(HEAD_RESPONSE_PER_IMAGE), 0);

	close(client);
	close(sock);

	return 0;
}

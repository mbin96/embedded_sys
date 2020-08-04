#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/tracking.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/core/ocl.hpp>
#include <time.h>
// #include <winsock2.h>
#include <sys/types.h> 
// #include <WinBase.h>


#pragma comment (lib , "ws2_32.lib")
#pragma comment(lib, "vfw32.lib") 
#pragma comment( lib, "comctl32.lib" )


using namespace std;
using namespace cv;

const std::string currentDateTime() {
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��
	return buf;
}

void showError(const char * msg)
{
	std::cout << "���� : " << msg << std::endl;
	exit(-1);
}


int main() {

	VideoCapture capture;
	string videoStreamAddress;
	char Tosocket[50];
	cout << "Put Your IP Camera Address : ";
	cin >> videoStreamAddress;
	string A = "http://";
	string B = "/mjpg/video.mjpg";
	strcpy(Tosocket, videoStreamAddress.c_str());
	videoStreamAddress = A + videoStreamAddress + B;

	capture.open(videoStreamAddress);
	
	// WSADATA data;
	// ::WSAStartup(MAKEWORD(2, 2), &data);

	// SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//if (client == INVALID_SOCKET)
	//showError("Ŭ���̾�Ʈ ���� ����");

	// sockaddr_in addr = { 0 };
	// addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = inet_addr(Tosocket);
	// addr.sin_port = htons(23000);
	//if (connect(client, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	//	showError("���� ����");
	//VideoCapture capture(0);
	if (!capture.isOpened()) {
		cout << "Can not open capture!!" << endl;
		return 0;
	}
	CascadeClassifier cascade;
	vector<Rect> faces;
	cascade.load("/usr/local/share/opencv4/lbpcascades/lbpcascade_frontalface_improved.xml");
	
	vector<vector<Point> > cnts;
	Size size = Size((int)capture.get(CAP_PROP_FRAME_WIDTH),
		(int)capture.get(CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer;

	double fps = 5.0;
	int detect = 0;
	int isitfirst = 1;
	int save = 0;
	int  cnt = 0;
	int numb = 0;
	int face = 0;
	imshow("frame", NULL);
	waitKey(100);
	
	Mat frame, grayImage, grayImage2;
	Mat avgImage, diffImage, mask;
	Mat last_frame, moveImage, flow;
	Mat face_img;

	for (;;) {
		capture >> frame;
		capture >> last_frame;
		cascade.detectMultiScale(frame, faces, 1.1, 4, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
		if (frame.empty()) break;

		cvtColor(frame, grayImage, COLOR_BGR2GRAY);
		cvtColor(last_frame, grayImage2, COLOR_BGR2GRAY);
		GaussianBlur(grayImage, grayImage, Size(5, 5), 0.5);
		avgImage = grayImage2;
		absdiff(grayImage, avgImage, diffImage);
		threshold(diffImage, mask, 50, 255, THRESH_BINARY);
		dilate(mask, mask, Mat(), Point(-1, -1), 2);
		findContours(mask, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		putText(frame, currentDateTime(), Point(frame.cols - frame.cols / 3 * 2, frame.rows - frame.rows / 8), FONT_HERSHEY_SIMPLEX, 0.55, Scalar(255, 255, 255), 2);
 
		for (int i = 0; i < cnts.size(); i++) {
			if (contourArea(cnts[i]) < 500) {
				continue;
			}
			putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
			printf("Motion Detected!!\n");
			//char motionDetect[] = "Detect!";
			//send(client, motionDetect, strlen(motionDetect), 0);
			detect = 1;
			cnt = 0;
		}	

		for (int y = 0; y < faces.size(); y++)
		{
			Point lb(faces[y].x + faces[y].width, faces[y].y + faces[y].height);
			Point tr(faces[y].x, faces[y].y);
			Rect rect(faces[y].x, faces[y].y, faces[y].width, faces[y].height);
			rectangle(frame, lb, tr, Scalar(0, 255, 0), 2, 8, 0);
			face_img = frame(rect);
			face = 1;
		}

		if (isitfirst && detect) {
			char motion[100];
			sprintf(motion, "motiondetect%2d.avi", numb++);
			writer.open(motion,VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
			save = 1;
			if (!writer.isOpened())
			{
				cout << "save avi error" << endl;
				return 1;
			}
		}
		if (save){
			isitfirst = 0;
			if (cnt == 30) {
				printf("done\n");
				writer.release();
				isitfirst = 1;
			}
			writer << frame;
		}
  
		cnt++;	
		resize(frame, frame, Size(frame.cols * 3/2, frame.rows * 3/2));
		
		imshow("frame", frame);
		if (face)
		{
			resize(face_img, face_img, Size(face_img.cols * 3 / 2, face_img.rows * 3 / 2));
			imshow("face", face_img);
			face = 0;
		}
		if (waitKey(5) == 27) break;

	}

	// closesocket(client);
	// ::WSACleanup();
	return 0;
}



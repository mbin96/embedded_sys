#include "camera.h"
#include "mjpegServer.h"
#include "jpegComp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include <fcntl.h>    /* for O_RDONLY */
#include <linux/fb.h> /* for fb_var_screeninfo, FBIOGET_VSCREENINFO*/
#include <termios.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <float.h>
#include <math.h>
#include <sys/ioctl.h>
#include "text.h"
#define DEBUG
#define FBDEV_FILE "/dev/fb0" //cam

int main()
{
    int dev;
    int f_loop = 1;
    unsigned short vkey_org[2], vkey_new[2], vkey_run[2];
    unsigned char * imageBuffer = (unsigned char *)malloc(IMAGE_WIDTH*IMAGE_HEIGHT*3);
    unsigned char * jpegBuffer[1];
    unsigned char * camCloseImg[1];
    unsigned long jpegSize;
    unsigned long camCloseImgSize;
    int err;


    FILE * fp = fopen("camClose.jpeg","rb");
    fseek(fp, 0, SEEK_END);
	camCloseImgSize =ftell(fp);
    fseek(fp, 0, SEEK_SET);

	camCloseImg[0] = (char *)malloc((camCloseImgSize+1) * sizeof(char));
    fread(camCloseImg[0],camCloseImgSize,1,fp);
    fclose(fp);

    //open dip sw
    if ((dev = open("/dev/dipsw", O_RDONLY)) < 0)
    {
        perror("DIPSW open fail\n");
        return -1;
    }
    
    cameraInit();
    initSocket(8080);
    createConnetion();
    textlcdInit();
    display1();
    printf("Set dipsw 1 to run cam\nSet dipsw 9 to quit\n");
    display_set();
    while (f_loop) //dip_sw 확인
    {              //웹서버는 계속 돌게끔
        read(dev, &vkey_new[0], 4);
        printf("\t> (SW1) : %02X\n", vkey_new[0]);
        if (vkey_new[0] == 1 || vkey_new[0] == 9){
            display2();
            display_init();
            camera(imageBuffer);
            
            #ifdef DEBUG
            printf("camera");
            #endif //Debug
            jpegCompress(imageBuffer, jpegBuffer, &jpegSize);
            
            #ifdef DEBUG
            printf("compress");
            #endif //Debug
            err = sendFrame(*jpegBuffer, jpegSize);
            #ifdef DEBUG
            printf("sendFrame");
            #endif //Debug
            if (err == -1){
			    closeConnection();
                createConnetion();
            }
        }else{
            sendFrame(*camCloseImg, camCloseImgSize);
            sleep(1);
            display1();
            display_set();
        }
        
        if (vkey_new[0] == 9){            
            f_loop = 0;
            cameraClean();
            display1();

        }
        
        
    }
    close(dev); //dipsw파일을닫는다.
    closeConnection();
    closeSocket();
    free(imageBuffer);
    free(jpegBuffer[0]);
    //free(jpegBuffer);
    return 0;
}
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

void cameraInit();
void cameraClean();
void camera(char *image_buffer);
void display_set();
void display_init();
#define FBDEV_FILE "/dev/fb0"

#define CIS_IMAGE_WIDTH 320
#define CIS_IMAGE_HEIGHT 240
#define CIS_IMAGE_SIZE CIS_IMAGE_WIDTH*CIS_IMAGE_HEIGHT*2

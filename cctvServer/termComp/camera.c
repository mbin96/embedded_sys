#include "camera.h"

struct fb_var_screeninfo fbvar;
unsigned short cisdata[CIS_IMAGE_SIZE];

int isitFirst = 0;
int dev_fd, fbfd;
int screen_width;
int screen_height;
int line_length;
struct fb_var_screeninfo fbvar;
struct fb_fix_screeninfo fbfix;
void *fb_mapped;
int mem_size;

void display_init()
{
    FILE *fp = fopen("running","rb");
    unsigned short tmp[800*480];
    fread(tmp,800*480*2,1,fp);
    //unsigned short tmp2[800*480];
    unsigned short *ptr;
    int coor_x, coor_y;
    
    for (coor_y = 0; coor_y < 480; coor_y++)
    {
        ptr = (unsigned short *)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < 800; coor_x++)
        {
            *ptr++ = tmp[coor_y * 800 + coor_x];
        }
    }
}

//bg = background (800*480 16bit image)
void display_set()
{
    FILE *fp = fopen("initialize","rb");
    unsigned short tmp[800*480];
    fread(tmp,800*480*2,1,fp);
    //unsigned short tmp2[800*480];
    unsigned short *ptr;
    int coor_x, coor_y;
    for (coor_y = 0; coor_y < 480; coor_y++)
    {
        ptr = (unsigned short *)fb_mapped + screen_width * coor_y;
        for (coor_x = 0; coor_x < 800; coor_x++)
        {
            *ptr++ = tmp[coor_y * 800 + coor_x];
        }
    }
}

void cameraInit(){
    if ((dev_fd = open("/dev/camera", O_RDWR | O_SYNC)) < 0)
    {
        perror("camera open");
        exit(1);
    }
    if (access(FBDEV_FILE, F_OK)) //Is is possible to  R/W?
    {
        printf("%s: access error\n", FBDEV_FILE);
        exit(1);
    }
    if ((fbfd = open(FBDEV_FILE, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE);
        exit(1);
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar) < 0)
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
        exit(1);
    }
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fbfix))
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        exit(1);
    }
    screen_width = fbvar.xres;
    screen_height = fbvar.yres;
    line_length = fbfix.line_length;
    mem_size = line_length * screen_height;
    fb_mapped = (void *)mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    display_init(); ///init once

    printf("1.3M CIS Camera Application (320x240)\n");

    
}

void cameraClean(){
    munmap(fb_mapped, mem_size);
    close(dev_fd);
    close(fbfd);
}
void camera(char *image_buffer)
{
    
    write(dev_fd, NULL, 1);
    read(dev_fd, cisdata, CIS_IMAGE_SIZE);
    int x = 0, y = 0;
    for (y = 0; y < 240; y++)
    {
        for (x = 0; x < 320; x++)
        {
            unsigned int pixelIdx = ((y * 320) + x) * 3;
            image_buffer[pixelIdx + 0] = ((cisdata[y * 320 + x] & 0xF800) >> 11) << 3; //r
            image_buffer[pixelIdx + 1] = ((cisdata[y * 320 + x] & 0x07E0) >> 5) << 2;  //g
            image_buffer[pixelIdx + 2] = (cisdata[y * 320 + x] & 0x1F) << 3;           //b
        }
    }


}
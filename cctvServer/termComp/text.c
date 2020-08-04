#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "text.h"

#define TEXTLCD_BASE 0xbc
#define TEXT_LCD_COMMAND_SET _IOW(TEXTLCD_BASE, 0, int)
#define TEXT_LCD_FUNCTION_SET _IOW(TEXTLCD_BASE, 1, int)
#define TEXT_LCD_DISPLAY_CONTROL _IOW(TEXTLCD_BASE, 2, int)
#define TEXT_LCD_CURSOR_SHIFT _IOW(TEXTLCD_BASE, 3, int)
#define TEXT_LCD_ENTRY_MODE_SET _IOW(TEXTLCD_BASE, 4, int)
#define TEXT_LCD_RETURN_HOME _IOW(TEXTLCD_BASE, 5, int)
#define TEXT_LCD_CLEAR _IOW(TEXTLCD_BASE, 6, int)
#define TEXT_LCD_DD_ADDRESS _IOW(TEXTLCD_BASE, 7, int)
#define TEXT_LCD_WRITE_BYTE _IOW(TEXTLCD_BASE, 8, int)

void *fb_mapped;
int mem_size;

typedef struct strcommand_varible
{
    char rows;
    char nfonts;
    char display_enable;
    char curwor_enable;
    char nblink;
    char set_screen;
    char set_rightshift;
    char increase;
    char nshift;
    char pos;
    char command;
    char strlength;
    char buf[16];
} strc;

int i, dev;

strc strcommand;

char buf0[16] = "CCTV INITIALIZE";
char buf1[16] = "     CCTV Run   ";
char buf2[16] = "  CCTV PAUSE   ";
char buf3[16] = "WHAT ARE YOU?";

void display1()
{
    strcommand.pos = 0;
    ioctl(dev, TEXT_LCD_DD_ADDRESS, &strcommand, 32);
    ioctl(dev, TEXT_LCD_CLEAR, &strcommand, 32);
    for (i = 0; i < 16; i++)
    {
        strcommand.buf[0] = buf0[i];
        ioctl(dev, TEXT_LCD_WRITE_BYTE, &strcommand, 32);
    }
}

void display2()
{
    strcommand.pos = 0;
    ioctl(dev, TEXT_LCD_DD_ADDRESS, &strcommand, 32);
    ioctl(dev, TEXT_LCD_CLEAR, &strcommand, 32);
    for (i = 0; i < 16; i++)
    {
        strcommand.buf[0] = buf1[i];
        ioctl(dev, TEXT_LCD_WRITE_BYTE, &strcommand, 32);
    }
}

int textlcdInit()
{
    int cnt = 0;

    strcommand.rows = 0;
    strcommand.nfonts = 0;
    strcommand.display_enable = 1;
    strcommand.nblink = 0;
    strcommand.set_screen = 0;
    strcommand.set_rightshift = 1;
    strcommand.increase = 1;
    strcommand.nshift = 0;
    strcommand.pos = 10;
    strcommand.command = 1;
    strcommand.strlength = 16;
    dev = open("/dev/textlcd", O_RDONLY | O_NDELAY);
    if (dev != -1)
    {

        write(dev, buf0, 16);
        strcommand.pos = 40;
        ioctl(dev, TEXT_LCD_DD_ADDRESS, &strcommand, 32);
        write(dev, buf1, 16);
        sleep(1);

        ioctl(dev, TEXT_LCD_CLEAR, &strcommand, 32);
    }
    else
    {
        printf("can't run\n");
        exit(1);
    }
    return 0;
}

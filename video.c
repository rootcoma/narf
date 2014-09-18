#include "video.h"
#include "memory.h"
#include "portio.h"
#include "font.h"

unsigned char g_320x200x256[] =
{
    /* MISC */
    0x63,
    /* SEQ */
    0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
};

void write_regs(unsigned char *regs)
{
    unsigned int i;

    /* write MISCELLANEOUS reg */
    out(VGA_MISC_WRITE, *regs);
    regs++;
    /* write SEQUENCER regs */
    for(i = 0; i < VGA_NUM_SEQ_REGS; i++) {
        out(VGA_SEQ_INDEX, i);
        out(VGA_SEQ_DATA, *regs);
        regs++;
    }
    /* unlock CRTC registers */
    out(VGA_CRTC_INDEX, 0x03);
    out(VGA_CRTC_DATA, in(VGA_CRTC_DATA) | 0x80);
    out(VGA_CRTC_INDEX, 0x11);
    out(VGA_CRTC_DATA, in(VGA_CRTC_DATA) & ~0x80);
    /* make sure they remain unlocked */
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;
    /* write CRTC regs */
    for (i = 0; i < VGA_NUM_CRTC_REGS; i++) {
        out(VGA_CRTC_INDEX, i);
        out(VGA_CRTC_DATA, *regs);
        regs++;
    }
    /* write GRAPHICS CONTROLLER regs */
    for (i = 0; i < VGA_NUM_GC_REGS; i++) {
        out(VGA_GC_INDEX, i);
        out(VGA_GC_DATA, *regs);
        regs++;
    }
    /* write ATTRIBUTE CONTROLLER regs */
    for (i = 0; i < VGA_NUM_AC_REGS; i++) {
        in(VGA_INSTAT_READ);
        out(VGA_AC_INDEX, i);
        out(VGA_AC_WRITE, *regs);
        regs++;
    }
    /* lock 16-color palette and unblank display */
    in(VGA_INSTAT_READ);
    out(VGA_AC_INDEX, 0x20);
}

void draw_char (int x, int y, unsigned char c, int color)
{
    int start = c * 16;
    int i, j;
    for (i=0;i<16;i++)
        for (j=0; j<8; j++)
            if (g_8x16_font[start+i] & (1 << (8-j)))
                putpixel(x+j, i+y, color);
}

void init_vbe(int mode)
{
    g_video.mode = mode;
    g_video.w = g_video_buffer.w = 640;
    g_video.h = g_video_buffer.h = 480;
    g_video.memory = (void*)0xA0000;

    /* disable vbe so we can muck with it */
    outw(VBE_DISPI_IOPORT_INDEX, 4);
    outw(VBE_DISPI_IOPORT_DATA, 0);
    uint16_t config[] = {640, 480, 0x08, 0x1}; /* w, h, bpp, enable */
    int i;
    for (i=1;i<5;i++) {
        outw(VBE_DISPI_IOPORT_INDEX, i);
        outw(VBE_DISPI_IOPORT_DATA, config[i-1]);
    }
    memset((void*)g_video.memory, 0, 8192);
    memset((void*)g_video_buffer.memory, 0, g_video_buffer.w*g_video_buffer.h);
}

void init_vga(int mode)
{
    if(mode==VGA_320x200x256) {
        g_video.mode = g_video_buffer.mode = VGA_320x200x256;
        g_video.w = g_video_buffer.w = 320;
        g_video.h = g_video_buffer.h = 200;
        g_video.memory = (void*)0xA0000;
        g_video_buffer.memory = malloc(g_video.w*g_video.h);
        write_regs((unsigned char*)g_320x200x256);
        /* clear screen */
        memset((void*)g_video.memory, 0, g_video_buffer.w*g_video_buffer.h);
    } else if(mode==VGA_640x480x16) {
        g_video.mode = g_video_buffer.mode = VGA_640x480x16;
        g_video.w = g_video_buffer.w = 640;
        g_video.h = g_video_buffer.h = 480;
        g_video.memory = (void*)0xA0000;
        g_video_buffer.memory = malloc(g_video.w*g_video.h);
        //write_regs((unsigned char*)g_640x480x16);
        init_vbe(VGA_640x480x16);
    }
}

void write_pixel256 (int x, int y, int c)
{
    unsigned int o = g_video_buffer.w * y + x;
    g_video_buffer.memory[o] = c & 255;              // b 
    g_video_buffer.memory[o + 1] = (c >> 8) & 255;   // g
    g_video_buffer.memory[o + 2] = (c >> 16) & 255;  // r

}

void putpixel (int x, int y, int c)
{
    if(x<0 || x>g_video_buffer.w)
        return;
    if(y<0 || y>g_video_buffer.h)
        return;
    write_pixel256(x, y, c);
}

void clear_screen(void)
{
    memset(g_video_buffer.memory, 0, g_video_buffer.w*g_video_buffer.h);
}

void swap_buffer(void)
{
    if (g_video_buffer.mode == VGA_640x480x16) {
        int i;
        for (i=0;i<(640*480)/(8192*8)+1;i++) {
            outw(VBE_DISPI_IOPORT_INDEX, 5);
            outw(VBE_DISPI_IOPORT_DATA, i);
            memcpy(g_video.memory, g_video_buffer.memory+(8192*i*8), 8192*8);
        }
    } else 
        memcpy(g_video.memory, g_video_buffer.memory, g_video_buffer.w*g_video_buffer.h);
}

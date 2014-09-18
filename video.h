#ifndef VIDEO_H
#define VIDEO_H

#include <stddef.h>
#include <stdint.h>

#define VBE_DISPI_IOPORT_INDEX 0x1CE
#define VBE_DISPI_IOPORT_DATA 0x1CF

#define VGA_NUM_SEQ_REGS    5
#define VGA_NUM_CRTC_REGS   25
#define VGA_NUM_GC_REGS     9
#define VGA_NUM_AC_REGS     21
#define VGA_NUM_REGS        (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

#define VGA_AC_INDEX        0x3C0
#define VGA_AC_WRITE        0x3C0
#define VGA_AC_READ         0x3C1
#define VGA_MISC_WRITE      0x3C2
#define VGA_SEQ_INDEX       0x3C4
#define VGA_SEQ_DATA        0x3C5
#define VGA_DAC_READ_INDEX  0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA        0x3C9
#define VGA_MISC_READ       0x3CC
#define VGA_GC_INDEX        0x3CE
#define VGA_GC_DATA         0x3CF
/*                          COLOR emulation     MONO emulation */
#define VGA_CRTC_INDEX      0x3D4              /* 0x3B4 */
#define VGA_CRTC_DATA       0x3D5              /* 0x3B5 */
#define VGA_INSTAT_READ     0x3DA

/* Hardware text mode color constants. */
enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

enum vga_mode
{
    VGA_320x200x256 = 320,
    VGA_640x480x16 = 480
};

typedef struct video_t {
    uint8_t *memory;
    size_t w, h;
    unsigned int mode;
} video_t;

void swap_buffer(void);
void putpixel (int x, int y, int c);
void clear_screen(void);
void init_vga(int mode);
void draw_char (int x, int y, unsigned char c, int color);

struct video_t g_video_buffer;
struct video_t g_video;

#endif

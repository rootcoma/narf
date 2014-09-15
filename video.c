#include "video.h"
#include "memory.h"

void putpixel (int x, int y, int c)
{
    if(x<0 || x>g_video_buffer.w)
        return;
    if(y<0 || y>g_video_buffer.h)
        return;
    unsigned int o = g_video_buffer.w * y + x;
    g_video_buffer.memory[o] = c & 255;              // b 
    g_video_buffer.memory[o + 1] = (c >> 8) & 255;   // g
    g_video_buffer.memory[o + 2] = (c >> 16) & 255;  // r
}

void clear_screen(void)
{
    memset(g_video_buffer.memory, 0, g_video_buffer.w*g_video_buffer.h); 
}

void swap_buffer(void)
{
    memcpy(g_video.memory, g_video_buffer.memory, g_video_buffer.w*g_video_buffer.h);
}

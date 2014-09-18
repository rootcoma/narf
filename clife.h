#include <stdint.h>
#include <stdbool.h>

typedef struct board_t {
    uint8_t *data;
    unsigned int w, h;  
} board_t;

static char life_b[10], life_s[10];
unsigned int last_width, last_height;
struct board_t board, board_buffer;

void resize_life (const int w, const int h);
void create_board (const int w, const int h, board_t *b);
void randomize_life (void);
void init_life (char *rules);
void step_life (void);
bool get_bit (const int x, const int y, const board_t *b);
void set_bit (const int x, const int y, const bool p, board_t *b);
uint8_t count_neighbors (const int x, const int y, const board_t *b);

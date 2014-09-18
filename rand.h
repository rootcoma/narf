#include <stdint.h>
#define PHI 0x9e3779b9

static uint32_t Q[4096], c = 362436;
void init_rand (uint32_t c);
uint32_t rand (void);

#include <stdint.h>

#define DAC_BIAS 0x00010000 // Settling time set to 2,5us

uint32_t sine[50] = {512, 577,  640,  701,  759,  813, 863, 907, 945, 976,
                     999, 1015, 1023, 1023, 1015, 999, 976, 945, 907, 863,
                     813, 759,  701,  640,  577,  513, 448, 385, 324, 266,
                     212, 162,  118,  80,   49,   26,  10,  2,   2,   10,
                     26,  49,   80,   118,  162,  212, 266, 324, 385, 448};
uint32_t sample_idx = 0;

void DACInit(void);
void init();
void TIMInit(void);

/** 0b001 to enable interrupt */
#define MATCH_INT 1
/** 0b010 to enable reset of the counter */
#define MATCH_RST 2
/** 0b100 to stop the counter */
#define MATCH_STP 4

#define MR_0 0
#define MR_1 1
#define MR_2 2
#define MR_3 3

#define setmatch(timer, reg, val)               \
  timer->MCR |= (val << (reg * 3))

#define clrmatch(timer, reg, val)               \
  timer->MCR &= ~(val << (reg * 3))

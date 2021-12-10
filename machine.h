#ifndef __MACHINE_H__
#define __MACHINE_H__

#define RESOLUTION_X    (320)
#define RESOLUTION_Y    (240)

#define CHAR_ROM_SZ     (4096)
const volatile int8_t* CHARACTER_ROM_mem;

volatile int8_t* SRCEEN_RAM_mem;
int machine_init_mem(void);

#endif


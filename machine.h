#ifndef __MACHINE_H__
#define __MACHINE_H__

#define RESOLUTION_X    (320)
#define RESOLUTION_Y    (240)

#define CHAR_ROM_SZ     (4096)
const volatile uint8_t* CHARACTER_ROM_mem;

#define PALLET_COLOR_NES_BLACK  (0x0)
#define PALLET_COLOR_NES_WHITE  (0x1)
#define PALLET_ROM_SZ     (64)
const volatile uint32_t PALLET_ROM_mem[PALLET_ROM_SZ];

#define SCREEN_MATRIX_X (size_t)(RESOLUTION_X / 8)
#define SCREEN_MATRIX_Y (size_t)(RESOLUTION_Y / 8)
#define SCREEN_MEM_SZ   (SCREEN_MATRIX_X * SCREEN_MATRIX_Y)
volatile uint8_t* SRCEEN_RAM_mem;

int machine_init_mem(void);

#endif


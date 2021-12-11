#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "machine.h"

/* Got these from here:
https://lospec.com/palette-list/nintendo-entertainment-system
https://lospec.com/palette-list/nintendo-entertainment-system.txt

    TODO - load/mmap from file
    Since there are 55 values, I rounded up to ^2 and filled with zeros

    Also, could eliminate the 0xff prefix - and make uint_8 addressable
*/
const volatile uint32_t PALLET_ROM_mem[PALLET_ROM_SZ] = { 
0xFF000000, 0xFFfcfcfc, 0xFFf8f8f8, 0xFFbcbcbc, 0xFF7c7c7c, 0xFFa4e4fc, 0xFF3cbcfc, 0xFF0078f8,
0xFF0000fc, 0xFFb8b8f8, 0xFF6888fc, 0xFF0058f8, 0xFF0000bc, 0xFFd8b8f8, 0xFF9878f8, 0xFF6844fc,
0xFF4428bc, 0xFFf8b8f8, 0xFFf878f8, 0xFFd800cc, 0xFF940084, 0xFFf8a4c0, 0xFFf85898, 0xFFe40058,
0xFFa80020, 0xFFf0d0b0, 0xFFf87858, 0xFFf83800, 0xFFa81000, 0xFFfce0a8, 0xFFfca044, 0xFFe45c10,
0xFF881400, 0xFFf8d878, 0xFFf8b800, 0xFFac7c00, 0xFF503000, 0xFFd8f878, 0xFFb8f818, 0xFF00b800,
0xFF007800, 0xFFb8f8b8, 0xFF58d854, 0xFF00a800, 0xFF006800, 0xFFb8f8d8, 0xFF58f898, 0xFF00a844,
0xFF005800, 0xFF00fcfc, 0xFF00e8d8, 0xFF008888, 0xFF004058, 0xFFf8d8f8, 0xFF787878, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};


int machine_init_mem() {

//Screen memory

#if 0
int screen_fd = open("mem/screen", O_CREAT | O_RDWR);
if (screen_fd == -1) {
    perror("couldn't open screen file:");
    return -1;
}
#endif

printf("Screen matrix (x,y) (%zu,%zu) characters\n", SCREEN_MATRIX_X, SCREEN_MATRIX_Y);

SRCEEN_RAM_mem = mmap(NULL, SCREEN_MEM_SZ, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
if (SRCEEN_RAM_mem == MAP_FAILED) {
    perror("couldn't mmap screen ram:");
    return -1;
}

printf("[%p] - [screen memory RAM] mmap (%zu bytes mapped)\n", SRCEEN_RAM_mem, SCREEN_MEM_SZ);


//Character rom

size_t page_sz = sysconf(_SC_PAGE_SIZE);

size_t length = (CHAR_ROM_SZ / page_sz) * page_sz;

if ((length % page_sz) != 0) {
    length += page_sz;
}

int char_fd = open("c64.bin", O_RDONLY);
if (char_fd == -1) {
    perror("couldn't open character rom:");
    return -1;
}


CHARACTER_ROM_mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, char_fd, 0);   
if (CHARACTER_ROM_mem == MAP_FAILED) {
    perror("couldn't mmap character rom:");
    return -1;
}

printf("[%p] - [character ROM] (page size %zu) mmap (file size %zu bytes)\n", CHARACTER_ROM_mem, page_sz, length);


return 0;
}

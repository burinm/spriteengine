#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "machine.h"

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

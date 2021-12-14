#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "machine.h"
#include "screen_matrix.h" //TODO: refactor, is this a dependency
#include "graphics.h" //only for g_update_renderer - refactor?

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

int screen_fd = shm_open("/screen_mem", O_CREAT | O_RDWR, S_IRWXU);
if (screen_fd == -1) {
    perror("couldn't open screen file:");
    return -1;
}

#if 0
//Alternatively, this could be dd'ed from command line
for (int i=0; i<SCREEN_MEM_SZ; i++) {
    if (write(screen_fd, "\0", 1) == -1) {
        perror("problem creating screen backing file");
        goto error;
        return -1;
    }
}
#endif
if (ftruncate(screen_fd, SCREEN_MEM_SZ) == -1) {
        perror("couldn't set screen memory size:");
        goto error;
        return -1;
}

struct stat stat_buf;
    if (fstat(screen_fd, &stat_buf) != 0) {
        perror("couldn't stat screen memory file:");
        goto error;
        return -1;
    }

//Check to see if ram was corrupted
if (stat_buf.st_size != SCREEN_MEM_SZ) {
    printf("Screen ram memory must be %u bytes!\n", SCREEN_MEM_SZ);
    goto error;
    return -1;
}



printf("Screen matrix (x,y) (%u,%u) characters\n", SCREEN_MATRIX_X, SCREEN_MATRIX_Y);

//SRCEEN_RAM_mem = mmap(NULL, SCREEN_MEM_SZ, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
SRCEEN_RAM_mem = mmap(NULL, SCREEN_MEM_SZ, PROT_READ|PROT_WRITE, MAP_SHARED, screen_fd, 0);
if (SRCEEN_RAM_mem == MAP_FAILED) {
    perror("couldn't mmap screen ram:");
    goto error;
    return -1;
}

printf("[%p] - [screen memory RAM] mmap (%u bytes mapped)\n", SRCEEN_RAM_mem, SCREEN_MEM_SZ);

#if 0
if (mlock(SRCEEN_RAM_mem, SCREEN_MEM_SZ) == -1) {
    perror("couldn't lock screen ram:");
    goto error;
    return -1;
}
#endif


//Character rom

size_t page_sz = sysconf(_SC_PAGE_SIZE);

size_t length = (CHAR_ROM_SZ / page_sz) * page_sz;

if ((length % page_sz) != 0) {
    length += page_sz;
}

int char_fd = open("c64.bin", O_RDONLY);
if (char_fd == -1) {
    perror("couldn't open character rom:");
    goto error;
    return -1;
}


CHARACTER_ROM_mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, char_fd, 0);   
if (CHARACTER_ROM_mem == MAP_FAILED) {
    perror("couldn't mmap character rom:");
    goto error;
    return -1;
}

printf("[%p] - [character ROM] (page size %zu) mmap (file size %zu bytes)\n", CHARACTER_ROM_mem, page_sz, length);



error:
    //TODO clean up mmap and other resources
    //close(screen_fd);
    if (shm_unlink("SCREEN_MEM_HANDLE") == -1) {
        perror("couldn't unlink screen memory:");;
    }
    //return -1;
return 0;
}

void machine_clear_screen_matrix(void) {
    for (int i=0; i<SCREEN_MEM_SZ; i++) {
        SRCEEN_RAM_mem[i] = 0;
    }

}

uint32_t do_vsync(uint32_t interval, void* param) {
    vsync_params_t *v_param = (vsync_params_t*)param;

    //msync(SRCEEN_RAM_mem, SCREEN_MEM_SZ, MS_SYNC);
    screen_render_from_matrix(v_param->pixels);

    g_update_renderer(v_param->pixels);

//printf("tick\n");
return (interval);
}

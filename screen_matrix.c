#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "screen_matrix.h"
#include "machine.h"

int load_screen_matrix(const char* matrix) {

int matrix_fd = open(matrix, O_RDONLY);
    if (matrix_fd == -1) {
        perror("couldn't open matrix file:");
        return -1;
    } 

    struct stat stat_buf;
    if (fstat(matrix_fd, &stat_buf) != 0) {
        perror("couldn't stat matrix file:");
        return -1;

    }

    if (stat_buf.st_size != SCREEN_MEM_SZ) {
        printf("Screen matrix file must be (%u x %u) %u bytes\n",
            SCREEN_MATRIX_X, SCREEN_MATRIX_Y, SCREEN_MEM_SZ);  
        return -1;
    }

    ssize_t bytes_read = read(matrix_fd, (void*)SRCEEN_RAM_mem, SCREEN_MEM_SZ);  
    if (bytes_read == -1) {
        perror("couldn't read matrix file:");
        return -1;
    }

    printf("Read in new screen matrix of %zd bytes\n", bytes_read);
        

return 0;
}

void load_screen_matrix_on_right_from_memory(size_t offset) {
    int location_level;
    int location_screen;
    for (int j=0; j<SCREEN_MATRIX_Y; j++) {
        location_screen = SCREEN_MATRIX_X-1 + j*SCREEN_MATRIX_X;
        if (offset < LEVEL_COLS) {
            location_level = offset + j*LEVEL_COLS;
            SRCEEN_RAM_mem[location_screen] =
                LEVELMAP_ROM_mem[location_level];
        } else {
            SRCEEN_RAM_mem[location_screen] = 32;
        }
    }
}

void matrix_scroll_left() {
    for (int j=0; j<SCREEN_MATRIX_Y; j++) {
        for (int i=0; i < SCREEN_MATRIX_X-1; i++) {
            SRCEEN_RAM_mem[i + j*SCREEN_MATRIX_X] = SRCEEN_RAM_mem[1 + i + j*SCREEN_MATRIX_X];
        }
    }
}

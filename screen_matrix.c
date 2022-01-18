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

void load_screen_matrix_from_memory(size_t offset) {
    int location;
    for (int j=0; j<SCREEN_MATRIX_Y; j++) {
        for (int i=0; i<SCREEN_MATRIX_X; i++) {
            if (i + offset < LEVEL_COLS) {
                location = i + offset + j*LEVEL_COLS;
                SRCEEN_RAM_mem[i + j*SCREEN_MATRIX_X] =
                    LEVELMAP_ROM_mem[location];
            } else {
                SRCEEN_RAM_mem[i + j*SCREEN_MATRIX_X] = 32;
            }
        }
    }

}

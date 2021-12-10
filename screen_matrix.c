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
        printf("Screen matrix file must be (%zu x %zu) %zu bytes\n",
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

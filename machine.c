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


//Character rom

size_t page_sz = sysconf(_SC_PAGE_SIZE);

size_t length = (CHAR_ROM_SZ / page_sz) * page_sz; 

if ((length % page_sz) != 0) {
    length += page_sz;
}

int fd = open("c64.bin", O_RDONLY);
if (fd == -1) {
    perror("couldn't't open character rom:");
    return -1;
}


printf("(page size %zu) for mmap file size %zu bytes\n", page_sz, length);

CHARACTER_ROM_mem = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);   
if (*CHARACTER_ROM_mem == -1) {
    perror("couldn't mmap character rom:");
    return -1;
}


return 0;
}

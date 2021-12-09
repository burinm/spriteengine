#include <stdint.h>
#include "blitz_n_chips.h"

#include "machine.h"

void blit_characters(void* p) {

    for(int j=0; j<50; j++) {
        //scan one line
        for(int i=0; i<RESOULTION_X; i++) {
            *(uint8_t *)p++ = 0xee; 
            *(uint8_t *)p++ = 0xaa; 
            *(uint8_t *)p++ = 0xcc; 
        };
    }

}


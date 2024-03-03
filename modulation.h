#ifndef MODULATION_H
#define MODULATION_H

#include <stdint.h>

void start_modulation(uint32_t time_in_s) {
    uint64_t time_in_ms = time_in_s * 1000;

    for(int current_tti = 0; current_tti < time_in_ms; current_tti++) {

    }
}


#endif // MODULATION_H

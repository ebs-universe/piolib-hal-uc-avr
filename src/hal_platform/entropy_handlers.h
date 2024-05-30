/* 
   Copyright (c)
   (c) 2024 Chintalagiri Shashank
   (c) 2014 Walter Anderson
      
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : AVR
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */


// The code in this file is based on the arduino C++ Entropy library
// by Walter anderson. Use of uC_ENTROPY_SOURCE 2 triggers inclusion 
// of this file in the build, requiring the use of GPLv3 licensed 
// code.
//
// @see https://github.com/sylvandb/avr-hardware-random-number-generation


#include <stdint.h>
#include <avr/io.h>
#include <hal/uc/entropy.h>
#include <ds/bytebuf.h>
#include <hal/uc/timer.h>

#if uC_ENTROPY_ENABLED && uC_ENTROPY_SOURCE == 2

static inline void wdt_entropy_isr(void)
{
    uint8_t sourcebyte;
    switch (timer_if[uC_ENTROPY_TIMER]->hwif->type) {
        case TIMER_HWIF_TIMER_16:
            sourcebyte = HWREG8(timer_if[uC_ENTROPY_TIMER]->hwif->base + OFS_TIMER_TCNTxL);
            break;
        default:
            sourcebyte = HWREG8(timer_if[uC_ENTROPY_TIMER]->hwif->base + OFS_TIMER_TCNTx);
            break;
    } 
    bytebuf_cWriteByte(entropy_state.source, sourcebyte);
    if (bytebuf_cPopulation(entropy_state.source) == uC_ENTROPY_SOURCE_BUFFER_SIZE)
    {
        union {
            uint32_t word;
            uint8_t bytes[4];
        } entropy;

        // The following code is an implementation of Jenkin's one at a time hash
        // This hash function has had preliminary testing to verify that it
        // produces reasonably uniform random results when using WDT jitter
        // on a variety of Arduino platforms
        for(uint8_t gWDT_loop_counter = 0; 
                    gWDT_loop_counter < uC_ENTROPY_SOURCE_BUFFER_SIZE; 
                    ++gWDT_loop_counter) {
            entropy.word += entropy_state.source->_bufp[gWDT_loop_counter];
            entropy.word += (entropy.word << 10);
            entropy.word ^= (entropy.word >> 6);
        }
        entropy.word += (entropy.word << 3);
        entropy.word ^= (entropy.word >> 11);
        entropy.word += (entropy.word << 15);
        
        for(uint8_t wc = 0; wc < 4; wc++) {
            bytebuf_cWriteByte(entropy_state.entropy, entropy.bytes[wc]);
        }
        #if uC_ENTROPY_MODE == 1
            if (!bytebuf_cGetFree(entropy_state.entropy)) {
                entropy_deinit();
            };
        #endif
        bytebuf_vFlush(entropy_state.source);
    }
}

#endif
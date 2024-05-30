/* 
   Copyright (c)
   (c) 2024 Chintalagiri Shashank
      
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : AVR
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include <application.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <hal/uc/entropy.h>
#include <hal/uc/timer.h>
#include <platform/cpu.h>

#ifndef uC_ENTROPY_ENABLED
#define uC_ENTROPY_ENABLED   0
#endif

#ifndef uC_ENTROPY_SOURCE
#define uC_ENTROPY_SOURCE   2
#endif


#if uC_ENTROPY_ENABLED 

uint8_t entropy_buffer[uC_ENTROPY_POOL_SIZE];
uint8_t entropy_source_buffer[uC_ENTROPY_SOURCE_BUFFER_SIZE];
bytebuf entropy_buf;
bytebuf entropy_source;
entropy_state_t entropy_state;

static inline void _entropy_state_init(void){
    // If the size is not set, we assume we've never initialized the 
    // buffer. This has the implicit and possibly incorrect assumption 
    // that RAM will be reset to 0s on processor reset.    
    entropy_state.accumulating = 1;
    if (!entropy_buf._size) {
        bytebuf_vInit(entropy_buffer, uC_ENTROPY_POOL_SIZE, &entropy_buf);
    }
    if (!entropy_source._size) {
        bytebuf_vInit(entropy_source_buffer, uC_ENTROPY_SOURCE_BUFFER_SIZE, &entropy_source);
    }
    entropy_state.entropy = &entropy_buf;
    entropy_state.source = &entropy_source;
}

static inline void _entropy_state_deinit(void){
    // If the size is not set, we assume we've never initialized the 
    // buffer. This has the implicit and possibly incorrect assumption 
    // that RAM will be reset to 0s on processor reset.    
    entropy_state.accumulating = 0;
}

#if uC_ENTROPY_SOURCE == 1

void entropy_init(void) {
    _entropy_state_init();
}

void entropy_deinit(void) {
    _entropy_state_deinit();
}


#endif

#if uC_ENTROPY_SOURCE == 2
#include <hal/uc/core.h>
#include <stdlib.h>

typedef struct{
    uint8_t wdtcsr_bak;
} entropy_backup_t;

static entropy_backup_t * entropy_backup;


void entropy_init(void){
    // Note that watchdog resets are disabled in this condition.
    // Further, if the application services the watchdog, the 
    // randomness generated will be a convolution of whatever is
    // happening here and the loop that's controlling the watchdog 
    // service, and myst separately be validated.
    entropy_backup = malloc(sizeof(entropy_backup_t));
    critical_enter();
    wdt_reset();
    MCUSR = 0;
    WDTCSR = (1 << WDCE) | (1 << WDE);
    WDTCSR = (1 << WDIE);
    critical_exit();
    // TODO Save timer state. Also disable interrupts if set.
    timer_init(uC_ENTROPY_TIMER);
    timer_set_prescaler(
        uC_ENTROPY_TIMER, 
        TIMER_PRESCALER_DIV1
    );
    timer_set_mode(
        uC_ENTROPY_TIMER, 
        TIMER_MODE_COUNTING
    );
    _entropy_state_init();
}

void entropy_deinit(void){
    _entropy_state_deinit();
    // TODO Restore timer state instead.
    timer_set_mode(
        uC_ENTROPY_TIMER, 
        TIMER_MODE_STOPPED
    );
    critical_enter();
    wdt_reset();
    MCUSR = 0;
    WDTCSR = (1 << WDCE) | (1 << WDE);
    WDTCSR = 0x00;
    // TODO This causes a reset loop. Work it out later.
    // WDTCSR = entropy_backup->wdtcsr_bak;
    critical_exit();
    free(entropy_backup);
}

#endif
#endif

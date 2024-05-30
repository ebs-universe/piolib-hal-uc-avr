/* 
 Copyright (c)
     (c) 2024 Chintalagiri Shashank
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
 
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

#ifndef CORE_IMPL_H
#define CORE_IMPL_H

#include <application.h>
#include <hal/uc/core.h>
#include <avr/io.h>
#include <avr/wdt.h>


#if !defined(__AVR_ATmega328P__) \
    && !defined(__AVR_ATmega168P__) \
    && !defined(__AVR_ATmega88P__)
    #warning "TIMER interface is untested for the chosen MCU."
#endif

typedef enum {
    MCLK_PRESCALER_DIV1    = 1, 
    MCLK_PRESCALER_DIV2    = 2,
    MCLK_PRESCALER_DIV4    = 4,  
    MCLK_PRESCALER_DIV8    = 8,
    MCLK_PRESCALER_DIV16   = 16, 
    MCLK_PRESCALER_DIV32   = 32,
    MCLK_PRESCALER_DIV64   = 64,
    MCLK_PRESCALER_DIV128  = 128,
    MCLK_PRESCALER_DIV256  = 256
} MCLK_PRESCALER;

typedef enum {
    WDT_PRESCALER_DIV2K     = 2,
    WDT_PRESCALER_DIV4K     = 4,
    WDT_PRESCALER_DIV8K     = 8,  
    WDT_PRESCALER_DIV16K    = 16,
    WDT_PRESCALER_DIV32K    = 32, 
    WDT_PRESCALER_DIV64K    = 64,
    WDT_PRESCALER_DIV128K   = 128,
    WDT_PRESCALER_DIV256K   = 256,
    WDT_PRESCALER_DIV512K   = 512,
    WDT_PRESCALER_DIV1024K  = 1024,
} WDT_PRESCALER;

#if uC_WDT_ENABLED == 1

static inline void watchdog_clear(void){
    wdt_reset();
}

#endif

#endif

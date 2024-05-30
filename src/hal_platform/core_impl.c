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

#include <hal/uc/core.h>
#include <platform/cpu.h>
#include <platform/debug.h>

#if uC_WDT_ENABLED == 1

void set_wdt_prescaler(WDT_PRESCALER prescaler);

/**
* @brief Initializes the Watchdog timer. Nothing to do here.
* 
*/
void watchdog_init(void) {
    ;
}


/**
* @brief Initializes the Watchdog timer. Nothing to do here.
* 
* Presently just sets the default values. This should be changed to 
* accept some parameter, perhaps interval, and set accordingly. 
* 
* Default should be an approximately 2 sec timeout.
*/
void watchdog_start(void){
    critical_enter();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    WDTCSR = (1 << WDCE) | (1 << WDE);
    set_wdt_prescaler(WDT_PRESCALER_DIV128K);
    critical_exit();
}

void watchdog_hold(void){
    critical_enter();
    wdt_reset();
    MCUSR &= ~(1 << WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = 0x00;
    critical_exit();
}

void set_wdt_prescaler(WDT_PRESCALER prescaler) {
    uint8_t wdp_bits = 0;

    switch (prescaler) {
        case WDT_PRESCALER_DIV2K:
            wdp_bits = 0b0000;
            break;
        case WDT_PRESCALER_DIV4K:
            wdp_bits = 0b0001;
            break;
        case WDT_PRESCALER_DIV8K:
            wdp_bits = 0b0010;
            break;
        case WDT_PRESCALER_DIV16K:
            wdp_bits = 0b0011;
            break;
        case WDT_PRESCALER_DIV32K:
            wdp_bits = 0b0100;
            break;
        case WDT_PRESCALER_DIV64K:
            wdp_bits = 0b0101;
            break;
        case WDT_PRESCALER_DIV128K:
            wdp_bits = 0b0110;
            break;
        case WDT_PRESCALER_DIV256K:
            wdp_bits = 0b0111;
            break;
        case WDT_PRESCALER_DIV512K:
            wdp_bits = 0b1000;
            break;
        case WDT_PRESCALER_DIV1024K:
            wdp_bits = 0b1001;
            break;
    }

    // Set the WDTCSR register with the computed wdp_bits and enable the watchdog
    WDTCSR = (1<<WDIE) | (1 << WDE) | (wdp_bits & 0b0111) | (wdp_bits &0b1000 << 2);
}

#endif

/**
* @brief Sets the MCU Power controls to defaults.
* 
* Power controls not implemented.
* 
*/
void power_set_full(void){
    ;
}

/**
* @brief Sets System Clock Prescaler
* 
* Don't generally use this. While the prescaler will change, many other 
* linked elements won't. 
*
* Additional HAL functions for clock controls for power optimization
* are yet to be added. Specifically, a number of peripherals will need
* need to adjust their clock related registers to follow changes in 
* system clock : 
*     - Recalculation of UART UBRR to maintain baud rate
*     - Recalculation of Timer Prescaler and Top to maintain timer period
*
* Specific details of how these are to be implemented are as yet unclear.
* Whether or not this will ever be implemented is also unclear. A concrete
* use case will be needed to justify the effort required.
* 
*/
void clock_set_prescaler(uint16_t ps){
    uint8_t clkps = 0;
    switch (ps) {
        case MCLK_PRESCALER_DIV1:
            clkps = 0b0000;
            break;
        case MCLK_PRESCALER_DIV2:
            clkps = 0b0001;
            break;
        case MCLK_PRESCALER_DIV4:
            clkps = 0b0010;
            break;
        case MCLK_PRESCALER_DIV8:
            clkps = 0b0011;
            break;
        case MCLK_PRESCALER_DIV16:
            clkps = 0b0100;
            break;
        case MCLK_PRESCALER_DIV32:
            clkps = 0b0101;
            break;
        case MCLK_PRESCALER_DIV64:
            clkps = 0b0110;
            break;
        case MCLK_PRESCALER_DIV128:
            clkps = 0b0111;
            break;
        case MCLK_PRESCALER_DIV256:
            clkps = 0b1000;
            break;
        default:
            die();
            break;
    }
    critical_enter();
    CLKPR = (1<<CLKPCE);
    CLKPR = clkps & 0x0F;
    critical_exit();    
}

/**
* @brief Sets the clock to the default
* 
* Sets the clock prescaler to /1. 
*
*/
void clock_set_default(void){
    clock_set_prescaler(MCLK_PRESCALER_DIV1);
}

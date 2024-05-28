/* 
   Copyright 2024 Chintalagiri Shashank
   Copyright 2015 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
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

#ifndef GPIO_IMPL_H
#define GPIO_IMPL_H

#include <platform/types.h>
#include <avr/io.h>

#if !defined(__AVR_ATmega328P__) \
    && !defined(__AVR_ATmega168P__) \
    && !defined(__AVR_ATmega88P__)
    #warning "GPIO interface is untested for the chosen MCU. Addresses may need fixing."
#endif

#define uC_PB 0x23
#define uC_PC 0x26
#define uC_PD 0x29

#define PORT_OFFSET 2
#define DDR_OFFSET  1
#define PIN_OFFSET  0


static inline void gpio_conf_output(PORTSELECTOR_t port,
                                    PINSELECTOR_t pin){
    HWREG8(port + DDR_OFFSET) |= (1 << pin);
}

static inline void gpio_conf_input(PORTSELECTOR_t port,
                                   PINSELECTOR_t pin){
    HWREG8(port + DDR_OFFSET) &= ~(1 << pin);
}

static inline void gpio_conf_periph(PORTSELECTOR_t port,
                                    PINSELECTOR_t pin,
                                    uint8_t periph
                                   ) __attribute__((error("AVR does not need peripherals to be configured at the IOs. Make sure to set the direction correctly, though.")));

static inline void gpio_conf_periph(PORTSELECTOR_t port,
                                    PINSELECTOR_t pin,
                                    uint8_t periph) { };

static inline void gpio_set_output_high(PORTSELECTOR_t port,
                                        PINSELECTOR_t pin){
    HWREG8(port + PORT_OFFSET) |= (1 << pin);
}

static inline void gpio_set_output_low(PORTSELECTOR_t port,
                                       PINSELECTOR_t pin){
    HWREG8(port + PORT_OFFSET) &= ~(1 << pin);
}

static inline void gpio_set_output_toggle(PORTSELECTOR_t port,
                                          PINSELECTOR_t pin){
    HWREG8(port + PORT_OFFSET) ^= (1 << pin);
}

static inline void gpio_conf_input_pullup(PORTSELECTOR_t port,
                                          PINSELECTOR_t pin){
    HWREG8(port + DDR_OFFSET) &= ~(1 << pin);
    HWREG8(port + PORT_OFFSET) |= (1 << pin);
}                         

static inline void gpio_conf_input_pulldown(PORTSELECTOR_t port,
                                            PINSELECTOR_t pin) __attribute__((error("AVR does not have input pulldowns")));

static inline void gpio_conf_input_pulldown(PORTSELECTOR_t port,
                                            PINSELECTOR_t pin) {}

static inline void gpio_conf_input_hiz(PORTSELECTOR_t port,
                                       PINSELECTOR_t pin){
    HWREG8(port + DDR_OFFSET) &= ~(1 << pin);
    HWREG8(port + PORT_OFFSET) &= ~(1 << pin);
}

static inline HAL_BASE_t gpio_get_input(PORTSELECTOR_t port,
                                        PINSELECTOR_t pin){
    return (HWREG8(port + PIN_OFFSET) & (1 << pin)) ? 1 : 0;
}

#endif
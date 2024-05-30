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

#include <hal/uc/core.h>
#include <avr/interrupt.h>
#include "core_handlers.h"
#include <handlers.h>
#if uC_ENTROPY_ENABLED && uC_ENTROPY_SOURCE == 2
#include "entropy_handlers.h"
#endif

volatile uint8_t __core_handler_inclusion;

#if uC_WDT_ENABLED || (uC_ENTROPY_ENABLED && uC_ENTROPY_SOURCE == 2) 
    ISR(WDT_vect) {
        #if uC_ENTROPY_ENABLED && uC_ENTROPY_SOURCE == 2
            if (entropy_state.accumulating) {
                wdt_entropy_isr();
            }
        #endif
        #ifdef uC_WDT_IRQH
            uC_WDT_IRQH();
        #endif
    }
#endif

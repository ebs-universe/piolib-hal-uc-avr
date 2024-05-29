/* 
   Copyright (c)
   (c) 2024 Chintalagiri Shashank
   (c) 2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
      
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

#include <hal/uc/timer.h>
#include <platform/types.h>
#include <avr/interrupt.h>
#include "timer_handlers.h"

#include <handlers.h>


volatile uint8_t __timer_handler_inclusion;

#if uC_TIMER0_ENABLED
    static inline void timer0_handler_overflow(void);
    static inline void timer0_handler_oc0(void);
    static inline void timer0_handler_oc1(void);

    static inline void timer0_handler_overflow(void){ };
    static inline void timer0_handler_oc0(void){ };
    static inline void timer0_handler_oc1(void){ };

    ISR(TIMER0_OVF_vect) {
        #ifdef uC_TIMER0_OVF_IRQH
            uC_TIMER0_OVF_IRQH();
        #endif
        timer0_handler_overflow();
    }

    ISR(TIMER0_COMPA_vect) {
        #ifdef uC_TIMER0_OC0_IRQH
            uC_TIMER0_OC0_IRQH();
        #endif
        timer0_handler_oc0();
    }

    ISR(TIMER0_COMPB_vect) {
        #ifdef uC_TIMER0_OC1_IRQH
            uC_TIMER0_OC1_IRQ_HANDLER();
        #endif
        timer0_handler_oc1();
    }

#endif

#if uC_TIMER1_ENABLED
    static inline void timer1_handler_overflow(void);
    static inline void timer1_handler_oc0(void);
    static inline void timer1_handler_oc1(void);

    static inline void timer1_handler_overflow(void){ };
    static inline void timer1_handler_oc0(void){ };
    static inline void timer1_handler_oc1(void){ };

    ISR(TIMER1_OVF_vect) {
        #ifdef uC_TIMER1_OVF_IRQH
            uC_TIMER1_OVF_IRQH();
        #endif
        timer1_handler_overflow();
    }

    ISR(TIMER1_COMPA_vect) {
        #ifdef uC_TIMER1_OC0_IRQH
            uC_TIMER1_OC0_IRQH();
        #endif
        timer1_handler_oc0();
    }

    ISR(TIMER1_COMPB_vect) {
        #ifdef uC_TIMER1_OC1_IRQH
            uC_TIMER1_OC1_IRQ_HANDLER();
        #endif
        timer1_handler_oc1();
    }

#endif

#if uC_TIMER2_ENABLED
    static inline void timer2_handler_overflow(void);
    static inline void timer2_handler_oc0(void);
    static inline void timer2_handler_oc1(void);

    static inline void timer2_handler_overflow(void){ };
    static inline void timer2_handler_oc0(void){ };
    static inline void timer2_handler_oc1(void){ };

    ISR(TIMER2_OVF_vect) {
        #ifdef uC_TIMER2_OVF_IRQH
            uC_TIMER2_OVF_IRQH();
        #endif
        timer2_handler_overflow();
    }

    ISR(TIMER2_COMPA_vect) {
        #ifdef uC_TIMER2_OC0_IRQH
            uC_TIMER2_OC0_IRQH();
        #endif
        timer2_handler_oc0();
    }

    ISR(TIMER2_COMPB_vect) {
        #ifdef uC_TIMER1_OC1_IRQH
            uC_TIMER2_OC1_IRQ_HANDLER();
        #endif
        timer1_handler_oc1();
    }

#endif

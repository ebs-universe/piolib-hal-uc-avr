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


#include "hal/uc/timer.h"
#include <stddef.h>

#ifdef uC_INCLUDE_TIMER_IFACE

#if uC_TIMER0_ENABLED
    static const _timer_hwif_t _timer0_hwif = {
        uC_TIMER0_TYPE, uC_TIMER0_BASE
    };
        
    timer_state_t timer0_state = {0};
        
    const timer_if_t timer0_if = {
        &_timer0_hwif,
        &timer0_state,
    };  
#endif

#if uC_TIMER1_ENABLED
    static const _timer_hwif_t _timer1_hwif = {
        uC_TIMER1_TYPE, uC_TIMER1_BASE,
    };
        
    timer_state_t timer1_state = {0};
        
    const timer_if_t timer1_if = {
        &_timer1_hwif,
        &timer1_state,
    };  
#endif
    
#if uC_TIMER2_ENABLED
    static const _timer_hwif_t _timer2_hwif = {
        uC_TIMER2_TYPE, uC_TIMER2_BASE, 
    };
        
    timer_state_t timer2_state = {0};
        
    const timer_if_t timer2_if = {
        &_timer2_hwif,
        &timer2_state,
    };
#endif


// While it is tempting to use the application defines to trim 
// this to the needed number, the cost of this structure may be 
// lower than the headaches associated with that. 
// Specifically, we're using the intfnum (array index) to determine 
// the correct addresses for the interrupt controls. 
// If that cannot be relied on from this array, timer_if_t 
// must include a uint8_t interface number and the code will need 
// to be updated accordingly.
const timer_if_t *const timer_if[uC_TIMERS_COUNT] = {
    #if uC_TIMER0_ENABLED
        &timer0_if
    #else
        NULL
    #endif
    ,
    #if uC_TIMER1_ENABLED
        &timer1_if
    #else
        NULL
    #endif
    ,
    #if uC_TIMER2_ENABLED
        &timer2_if
    #else
        NULL
    #endif
};


#ifdef uC_INCLUDE_TIMER_8A_IFACE
    void timer_8A_init(uint8_t intfnum){
        timer_set_mode(intfnum, TIMER_MODE_STOPPED);
        timer_set_prescaler(intfnum, TIMER_PRESCALER_DIV1);
        timer_disable_int_overflow(intfnum);
        timer_disable_int_top(intfnum);
        timer_set_cmr_ch(intfnum, 0, 0);
        timer_set_cmr_ch(intfnum, 1, 0);
    }
#endif

#ifdef uC_INCLUDE_TIMER_8B_IFACE
    void timer_8B_init(uint8_t intfnum){
        timer_set_mode(intfnum, TIMER_MODE_STOPPED);
        timer_set_prescaler(intfnum, TIMER_PRESCALER_DIV1);
        timer_disable_int_overflow(intfnum);
        timer_disable_int_top(intfnum);
        timer_set_cmr_ch(intfnum, 0, 0);
        timer_set_cmr_ch(intfnum, 1, 0);
    }
#endif

#ifdef uC_INCLUDE_TIMER_16_IFACE
    void timer_16_init(uint8_t intfnum){
        timer_set_mode(intfnum, TIMER_MODE_STOPPED);
        timer_set_prescaler(intfnum, TIMER_PRESCALER_DIV1);
        timer_disable_int_overflow(intfnum);
        timer_disable_int_top(intfnum);
        timer_set_cmr_ch(intfnum, 0, 0);
        timer_set_cmr_ch(intfnum, 1, 0);
    }
#endif

#endif

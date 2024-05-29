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

#ifndef TIMER_IMPL_H
#define TIMER_IMPL_H

#include <stdint.h>
#include <avr/io.h>
#include <platform/types.h>
#include <platform/debug.h>
#include "hal/uc/timer.h"

#ifdef uC_INCLUDE_TIMER_IFACE

typedef enum {
    TIMER_HWIF_UNDEF, 
    TIMER_HWIF_TIMER_8A, 
    TIMER_HWIF_TIMER_16,
    TIMER_HWIF_TIMER_8B,
} TIMER_HWIF_TYPE;

#if !defined(__AVR_ATmega328P__) \
    && !defined(__AVR_ATmega168P__) \
    && !defined(__AVR_ATmega88P__)
    #warning "TIMER interface is untested for the chosen MCU."
#endif

#define uC_TIMERS_COUNT  3
#define uC_TIMER0_BASE   0x44
#define uC_TIMER1_BASE   0x80
#define uC_TIMER2_BASE   0xB0

#define uC_TIMER0_TYPE   TIMER_HWIF_TIMER_8A
#define uC_TIMER1_TYPE   TIMER_HWIF_TIMER_16
#define uC_TIMER2_TYPE   TIMER_HWIF_TIMER_8B

// Registers for timer interrupt controls. These need to be incremented by TC number. 
// The 328P and 328PB have them as blocks. Hopefully this is always the case. If it 
// isn't, these will need to go into the timer HWIF structs
#define uC_TIMER_TIFR_BASE  0x35
#define uC_TIMER_TIMSK_BASE 0x6E

// Registers for 16bit timers
#define OFS_TIMER_OCRxBH    0x0B
#define OFS_TIMER_OCRxBL    0x0A
#define OFS_TIMER_OCRxAH    0x09
#define OFS_TIMER_OCRxAL    0x08
#define OFS_TIMER_ICRxH     0x07
#define OFS_TIMER_ICRxL     0x06
#define OFS_TIMER_TCNTxH    0x05
#define OFS_TIMER_TCNTxL    0x04
#define OFS_TIMER_TCCRxC    0x02

// Registers for 8bit timers
#define OFS_TIMER_OCRxB     0x04
#define OFS_TIMER_OCRxA     0x03
#define OFS_TIMER_TCNTx     0x02

// Common Registers
#define OFS_TIMER_TCCRxB    0x01
#define OFS_TIMER_TCCRxA    0x00


#if uC_TIMER0_ENABLED
    #define uC_INCLUDE_TIMER_8A_IFACE
#endif

#if uC_TIMER1_ENABLED
    #define uC_INCLUDE_TIMER_16_IFACE
#endif

#if uC_TIMER2_ENABLED
    #define uC_INCLUDE_TIMER_8B_IFACE
#endif
    

typedef enum {
    TIMER_MODE_STOPPED, 
    TIMER_MODE_PERIODIC,
    TIMER_MODE_COUNTING,  
    // TIMER_MODE_UPDOWN,    
} TIMER_MODE;

typedef enum {
    TIMER_PRESCALER_DIV1    = 1, 
    TIMER_PRESCALER_DIV8    = 8,
    TIMER_PRESCALER_DIV32   = 32,   // Only on 8B
    TIMER_PRESCALER_DIV64   = 64, 
    TIMER_PRESCALER_DIV128  = 128,  // Only on 8B
    TIMER_PRESCALER_DIV256  = 256,
    TIMER_PRESCALER_DIV1024 = 1024,
} TIMER_PRESCALER;

typedef enum {
    TIMER_OUTMODE_PWM,      // Reset/Set
    TIMER_OUTMODE_NPWM,     // Set/Reset
    TIMER_OUTMODE_SET,      // Set,    NotImplemented
    TIMER_OUTMODE_RESET,    // Reset,  NotImplemented
    TIMER_OUTMODE_TOGGLE,   // Toggle, NotImplemented
    TIMER_OUTMODE_BIT,      // Bit,    NotImplemented
} TIMER_OUTMODE;

typedef struct _TIMER_HWIF_t{
    const TIMER_HWIF_TYPE type;
    const HAL_ADDRESS_t base;
    // const HAL_BASE_t primary_vector;
    // const HAL_BASE_t secondary_vector;
}_timer_hwif_t;

typedef struct _TIMER_STATE_t{
    TIMER_MODE mode;
    TIMER_PRESCALER prescaler;
}timer_state_t;

typedef struct TIMER_IF_t{
    const _timer_hwif_t *const hwif;
    timer_state_t *const state;
}timer_if_t;

extern const timer_if_t *const timer_if[];


#if uC_TIMER0_ENABLED
    extern const timer_if_t timer0_if;
#endif
#if uC_TIMER1_ENABLED
    extern const timer_if_t timer1_if;
#endif
#if uC_TIMER2_ENABLED
    extern const timer_if_t timer2_if;
#endif


#ifdef uC_INCLUDE_TIMER_8A_IFACE
void timer_8A_init(uint8_t intfnum);
#endif

#ifdef uC_INCLUDE_TIMER_8B_IFACE
void timer_16_init(uint8_t intfnum);
#endif

#ifdef uC_INCLUDE_TIMER_8B_IFACE
void timer_8B_init(uint8_t intfnum);
#endif


static inline void timer_init(uint8_t intfnum){
    if (timer_if[intfnum]->hwif->type == TIMER_HWIF_UNDEF){
        ;
    }
    #ifdef uC_INCLUDE_TIMER_8A_IFACE
    else if (timer_if[intfnum]->hwif->type == TIMER_HWIF_TIMER_8A){
        timer_8A_init(intfnum);
    }
    #endif
    #ifdef uC_INCLUDE_TIMER_8B_IFACE
    else if(timer_if[intfnum]->hwif->type == TIMER_HWIF_TIMER_8B){
        timer_8B_init(intfnum);
    }
    #endif
    #ifdef uC_INCLUDE_TIMER_16_IFACE
    else if(timer_if[intfnum]->hwif->type == TIMER_HWIF_TIMER_16){
        timer_16_init(intfnum);
    }
    #endif
}

static inline void _timer_configure_clock(uint8_t intfnum);

static inline void _timer_configure_clock(uint8_t intfnum) {
    const timer_if_t *timer = timer_if[intfnum];
    uint8_t cs = 0;
    uint8_t wgm = 0;

    TIMER_HWIF_TYPE type = timer->hwif->type;
    TIMER_PRESCALER prescaler = timer->state->prescaler;
    TIMER_MODE mode = timer->state->mode;

    uint8_t base = timer->hwif->base;
    uint8_t cs_mask = ~((1<<CS00) | (1<<CS01) | (1<<CS02));
    
    uint8_t tca_wgm_mask = ~((1<<WGM10) | (1<<WGM11));
    uint8_t tcb_wgm_mask = ~((1<<WGM12) | (1<<WGM13));

    if (mode == TIMER_MODE_STOPPED) {
        HWREG8(base + OFS_TIMER_TCCRxB) &= cs_mask;
        return;
    }; 
    
    // WGM
    switch (type) {
        case TIMER_HWIF_TIMER_8A:
        case TIMER_HWIF_TIMER_8B:
            switch (mode) {
                case TIMER_MODE_COUNTING:
                    wgm = 0b000;  // Normal
                    break;
                case TIMER_MODE_PERIODIC:
                    wgm = 0b010;  // CTC
                    break;
                default:
                    die();
                    wgm = 0b000;
                    break;
            }
            break;
        case TIMER_HWIF_TIMER_16:
            switch (mode) {
                case TIMER_MODE_COUNTING:
                    wgm = 0b0000;  // Normal
                    break;
                case TIMER_MODE_PERIODIC:
                    wgm = 0b0100;  // CTC
                    break;
                default:
                    die();
                    wgm = 0b0000;
                    break;
            }
            break;
        default:
            die();
            wgm = 0b000;
            break;
    }

    // CS
    switch (type) {
        case TIMER_HWIF_TIMER_8A:
        case TIMER_HWIF_TIMER_16:
            switch (prescaler) {
                case TIMER_PRESCALER_DIV1:
                    cs = 0b001;
                    break;
                case TIMER_PRESCALER_DIV8:
                    cs = 0b010;
                    break;
                case TIMER_PRESCALER_DIV64:
                    cs = 0b011;
                    break;
                case TIMER_PRESCALER_DIV256:
                    cs = 0b100;
                    break;
                case TIMER_PRESCALER_DIV1024:
                    cs = 0b101;
                    break;
                default:
                    die();
                    cs = 0b000;
                    break;
            }
            break;
        case TIMER_HWIF_TIMER_8B:
            switch (prescaler) {
                case TIMER_PRESCALER_DIV1:
                    cs = 0b001;
                    break;
                case TIMER_PRESCALER_DIV8:
                    cs = 0b010;
                    break;
                case TIMER_PRESCALER_DIV32:
                    cs = 0b011;
                    break;
                case TIMER_PRESCALER_DIV64:
                    cs = 0b100;
                    break;
                case TIMER_PRESCALER_DIV128:
                    cs = 0b101;
                    break;
                case TIMER_PRESCALER_DIV256:
                    cs = 0b110;
                    break;
                case TIMER_PRESCALER_DIV1024:
                    cs = 0b111;
                default:
                    die();
                    cs = 0b000;
                    break;
            }
            break;
        default:
            cs = 0b000;
            die();
            break;
    }

    uint8_t tcb_mask = cs_mask | tcb_wgm_mask;
    HWREG8(base + OFS_TIMER_TCCRxA) = (HWREG8(base + OFS_TIMER_TCCRxA) & tca_wgm_mask) | (wgm & 0b11); 
    HWREG8(base + OFS_TIMER_TCCRxB) = (HWREG8(base + OFS_TIMER_TCCRxB) & tcb_mask) | (cs | ((wgm & 0b1100) << 1)); 
}

#define TIMER0_UNSUPPORTED_PRESCALERS (TIMER_PRESCALER_DIV32 | TIMER_PRESCALER_DIV128)
#define TIMER1_UNSUPPORTED_PRESCALERS (TIMER_PRESCALER_DIV32 | TIMER_PRESCALER_DIV128)
#define TIMER2_UNSUPPORTED_PRESCALERS ()

#if defined(__GNUC__) || defined(__clang__)
#define DIAGNOSE_IF_UNSUPPORTED(intfnum, prescaler) \
    __attribute__((diagnose_if(((intfnum == 0 && (prescaler == TIMER_PRESCALER_DIV32 || prescaler == TIMER_PRESCALER_DIV128)) || \
                                (intfnum == 1 && (prescaler == TIMER_PRESCALER_DIV32 || prescaler == TIMER_PRESCALER_DIV128))), \
                                "Unsupported prescaler value for the specified timer", "warning")))
#else
#define DIAGNOSE_IF_UNSUPPORTED(intfnum, prescaler)
#endif
static inline void timer_set_prescaler(uint8_t intfnum, uint16_t prescaler){
    timer_if[intfnum]->state->prescaler = prescaler;
    _timer_configure_clock(intfnum);
}

static inline void timer_set_mode(uint8_t intfnum, uint8_t mode){
    timer_if[intfnum]->state->mode = mode;
    _timer_configure_clock(intfnum);
}

static inline void timer_enable_int_overflow(uint8_t intfnum){
    HWREG8(uC_TIMER_TIFR_BASE + intfnum) |= (1<<TOV0);
    HWREG8(uC_TIMER_TIMSK_BASE + intfnum) |= (1<<TOIE0);
};

static inline void timer_disable_int_overflow(uint8_t intfnum){
    HWREG8(uC_TIMER_TIMSK_BASE + intfnum) &= ~(1<<TOIE0);
    HWREG8(uC_TIMER_TIFR_BASE + intfnum) |= (1<<TOV0);
};

static inline void timer_enable_int_ch (uint8_t intfnum, uint8_t channel){
    HWREG8(uC_TIMER_TIFR_BASE + intfnum) |= (1<< (OCF0A+channel));
    HWREG8(uC_TIMER_TIMSK_BASE + intfnum) |= (1<< (OCIE0A+channel));
}

static inline void timer_disable_int_ch(uint8_t intfnum, uint8_t channel){
    HWREG8(uC_TIMER_TIFR_BASE + intfnum) |= (1<<(OCF0A+channel));
    HWREG8(uC_TIMER_TIMSK_BASE + intfnum) &= ~(1<<(OCIE0A+channel));
}

static inline uint16_t _chocr_offset(uint8_t intfnum, uint8_t channel);

static inline uint16_t _chocr_offset(uint8_t intfnum, uint8_t channel){
    const timer_if_t * timer = timer_if[intfnum];
    switch (timer->hwif->type) {
        case TIMER_HWIF_TIMER_16:
            return (0x08 + channel * 2);
        case TIMER_HWIF_TIMER_8A:
        case TIMER_HWIF_TIMER_8B:
            return (0x03 + channel * 1);
        default:
            die();
            return(0x00);
    }
}

static inline void timer_set_cmr_ch(uint8_t intfnum, uint8_t channel, uint16_t cm){
    const timer_if_t * timer = timer_if[intfnum];
    
    if (timer->hwif->type == TIMER_HWIF_TIMER_16) {
        HWREG8(timer->hwif->base + _chocr_offset(intfnum, channel)) = (uint8_t)(cm);
        HWREG8(timer->hwif->base + _chocr_offset(intfnum, channel) + 1) = (uint8_t)(cm >> 8);
    } else {
        HWREG8(timer->hwif->base + _chocr_offset(intfnum, channel)) = (uint8_t)(cm);
    }
}

static inline uint16_t timer_get_cmr_ch(uint8_t intfnum, uint8_t channel){
    const timer_if_t * timer = timer_if[intfnum];
    if (timer->hwif->type == TIMER_HWIF_TIMER_16) {
        return (
            (uint16_t)HWREG8(timer->hwif->base + _chocr_offset(intfnum, channel)) |
            ((uint16_t)HWREG8(timer->hwif->base + _chocr_offset(intfnum, channel) + 1) << 8) 
        );
    } else {
        return (uint16_t)HWREG8(timer->hwif->base + _chocr_offset(intfnum, channel));
    }
}


static inline void timer_set_top(uint8_t intfnum, uint16_t top){
    const timer_if_t * timer = timer_if[intfnum];
    const TIMER_MODE mode = timer -> state -> mode;
    
    if (mode == TIMER_MODE_COUNTING){
        return;
    }
    else{
        timer_set_cmr_ch(intfnum, 0, top);
    }
}

static inline uint16_t timer_get_top(uint8_t intfnum){
    const timer_if_t * timer = timer_if[intfnum];
    const TIMER_MODE mode = timer -> state -> mode;
    if (mode == TIMER_MODE_COUNTING){
        if (timer->hwif->type == TIMER_HWIF_TIMER_16) {
            return (0xFFFF);
        } else {
            return (0xFF);
        }
    }
    else{
        return timer_get_cmr_ch(intfnum, 0);
    }
}

static inline void timer_enable_int_top(uint8_t intfnum){
    const timer_if_t * timer = timer_if[intfnum];
    const TIMER_MODE mode = timer -> state -> mode;

    if (mode == TIMER_MODE_COUNTING) { 
        timer_enable_int_overflow(intfnum);    
    } else {
        timer_enable_int_ch(intfnum, 0);
    }
};

static inline void timer_disable_int_top(uint8_t intfnum){
    const timer_if_t * timer = timer_if[intfnum];
    const TIMER_MODE mode = timer -> state -> mode;

    if (mode == TIMER_MODE_COUNTING) { 
        timer_disable_int_overflow(intfnum);    
    } else {
        timer_disable_int_ch(intfnum, 0);
    }
};

static inline void timer_set_outmode_ch(uint8_t intfnum, uint8_t channel, uint8_t outmode){
    // uint16_t loutmode;
    // switch (outmode){
    //     case TIMER_OUTMODE_PWM:
    //         loutmode = OUTMOD_7;
    //         break;
    //     case TIMER_OUTMODE_NPWM:
    //         loutmode = OUTMOD_3;
    //         break;
    //     default:
    //         loutmode = OUTMOD_0;
    //         break;
    // }
    // HWREG16(timer_if[intfnum]->hwif->base + timer_chctl_offset(channel)) &= ~(OUTMOD0 | OUTMOD1 | OUTMOD2);
    // HWREG16(timer_if[intfnum]->hwif->base + timer_chctl_offset(channel)) |= (loutmode | OUT);
}

#endif

#endif

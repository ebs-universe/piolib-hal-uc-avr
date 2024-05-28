/* 
   Copyright (c) 
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : MSP430
   
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


#include <hal/uc/uart.h>
#include "gpio_impl.h"


#ifdef uC_INCLUDE_UART_IFACE

#if uC_UART0_ENABLED

    // Notes: 
    //   - Technically, the AVR does have clock source selection in the form 
    //     of sync uart mode. We aren't implementing that here, but we should.
    
    // BAUD RATE GENERATION
    #define UART0_UBRR    ((F_CPU / (16UL * uC_UART0_BAUD)) - 1)
    #define UART0_UBRRH_VALUE ((UART0_UBRR >> 8) & 0xFF)
    #define UART0_UBRRL_VALUE (UART0_UBRR & 0xFF)
    
    uint8_t uart0_txbuffer[uC_UART0_TXBUF_LEN + 1];
    uint8_t uart0_rxbuffer[uC_UART0_RXBUF_LEN + 1];
    bytebuf uart0_txbuf;
    bytebuf uart0_rxbuf;
    uint16_t uart0_default_overrun_counter = 0;
    
    static const _uart_hwif_t _uart0_hwif = {
        UART_HWIF_AVR, uC_UART0_BASE
    };
    
    uart_state_t uart0_state = {0, &uart0_default_overrun_counter};
    
    const uart_if_t uart0_if = {
        &_uart0_hwif,
        &uart0_state,
        &uart0_txbuf,
        &uart0_rxbuf,
    };
    
    void _uart0_init(void);
    static inline void _uart0_buffer_init(void);
    static inline void _uart0_gpio_init(void);
    static inline void _uart0_peripheral_init(void);

    static inline void _uart0_buffer_init(void){
        bytebuf_vInit(uart0_txbuffer, uC_UART0_TXBUF_LEN, &uart0_txbuf);
        bytebuf_vInit(uart0_rxbuffer, uC_UART0_RXBUF_LEN, &uart0_rxbuf);
    }
    
    static inline void _uart0_gpio_init(void){
        gpio_conf_output(uC_PD, 0);
        gpio_conf_input(uC_PC, 1);
    }

    static inline void _uart0_peripheral_init(void){
        // Set baud rate
        HWREG8(uC_UART0_BASE + OFS_UART_UBRR0H) = UART0_UBRRH_VALUE;
        HWREG8(uC_UART0_BASE + OFS_UART_UBRR0L) = UART0_UBRRL_VALUE;
        
        // Set frame format: 8 data bits, 1 stop bit, no parity
        HWREG8(uC_UART0_BASE + OFS_UART_UCSR0C) = (1 << UCSZ01) | (1 << UCSZ00);
        
        // Enable transmitter and receiver
        HWREG8(uC_UART0_BASE + OFS_UART_UCSR0B) = (1 << TXEN0) | (1 << RXEN0);
        
        // Clear USART RXC and UDRE interrupt flags
        HWREG8(uC_UART0_BASE + OFS_UART_UCSR0A) |= (1 << RXC0) | (1 << UDRE0);
    
        // Enable the USART RXC and UDRE interrupts
        HWREG8(uC_UART0_BASE + OFS_UART_UCSR0B) |= (1 << RXCIE0 | ((1 << UDRIE0)));
    }
    
    void _uart0_init(void){
        _uart0_buffer_init();
        _uart0_gpio_init();        
        _uart0_peripheral_init();
    }
#endif
    

const uart_if_t *const uart_if[1] = {
    #if uC_UART0_ENABLED
        &uart0_if
    #else
        NULL
    #endif
};

void (*const _uart_init_func[1])(void) = {
    #if uC_UART0_ENABLED
        _uart0_init
    #else
        NULL
    #endif
};

void uart_init(uint8_t intfnum){
    (*_uart_init_func[intfnum])();
}

uint8_t uart_vprintf(uint8_t intfnum, const char *format, ...){
        uint8_t stat, lstat;
        va_list args;
        va_start(args, format);
        stat = bytebuf_cPushReqBlindLock(uart_if[intfnum]->txbuf, BYTEBUF_TOKEN_PRINT);
        if (stat){
            stat = vbprintf(uart_if[intfnum]->txbuf, format, args);
            va_end(args);
            lstat = bytebuf_cPushRelinquishLock(uart_if[intfnum]->txbuf, BYTEBUF_TOKEN_PRINT);
            uart_send_trigger(intfnum);
            if (lstat){
                return stat;
            }
            else{
                return 0;
            }
        }
        else{
            va_end(args);
            return 0;
        }
}

static uint8_t _uart_population_rxb(uint8_t intfnum){
    return uart_population_rxb(intfnum);
}

static uint8_t _uart_read(uint8_t intfnum, uint8_t * buffer, uint8_t len){
    return uart_read(intfnum, buffer, len);
}

static uint8_t _uart_reqlock(uint8_t intfnum, uint8_t len, uint8_t token){
    return uart_reqlock(intfnum, len, token);
}

static uint8_t _uart_rellock(uint8_t intfnum, uint8_t token){
    return uart_rellock(intfnum, token);
}

static uint8_t _uart_write(uint8_t intfnum, uint8_t * buffer, uint8_t len, uint8_t token){
    return uart_write(intfnum, buffer, len, token);
}

const pluggable_transport_t ptransport_uart = {
    &uart_init,
    &_uart_population_rxb,
    &_uart_read,
    &_uart_reqlock,
    &_uart_rellock,
    &_uart_write,
    &uart_send_trigger
};

#endif
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

#include <ds/bytebuf.h>
#include <hal/uc/uart.h>
#include <platform/types.h>
#include <avr/interrupt.h>


volatile uint8_t __uart_handler_inclusion;

    #if uC_UART0_ENABLED
    /**
     * @brief UART0 TX interrupt handler
     * 
     * Interrupt handler function for TX Complete on UART0.
     * 
     * Called when UART transmission is complete. Application code may need to change
     * this function.
     * 
     */
    static inline void uart0_handler_tx(void);

    /**
     * @brief UART1 RX interrupt handler
     * 
     * Interrupt handler function for RX Complete on UART1.
     * 
     * Called when UART reception is complete. Application code may need to change
     * this function.
     * 
     */
    static inline void uart0_handler_rx(void);


    static inline void uart0_handler_rx(void){
        uint8_t rval; 
        rval = HWREG8(uC_UART0_BASE + OFS_UART_UDR0);
        if (bytebuf_cGetFree(&uart0_rxbuf)){
            bytebuf_cPushByte(&uart0_rxbuf, rval, 0x00);
        }
        else{
            (*uart0_if.state->overrun_counter)++;
        }
    }
    
    static inline void uart0_handler_tx(void){
        uint8_t waiting = bytebuf_cPopulation(&uart0_txbuf);
        if (waiting <= 1){
            // Disable the UDRE interrupt
            HWREG8(uC_UART0_BASE + OFS_UART_UCSR0B) &= ~(1 << UDRIE0);
            uart0_if.state->triggered = 0;
        }
        if (waiting) {
            HWREG8(uC_UART0_BASE + OFS_UART_UDR0) = bytebuf_cPopByte(&uart0_txbuf);
        }
    }

    ISR(USART_RX_vect) {
        uart0_handler_rx();
    }

    ISR(USART_UDRE_vect) {
        uart0_handler_tx();
    }

    #endif


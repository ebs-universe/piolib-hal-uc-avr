/* 
   Copyright 2015 Quazar Technologies Pvt. Ltd.
   Copyright 2015 Chintalagiri Shashank
   
   This file is part of
   Embedded bootstraps : peripheral usage maps : TI Launchpad MSP420F5529
   
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

/**
 * @file uc_map.h
 * @brief uC Peripheral Map for AVR MCUs
 *
 * This file contains the peripheral map for the uC. This map was written 
 * in the context of the Arduino Nano based on the ATMega328P. Ideally, 
 * we'd want to generalize this file to other MCUs as they get used. 
 */

#ifndef uC_MAP
#define uC_MAP

#include <application.h>


/**
 * @name UART Configuration
 */
/**@{*/
    #if (uC_UART0_ENABLED) > 0
        #define uC_INCLUDE_UART_IFACE
    #endif
/**@}*/ 

/**
 * @name SPI Configuration
 */
/**@{*/
    #if (uC_SPI0_ENABLED) > 0
        #define uC_INCLUDE_SPI_IFACE
    #endif
/**@}*/ 

/**
 * @name TIMER Configuration
 */
/**@{*/
    #if (uC_TIMER0_ENABLED + uC_TIMER2_ENABLED) > 0
        #define uC_INCLUDE_TIMER_8_IFACE
    #endif
    #if (uC_TIMER1_ENABLED) > 0
        #define uC_INCLUDE_TIMER_16_IFACE
    #endif
    #if (uC_TIMER0_ENABLED + uC_TIMER1_ENABLED + uC_TIMER2_ENABLED) > 0
        #define uC_INCLUDE_TIMER_IFACE
    #endif
/**@}*/ 

#endif

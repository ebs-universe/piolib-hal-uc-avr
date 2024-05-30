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


#include "id_impl.h"
#include <rand/rand.h>

#if APP_ENABLE_ID && APP_ID_PROVIDER == 1

void id_init(void){
    #if APP_ID_AUTOGENERATE
    uint8_t state = eeprom_read(uC_ID_STATE_ADDR);
    
    if (state == uC_ID_STATE_DEFAULT) {
        uint8_t i = 0;
        uint8_t random_id[uC_ID_SIZE];
        while (i < uC_ID_SIZE) {
            random_id[i] = rand_byte();
            i++;
        }
        eeprom_write_buffer(uC_ID_ADDR, random_id, uC_ID_SIZE);
        eeprom_write(uC_ID_STATE_ADDR, uC_ID_STATE_RANDOM);
    }
    #endif
}

uint8_t id_read(uint8_t maxlen, void * buffer){
    uint8_t state = eeprom_read(uC_ID_STATE_ADDR);

    if (state == uC_ID_STATE_DEFAULT) {
        return 0; 
    }

    uint8_t len = (maxlen < uC_ID_SIZE) ? maxlen : uC_ID_SIZE;
    eeprom_read_buffer(uC_ID_ADDR, (uint8_t*)buffer, len);
    return len;
}

uint8_t id_write(uint8_t len, uint8_t * content){
    #if APP_ID_ACCEPT_WRITE
    uint8_t state = eeprom_read(uC_ID_STATE_ADDR);

    if (state == uC_ID_STATE_USER) {
        return 0; 
    }

    if (len > uC_ID_SIZE) {
        len = uC_ID_SIZE;
    }

    eeprom_write_buffer(uC_ID_ADDR, content, len);
    eeprom_write(uC_ID_STATE_ADDR, uC_ID_STATE_USER);
    return len;
    #endif
}

#endif
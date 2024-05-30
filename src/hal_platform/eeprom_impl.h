
#ifndef EEPROM_IMPL_H
#define EEPROM_IMPL_H

#include <avr/eeprom.h>

#define EEPROM_SIZE (E2END + 1)


static inline void eeprom_init(void){
    ;
}

static inline uint8_t eeprom_ready(void){
    return eeprom_is_ready();
}

static inline void eeprom_write(uint16_t address, uint8_t data) {
    while (!eeprom_is_ready());
    eeprom_update_byte((uint8_t*)address, data);
}

static inline uint8_t eeprom_read(uint16_t address) {
    while (!eeprom_is_ready());
    return eeprom_read_byte((const uint8_t*)address);
}

static inline void eeprom_write_buffer(uint16_t address, const uint8_t *data, uint8_t length) {
    while (!eeprom_is_ready());
    eeprom_update_block((const void*)data, (void*)address, length);
}

static inline void eeprom_read_buffer(uint16_t address, uint8_t *data, uint8_t length) {
    while (!eeprom_is_ready());
    eeprom_read_block((void*)data, (const void*)address, length);
}

#endif
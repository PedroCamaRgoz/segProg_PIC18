#ifndef EEPROM_H
#define EEPROM_H

#define LIMPO    0

char EEPROM_read( unsigned char addr );
void EEPROM_write( unsigned char addr, unsigned char data );

#endif

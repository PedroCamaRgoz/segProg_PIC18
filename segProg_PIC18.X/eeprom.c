/*
 * File:   eeprom.c
 * Author: curtaeletronica
 *
 * Created on 10 de Janeiro de 2020, 16:34
 * Vers�o de biblioteca : eeprom 1
 */

#include <xc.h>
#include "eeprom.h"

char EEPROM_read( unsigned char addr )
{
    
    EEADR = addr;         // endere�o 
    EECON1bits.EEPGD = 0;      // acessa memoria de dado
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;         // inicia leitura de memoria
    return( EEDATA );           // retorna 
      
}

void EEPROM_write( unsigned char addr, unsigned char data )
{
    INTCONbits.GIE = 0;
    EEADR = addr;               // endere�o 
    EEDATA = data;               // dados
    EECON1bits.EEPGD = 0;       // acessa memoria de dado
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;        // habilita grava��o
    EECON2 = 0x55;              // 
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    
    while( EECON1bits.WR );  // enquanto ciclo de grava��o 
    
    EECON1bits.WREN = 0; // desabilita grava��o
    INTCONbits.GIE = 1; // desabilita todas as interrup��es 
}

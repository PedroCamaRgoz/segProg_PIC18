/*
 * File:   main.c
 * Author: TCC ( ALICE, LEMES, PEDRO, CARLOS, DAVI ) - 2021
 *
 * Created on 16 de Novembro de 2021, 15:18
 */

#include "config.h"
#include <xc.h>
#include "me.h"
#include "meTELAS.h"
#include "teclado.h"
#include "timer1.h"
#include "meEXEC.h"
#include "IHM.h"
#include "sequencia.h"
#include "serialIO.h"
#include "i2c.h"
#include "MCP4725.h"

void __interrupt() irq(void)
{
    INTCONbits.GIE = 0;
    if(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
    {
        PIR1bits.TMR1IF = 0;
         intt1_1ms();
    }
    INTCONbits.GIE = 1;    
}

void main(void)
{
   teclado_init();
   timer1_init();
    
   tela.func = me_telas_init;
   execucao.func = me_exec_vazio;

    while(1)
    {
       serialIOscan();
       teclado_borda();
       tela.func = tela.func( tela.arg, tela.next);
       execucao.func = execucao.func( execucao.arg, execucao.next );               
    }
}

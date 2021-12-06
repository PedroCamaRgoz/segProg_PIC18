/*
 * File:   meEXEC.c
 * Author: 20187263
 *
 * Created on 16 de Novembro de 2021, 16:45
 */


#include <xc.h>
#include "me.h"
#include "meTELAS.h"
#include "timer1.h"
#include "IHM.h"
#include "delay.h"
#include "fifo.h"
#include "estados.h"
#include "MCP4725.h"
#include "teclado.h"
#include "atuadores.h"
#include "meEXEC.h"
#include "serialIO.h"
#include "i2c.h"
#include "timer1.h"

struct fsmT execucao;

#define STOP   0
#define PLAY   1
#define PAUSE  2
#define STEP   3
#define CANCELAR '*'


char auxPasso = 0;
char  vetorIn[1] = { 0 };
char vetorOut[1] = { 0 };
char btt;


void * me_exec_vazio ( void * arg, void * next )
{
   
   // execucao.next = me_exec_vazio;
    return( me_exec_vazio );
    
}

void * me_exec_init ( void * arg, void * next )
{
  
    I2C_init(20000000);
    resetContCiclos();
    resetIndicePassos();
    initSerialIO( vetorIn , vetorOut,1);

    return( me_exec_botoes );
}

void * me_exec_botoes ( void * arg , void * next )
{
    
    auxPasso = fifo_lerPos(getIndicePassos());
            
    if( btt == PLAY || btt == STEP || btt == STOP )
        return( me_exec_principal );        
    
    return( me_exec_botoes );
}

void * me_exec_principal ( void * arg, void * next )
{
    if(auxPasso <= 0x64 || auxPasso == 0xFE)
    {
        decodifica(&auxPasso);
        MCP4725((long)((4095L * auxPasso)/100));
        return( me_exec_ler_step );
    } 
    else if( auxPasso > 0x64 && auxPasso <= 0xDC )   
    {
        auxPasso -= 0x64;
        setT1(auxPasso  * 1000 );
        IHM.printv(1,0,auxPasso,5);
      //  execucao.arg = me_exec_ler_step ;
        return( me_exec_delay );
    }
    else
    {
        decodifica(&auxPasso);
        set_passo(auxPasso, vetorOut);
        
        return( me_exec_ler_sensor );
    }
    
//    if( btt == CANCELAR )
//    {
//        resetIndicePassos();
//        return( me_exec_vazio );
//    }
//    
    return( me_exec_principal );
}

void * me_exec_ler_sensor (void * arg, void * next )
{
    if( ler_sensor( auxPasso, vetorIn ))
           return( me_exec_ler_step );
    
    return( me_exec_ler_sensor ); 
}

void * me_exec_delay ( void * arg, void * next )
{
    if( !statusT1() )
        return( me_exec_ler_step );
     
                
    return( me_exec_delay);
}

void * me_exec_ler_step (void * arg, void * next )
{
  
        if( btt == STEP )
        {
            btt = PAUSE;
        }
        
         return( me_exec_indice );

  return( me_exec_ler_step );
}

//void * me_exec_ler_sensor (void * arg, void * next )
//{
//    if( ler_sensor( auxPasso, vetorIn ))
//    {
//    
//        if( btt == STEP )
//        {
//            btt = PAUSE;
//        }
//        
//      return( me_exec_indice );
//    }
//    return( me_exec_ler_sensor );
//}

void * me_exec_indice ( void * arg, void *  next )
{
    addIndicePassos();
    if( getIndicePassos() < fifo_indice() )
        return ( me_exec_botoes );
    else 
    {
        addContCiclos();
        return( me_exec_ciclos );
    }
}

void * me_exec_ciclos ( void * arg , void * next )
{
    if( ( btt != STOP ) && ((getContCiclos() < getCiclos()) || ( getCiclos() == 0) ) )
    {
        resetIndicePassos();
        return( me_exec_botoes );
    }
    
    else
    {
        MCP4725(0);
        return ( me_exec_vazio );
    }
}


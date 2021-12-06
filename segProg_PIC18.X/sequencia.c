/*
 * Created on 26 de Outubro de 2021, 17:36
 */

#include "config.h"
#include <xc.h>
#include "eeprom.h"
#include "sequencia.h"
//#include "fifo.h"
#include "dispLCD4vias.h"
#include "delay.h"
#include "IHM.h"

struct sequencia_t seq = {salvar_receita, buscar_receita, salvar_dado, buscar_dado, deletar_receita,receita_livre, formatar_receitas, print_receita_livre};

int end_receita (int x)
{
    return(x*16);                            // retorna o endereço da primeira posição das receitas 0 a 16
}

void salvar_receita ( unsigned char adrs, unsigned char *data )
{
    int  aux;
    aux = end_receita(adrs);
    for( char i=0 ; i<16; i++)
         EEPROM_write( (aux+i), *(data+i) );  // aux + i nos dá a posição da primeira coluna de cada receita, 
                                              // *data + i nos dá as posições do  endereço da vetor atrelado ao ponteiro *data 
}

void buscar_receita (unsigned char adrs, unsigned char * data )  // adrs = busca receitas 0 a 16, * data = recebe receita buscada e aponta para o vetor
{
    int aux;
    aux = end_receita(adrs);
    for( char i=0; i<16; i++ )
       *(data+i) = EEPROM_read(aux+i);       // *data+i recebe o endereço de eeprom nas posições aux+i 
}

void salvar_dado (char adrs, char col, unsigned char data) //funcao criada para salvar um unico dado de qualquer endereco de memoria da EEPROM
{
    int  aux;
    aux = end_receita(adrs);
    EEPROM_write((aux+col), data);
}

void buscar_dado (char adrs, char col, unsigned char *data) //funcao criada para buscar um unico dado de qualquer endereco de memoria da EEPROM
{
    int aux;
    aux = end_receita(adrs);
    *data = EEPROM_read((aux+col));
}

void deletar_receita (char receita)
{
   int  aux;
   aux = end_receita(receita);
   for( char i=0 ; i<16; i++)
        EEPROM_write((aux+i),LIMPO);        //
}

void receita_livre ( unsigned char * rec)
{
   unsigned int aux;
   for(char i=0;i<10;i++)
   {
       aux = end_receita(i);
       if(EEPROM_read(aux) == LIMPO)            // testa se a primeira posição de cada receita é igual a 0
       { 
          if(i<10)                             // separa os caracteres '0' a '9' dos caracteres 'A' a 'F' 
              *(rec + i) = '0'+i;              // '0' + i retorna as strings numericas de '0' a '9'    
          else                                 // separa os caracteres 'A' a 'F' dos '0' a '9'
              *(rec + i) = 'A'-10+i;           // 'A'-10 + i retorna a string alfabetica de 'A' a 'F'
       }
   }
             
}

//void formatar_receitas (void)
//{
//    for( char i = 0;i<256;i++)                 // Faz toda memoria eeprom igual a 0
//           EEPROM_write(i,LIMPO);
//   
//}

char formatar_receitas ( char x, char y )
{
    char cont = 0;
    
    for( char i = x ; i < y ;i++)                 // Faz toda memoria eeprom igual a 0
        EEPROM_write(i,LIMPO);
    
    cont = y;
    return( cont );
}




const char list[20]="0 1 2 3 4 5 6 7 8 9 ";
void print_receita_livre ( unsigned char * newline )            // imprime receitas com espaços entre elas e imprime '-' se a receita nessa posição estiver ocupada
{
    unsigned char   aux[10]="          ";
    unsigned char right[20]="                    ";
    
    receita_livre(aux);
    
    for(char i = 0; i < 20; i++)
    {
        if(list[2*i] == aux[i])
            *(newline +(2*i)) = list[2*i];
        else 
            *(newline+(2*i)) = '_';
    }

}

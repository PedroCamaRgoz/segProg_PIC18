/* 
 * File:   fifo.h
 * Author: luis420516@gmail.com
 *
 * Created on 9/6/2020 10:15:56 PM UTC
 * "Created in MPLAB Xpress"
 */

#ifndef FIFO_H
#define	FIFO_H


char ler_atuador(char tecla);
void alt_atuador( char tecla );
void init_atuadores(void);                                 /*INICIA OS ATUADORES RECUADOS*/
void fifo_reset(void);                                     //
char fifo_lerPos(char i);                                  //
void fifo_modifica(char pos, char valor);                  //
char * fifo_adrs(void);
void fifo_delete(void);                                    //DELETA O ÚLTIMO PASSO PRESSIONADO PELO USUARIO
void fifo_add(const char passo);                           //ADICIONA OS PASSOS NA FILA
void fifo_add_tempo(const char t);
char fifo_indice(void);                                    //RETORNA O INDICE DA FILA
char fifo_disponivel( void );                              //VERFICA SE HÁ ESPAÇO DISPONIVEL NA FILA
char fifo_tam(void);                                       //RETORNA O TAMANHO DO VETOR
void fifo_indicePrint_inc( void );                         //MOVE A TELA DA SEQUÊNCIA PARA DIREITA
void fifo_indicePrint_dec( void );                         //MOVE A TELA DA SEQUÊNCIA PARA ESQUERDA
void fifo_print(void);                                     //IMPRIME O VALOR PRESSIONADO PELA TECLA
void dignum_conc(char dig, unsigned int * ptrNum );        //
void dignum_apagar(unsigned int * ptrNum );                //
unsigned char dignum_tam(int num);                         //
void fifo_add_ciclo(unsigned int ciclos);                  //
unsigned int getCiclos(void);                              //
unsigned int getContCiclos(void);                          //RETORNA O VALOR DA VARÁVEL "cont_ciclos"
void resetContCiclos(void);
void addContCiclos(void);
void addIndicePassos(void);
void decIndicePassos(void);
unsigned char getIndicePassos(void);
void resetIndicePassos(void);
void fifo_alinharPrint( void );
char posAtual(char ind);
void printExec (char col, char ind);

void fifo_add_control(const char c);     //Funcao criada para adicionar a porcentagem de controle na fila - PENDENTE
char salva_atuadores_init( void );	     //Funcao criada para retornar a condicao inicial dos atuadores para cada sequencia criada
void manipula_atuadores_init(char aux);  //Funcao criada para manipular os atuadores conforme a condicao inicial buscada na memoria

void decodifica( unsigned char *d );
void codifica( unsigned char tipo, unsigned char *dado );
void fifo_print_reset ( void );  // add 2021

//
//struct atuador_t
//{
//    char (*ler) (char tecla);
//    void (*alterar) (char tecla);
//    void (*init) ( void );
//    char (*Salva_condInicial)( void );
//    void (*CondInicial)(char aux);
//    
//};
//extern struct atuador_t atuadores; 
//
//struct dignum_t
//{
//    void (*convString)(char dig, unsigned int * ptrNum );        
//    void (*apagar) (unsigned int * ptrNum );                
//    unsigned char ( *tamanho )(int num);        
//    
//}; 
//extern struct dignum_t dignum;
//
//struct ciclos_t
//{
//    unsigned int (*obter)(void);                              
//    unsigned int (*Contagem)(void);                          
//    void (*resetContagem )(void);
//    void (*addContagem )(void);
//        
//}; 
//extern struct ciclos_t ciclo;
//
//struct fifo_t
//{
//    void (*reset)(void);                                     
//    char (*lerPosicao)(char i);                                  
//    void (*modifica)(char pos, char valor);                  
//    //char (*adrs)(void);
//    void (*delete)(void);                                   
//    void (*adiciona)(const char passo);                          
//    void (*addTempo)(const char t);
//    char (*indice)(void);                                    
//    char (*disponivel)( void );                             
//    char (*tamanho)(void);                                     
//    void (*indicePrintInc)( void );                        
//    void (*indicePrintDec)( void );                       
//    void (*print)(void);                                                         
//    void (*alinharPrint)( void );
//    char (*posAtual)(char ind);
//    void (*printExec) (char col, char ind);
//    
//}; 
//extern struct fifo_t fifo;




#endif	/* FIFO_H */

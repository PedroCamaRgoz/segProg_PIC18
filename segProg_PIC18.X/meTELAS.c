/*
 * File:   meTELAS.c
 * Author: TCC ( ALICE, LEMES, PEDRO, CARLOS, DAVI ) - 2021
 *
 * Created on 16 de Novembro de 2021, 15:20
 */


#include <xc.h>
#include "me.h"
#include "meTELAS.h"
#include "teclado.h"
#include "timer1.h"
#include "delay.h"
#include "IHM.h"
#include "meEXEC.h"
#include "sequencia.h"
#include "eeprom.h"
#include "atuadores.h"
#include "fifo.h"
#include "serialIO.h"

//    EXPOSE
#define DELETE '*'
#define AVANCAR '#'
#define RETORNAR '*'
#define TEMPO '0'
#define SAIDA_ANALOG '9'
#define MEMORIA_ATDRS 12
#define CICLO_MAX 10000
#define SEG 120
#define PORCENT 100
#define NUMERO( X ) ( X >= '0' && X <= '9')

#define CONTROLE_V( x )  ( x  <= 0x64 || x == 0xFE )
#define TEMPO_V( x )      ( x  > 0x64 && x <= 0xDC )
#define SIM_NAO "# - SIM      * - NAO"

#define STOP   0
#define PLAY   1
#define PAUSE  2
#define STEP   3
#define CANCELAR '*'

const char       list[20]="0 1 2 3 4 5 6 7 8 9";
const char subscrever[20]="0 1 2 3 4 5 6 7 8 9 ";
unsigned char *ptr_aux;
char *ptr;
unsigned char vtr_aux[16];
unsigned char cond_inicial = 0;
unsigned int      segundos = 0;
unsigned int        numero = 0;
unsigned int        tensao = 0;
unsigned int        ciclos = 0;
unsigned char        modo  = 0;

unsigned char     aux[10]="          ";
unsigned char    aux1[10]="          ";
unsigned char   right[20]="                    ";
unsigned char config1[20]="                    ";
char            vetor[20]="                    ";

unsigned char teclado = 0;
char from = 0;
    
struct fsmT tela;



void ajuste_init ( void )
{
    dispLCD_lincol(3, 4); dispLCD_dataReg(ler_atuador('A') ? '+' : '-');    //Inserido para pegar o status atual dos atuadores apÃ³s retornar da tela posterior (TELA_CONTAR_PASSOS)
    dispLCD_lincol(3, 8); dispLCD_dataReg(ler_atuador('B') ? '+' : '-');
    dispLCD_lincol(3, 12);dispLCD_dataReg(ler_atuador('C') ? '+' : '-');
    dispLCD_lincol(3, 16);dispLCD_dataReg(ler_atuador('D') ? '+' : '-');
}

void * me_telas_init( void * arg, void * next )
{
    IHM.init();
    tela.next = me_telas_logo;
    return( me_telas_logo );
}

void * me_telas_start( void * arg, void * next )
{
    
     for(char i = 0; i <= 3; i++)
    {
        for(char j = 0; j <= 19; j++)
        {
            IHM.printpos(i, j, "\377");
            delay(10);
        }
    }

    for(char i = 0; i <= 3; i++)
    {
        for(char j = 0; j <= 19; j++)
        {
            IHM.printpos(i, j, " ");
            delay(10);
        }
    }     

    return( next ); 
}


void * me_telas_logo( void * arg, void * next )
{
    IHM.printpos(1, 0, "      SEGGIT        ");         

    char lema[20] = "Mundo ao seu comando";         
    char i = 0;
    

    for(ptr = lema; *ptr != 0; ptr++)            //Inserido para exibicao de lema na TELA_EMPRESA
    {
        dispLCD_lincol( 2, i );
        dispLCD_dataReg( *ptr );
        i++;
        delay(50);
    }
    ptr = 0;
   
    tela.arg = me_telas_clean;
    setT1(200);
    tela.next = me_telas_menu;
    return( me_telas_delay );
}

void * me_telas_delay( void * arg, void * next )
{
    if( !statusT1() )
        return( arg );
    else
        return( me_telas_delay );
}

void * me_telas_clean( void * arg, void * next )
{
    IHM.clr();
    return ( next );
}

void * me_telas_menu( void * arg, void * next )
{
    IHM.printpos(0,0,"1: INSERIR SEQUENCIA");
    IHM.printpos(1,0,"2: BUSCAR SEQUENCIA ");
    IHM.printpos(2,0,"3: REPETIR ULT. SEQU");
    IHM.printpos(3,0,"4: CONFIGURACOES    ");

    return( me_telas_menu_tecla );
}

void * me_telas_menu_tecla( void * arg, void * next )
{
    switch( get_tecla() )
    {
        case '1':   tela.next = me_telas_ajuste_inicial;       
                    return( me_telas_clean );
                    break;
        case '2':   
                    tela.next = me_tela_memoria_ocupadas;
                    return( me_telas_clean );
                    break;
        case '3':   fifo_print_reset();
                    tela.next = me_telas_repetir_ultima_seq;
                    return( me_telas_clean );
                    break;
        case '4':   tela.next = me_telas_config_menu;   
                    return( me_telas_clean );
                    break;
    }
    return( me_telas_menu_tecla );
}

void * go_comeback(void * arg, void * next)
{
    
    switch( get_tecla() )
    {
        case '*':
            tela.next = tela.arg;
            return( me_telas_clean );
            break;
            
        case '#':
            tela.next = next;
            return( me_telas_clean );
            break;
    }
    return(go_comeback);
}

void * me_telas_noprint( void * arg, void * next ) // não usada 
{
    
    return( next );
 
}

void * me_telas_ajuste_inicial ( void * arg, void * next )
{
    IHM.printpos(0, 0, "   AJUSTE O INICIO  "); 
    IHM.printpos(1, 0, "     DO SISTEMA     ");  
    IHM.printpos(3,0,  "   A   B   C   D    ");
    ajuste_init();
 
    return( me_telas_ajuste_editar );
}

void * me_telas_ajuste_editar (void * arg, void * next )
{
    switch( get_tecla() )
    {
        case 'A':   alt_atuador( get_tecla() ); dispLCD_lincol(3, 4); dispLCD_dataReg(ler_atuador('A') ? '+' : '-');     break;
        case 'B':   alt_atuador( get_tecla() ); dispLCD_lincol(3, 8); dispLCD_dataReg(ler_atuador('B') ? '+' : '-');     break;
        case 'C':   alt_atuador( get_tecla() ); dispLCD_lincol(3,12); dispLCD_dataReg(ler_atuador('C') ? '+' : '-');     break;
        case 'D':   alt_atuador( get_tecla() ); dispLCD_lincol(3,16); dispLCD_dataReg(ler_atuador('D') ? '+' : '-');     break;                                   
    }

    tela.next = me_tela_inserir_passo;

      if( get_tecla() == AVANCAR )
          return( me_telas_clean );
      if( get_tecla() == RETORNAR )
          return( me_telas_menu );
    
    return( me_telas_ajuste_editar );    
}

void * me_tela_inserir_passo (void * arg, void * next)
{
    
    IHM.printpos(0,0,"  INSIRA OS PASSOS  ");
    
    tela.next = me_telas_contar_passo;

    return( next );
}

void * me_telas_contar_passo ( void * arg, void * next )
{
    IHM.printpos(3,0,"<  /  >"); 
    IHM.printv(3, 1,( fifo_indice()-2), 2 );
    IHM.printv(3, 4,( fifo_tam()-2), 2 );

   
    if( get_tecla () == RETORNAR )
        return( me_telas_menu);
    else
       return( me_telas_editar_passo );
}


void * me_telas_editar_passo ( void * arg, void * next)
{
    switch( get_tecla() )
    {
        case 'A': 
        case 'B':
        case 'C': 
        case 'D': 
        
            if( fifo_disponivel() )
           {
               alt_atuador( get_tecla() );
               fifo_add( ler_atuador( get_tecla() )? get_tecla() : get_tecla() | 0x20 );
               return( me_telas_print_fifo );
           }
            break;
            
        case '6':   
            
                fifo_indicePrint_inc();
                return( me_telas_print_fifo );
                
            break;
            
        case '4': 
                fifo_indicePrint_dec();
                return( me_telas_print_fifo );
                
            break;
            
        case DELETE :
            
            if( fifo_indice() == 2 ) 
                return( me_telas_ajuste_inicial );               
            else 
            {
                fifo_delete();
                return(   me_telas_print_fifo   );
            }
            
            break;
            
        case TEMPO :   
            modo = SEG;
            numero = 0;
            tela.next =  me_telas_tempo;
            tela.arg  =  me_tela_inserir_tempo_fila;
            return( me_telas_tempo );
            
            break;
            
        case SAIDA_ANALOG :
            modo = PORCENT;
            numero = 0;
            tela.next = me_telas_controle;
            tela.arg  = me_tela_inserir_contrl_fila;
            return( me_telas_controle );
            
            break;
    }
    
    if ( get_tecla() == AVANCAR ) 
    {
         tela.next = me_tela_salvar_seq;
         return( me_telas_clean  ); 
    }
   
    return( me_telas_editar_passo );
}


void * me_telas_tempo ( void * arg, void * next )
{
    IHM.printpos(2,0,"config tempo:      s");
    IHM.printv(2,15,numero,3);
    
    return( me_telas_num_config );    
}

void *me_telas_controle (void * arg, void * next )
{
    IHM.printpos(2,0,"config contrl:     %");
    IHM.printv(2,15,numero,3);
    
    return( me_telas_num_config );      
}

void *me_telas_num_config ( void * arg, void * next)
{
    
    if ( get_tecla() >= '0' && get_tecla() <= '9' )
    {
        dignum_conc( get_tecla(),&numero);    
        if( numero > modo ) 
            numero = modo ; 
            return( next );
        
    }
    else if ( get_tecla() == DELETE  ) 
    {
        dignum_apagar(&numero);
        return( next );
    }
    
    else if ( get_tecla() == AVANCAR )
    {
        modo = 0;
        return( arg );
    }
    
    return( me_telas_num_config );
}

void * me_tela_inserir_tempo_fila ( void * arg, void * next )
{
    modo = 0;
    fifo_add_tempo(numero);
    tela.next = me_telas_print_fifo;
    return( me_telas_clean );    
}


void * me_tela_inserir_contrl_fila ( void * arg, void * next )
{
    modo = 0;
    fifo_add_control(numero);
    tela.next = me_telas_print_fifo;
    return( me_telas_clean );
}
   
void * me_telas_print_fifo ( void * arg, void * next )
{
    fifo_print();
    return( me_telas_contar_passo );
}

void * me_tela_memoria_ocupadas ( void * agr , void * next )
{   
   // unsigned char cont = 0;
   // seq.receitaLivre( aux );

//    for( char i = 0; i < 20 ; i++)        // arrumar bug
//    {
//        if( list[2*i] == aux[i] )
//            vetor[2*i] = '_';
//        else 
//            vetor[2*i] = list[2*i];
//    }
//    
//    for( char j = 0; j < 20; j++)
//    {
//        if( vetor[j] == '_')
//            cont++;
//    }
    
   tela.next = me_telas_buscar;
   return( me_telas_clean );
}

//void * me_telas_buscar_vazia ( void * arg, void * next ) // arrumar bug
//{
//    IHM.clr();
//    IHM.printpos(0,3,"SEM SEQUENCIAS");
//    IHM.printpos(1,2,"SALVAS NA MEMORIA");
//    
//    setT1(500);
//    tela.arg = me_telas_clean;
//    tela.next = me_telas_menu;
//    return( me_telas_delay );
//}

void * me_telas_buscar( void * arg, void * next )
{
    fifo_print_reset();
    IHM.printpos(0,0,"ESCOLHA MEMORIA QUE ");
    IHM.printpos(1,0,"QUE DESEJA EXECUTAR ");
   // IHM.printpos(3,0,vetor);
    IHM.printpos(3,0,list);
    
    tela.next = me_tela_buscar_tecla;
    return( me_telas_teclado );
}

void * me_telas_teclado ( void * arg, void * next )    // usar como uma função comum futuramente
{
   
    teclado = 0;
    switch ( get_tecla() )
    {
        case '0':
        case '1': 
        case '2': 
        case '3': 
        case '4':
        case '5': 
        case '6': 
        case '7': 
        case '8':
        case '9': 
        case 'A':
        case 'B':
        case 'C':
        case 'D': 
            
            teclado = get_tecla();
            teclado %= 0x30;
            return( next );
            
        case '*':           
            return( me_telas_menu );
            break;
    }
 
    return( me_telas_teclado );
}

void * me_tela_buscar_tecla( void * arg, void * next)
{
    seq.buscarDado(MEMORIA_ATDRS,teclado,&cond_inicial);
    manipula_atuadores_init( cond_inicial );
    seq.buscarReceita( teclado , vtr_aux );
    
    for( char * ptr = vtr_aux ; *ptr != 0; ptr ++ )
    {
        if( CONTROLE_V( *ptr ))
        {
            decodifica( &*ptr );
            fifo_add_control( *ptr );
        }
        else
        if( TEMPO_V ( *ptr ) )
        {
            decodifica(&*ptr);
            fifo_add_tempo( *ptr );
        }
        else 
        { 
            decodifica( &*ptr );
            switch( * ptr )
            {
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'a':
                case 'b':
                case 'c':
                case 'd':  
                           *ptr &= ~0x20;
                break;   
            }
            alt_atuador( *ptr );
            fifo_add( ler_atuador( *ptr )? *ptr : *ptr|0x20 );
        }
    }
    tela.arg = me_tela_memoria_ocupadas;
    return ( me_telas_print_buscar );
}

void * me_telas_print_buscar ( void * arg, void * next )
{
    IHM.clr();
    IHM.printpos(3,0,"<  /  > "); 
    IHM.printv(3, 1,( fifo_indice()-2), 2 );
    IHM.printv(3, 4,( fifo_tam()-2), 2 ); 
    fifo_print();
   
     tela.next = me_tela_edit_ciclos;
     tela.arg = tela.arg;
   
    return( go_comeback );
}

void * me_tela_salvar_seq (void * arg, void * next )
{
    
    IHM.printpos(0,4,"DESEJA SALVAR");
    IHM.printpos(1,5,"SEQUENCIA?");
    IHM.printpos(3,0,SIM_NAO);
    
    if( get_tecla() == AVANCAR )
    {
        tela.next = me_telas_verifica_memoria;
        return( me_telas_clean );
    }
    if( get_tecla() == RETORNAR )
    {
        tela.next = me_tela_edit_ciclos;
        return( me_telas_clean );
    }

    return( me_tela_salvar_seq  );
}

void * me_telas_verifica_memoria ( void * arg, void * next )
{
    unsigned char cont = 0;
    
    seq.receitaLivre( aux );
    for(char i = 0; i < 20; i++)
    {
        if(list[2*i] == aux[i])
            right[2*i] = list[2*i];
        else 
            right[2*i] = '_';
        
        delay(1);
    }
    
    for( char j = 0; j <20; j++)
    {
        if( right[j] == '_')
            cont++;
        
        delay(1);
    }
    
    if( cont > 9 )
    {
        from = 2;
        tela.next = me_tela_memoria_aviso;
        return( me_telas_clean  );
    }
    else   
    {
        from = 1;
        return( me_tela_memoria_escolha );
    }
    
    return( me_telas_verifica_memoria );
}

void * me_tela_memoria_aviso ( void * arg, void * next )
{
    IHM.printpos(0,2,"MEMORIAS OCUPADAS!");
    IHM.printpos(1,2,"DESEJA SUBSCREVER");
    IHM.printpos(2,3,"ALGUMA MEMORIA?");
    IHM.printpos(3,0,SIM_NAO);
            
    if( get_tecla() == AVANCAR )
    {
        tela.next = me_tela_memoria_escolha;
        return( me_telas_clean );
    }
    if( get_tecla() == RETORNAR )
    {
        tela.next = me_tela_edit_ciclos;
        return( me_telas_clean );
    }
    return(me_tela_memoria_aviso);
}

void * me_tela_memoria_escolha (void * arg, void * next )
{
    switch ( from )
    {
        case 1 :IHM.printpos(3,0,right);
        return( me_tela_memoria_escolha_tecla );
        break;
        
        case 2 :IHM.printpos(3,0,subscrever);
        break;
    }
    from = 0;
    return( me_tela_memoria_escolha ); 
}

void * me_tela_memoria_escolha_tecla( void * arg, void * next)
{
    IHM.printpos(0,0,"  ESCOLHA MEMORIA   ");
    IHM.printpos(1,0," QUE DESEJA SALVAR  ");
    IHM.printpos(2,0,"                 ");
    
    
    switch( get_tecla() )
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        
        salvar_dado(MEMORIA_ATDRS, get_tecla() % 0x30, cond_inicial);
        seq.salvarReceita(get_tecla() % 0x30, fifo_adrs() + 2);
        
        tela.next = me_tela_memoria_salvando;
        return( me_telas_start );
        break;            
    }
    
    return( me_tela_memoria_escolha_tecla);
}

void * me_tela_memoria_salvando( void * arg, void * next )
{
    seq.buscarDado(MEMORIA_ATDRS,MEMORIA_ATDRS,cond_inicial);
    seq.salvarReceita(13,fifo_adrs() + 2 );
    
    IHM.printpos(0,6,"SEQUENCIA");
    IHM.printpos(1,8,"SALVA");
    
    setT1(200);
    tela.next  = me_tela_edit_ciclos;
    tela.arg   = me_telas_clean;
    return( me_telas_delay );
    
}

void * me_tela_edit_ciclos( void * arg, void * next )
{
    IHM.printpos(0,0,"Ciclos:");
    IHM.printpos(3,0,"0 - Ciclo continuo");
    IHM.printv(0,15,ciclos,5);
    
    return( me_tela_edit_ciclos_tecla ); 
}

void * me_tela_edit_ciclos_tecla ( void * arg, void * next )
{
    if( NUMERO( get_tecla() ))
    {
        dignum_conc( get_tecla(), &ciclos );
        if( ciclos > CICLO_MAX )
            ciclos = CICLO_MAX;
        return( me_tela_edit_ciclos );
    }
    
    switch( get_tecla() )         // verificar se necessario
    {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
            tela.next = me_telas_print_fifo;
            return( me_telas_clean );
            break; 
            
        case DELETE:
            
            dignum_apagar(&ciclos);
            return( me_tela_edit_ciclos );
            break;
            
        case AVANCAR:
            
            btt = PLAY;
            return( me_tela_inserir_ciclo_fifo );
            break; 
    }
    
    return( me_tela_edit_ciclos_tecla );
}

void * me_tela_inserir_ciclo_fifo ( void * arg, void * next )
{
    fifo_add_ciclo(ciclos);
    tela.next = me_tela_execucao;
    execucao.func = me_exec_init;
    return( me_telas_clean );
}

void * me_tela_execucao (void * arg , void * next )
{
    IHM.printpos(0,0,"       [    ]       ");
    IHM.printpos(2,10,"_ciclos_");
    IHM.printpos(3,0,"<  /  >   |        |");
  
    return( me_tela_execucao_print );
 }

void * me_tela_execucao_print (void * arg , void * next )
{
    if( execucao.func != me_exec_principal )      // carrega os primeiros dados na tela antes de começar rodar
    {
       printExec(4,-1);
       printExec(9, 0);
       printExec(14,1);
       
       IHM.printv(3, 4, (     fifo_indice() - 2 ), 2);
       IHM.printv(3, 1, ( getIndicePassos() - 1 ), 2);                                         
       IHM.printv(3, 14,(   getContCiclos() + 1 ), 5); 
    }
    
   if( execucao.func == me_exec_principal && btt != PAUSE  )
    {
       IHM.printpos(0,0,"       [    ]       ");  // corrige o problema de os dados de seg e control ficaram um por cima do outro
       printExec(4,-1);
       printExec(9, 0);
       printExec(14,1);
    
       IHM.printv(3, 4, (     fifo_indice() - 2 ), 2);
       IHM.printv(3, 1, ( getIndicePassos() - 1 ), 2);                                         
       IHM.printv(3, 14,(   getContCiclos() + 1 ), 5); 
    }
    
       switch( get_tecla())
       {
            case TECLA_PLAY :
                btt = PLAY;
                break;

            case TECLA_PAUSE :
                btt = PAUSE;
                break;

            case TECLA_CANCELAR :
                btt = STOP;
                break;

            case TECLA_STEP :
             if( btt == PAUSE)
                 btt = STEP;
                break;    
        }
   
    if( execucao.func == me_exec_vazio  && btt == STOP )
    {
        tela.next =  me_telas_cancelar_execucao;
        return( me_telas_clean );
    }
    else if( execucao.func == me_exec_vazio )
    {
        tela.next = me_telas_print_fifo;
        return( me_telas_clean );
    }
   
    return( me_tela_execucao_print );
}

void * me_telas_cancelar_execucao ( void * arg , void * next )
{
    
    IHM.printpos(0,1,"Deseja cancelar ?");
    IHM.printpos(3,0,SIM_NAO);
   
    if( get_tecla() == AVANCAR )
    {
        resetContCiclos();
        resetIndicePassos();
        fifo_print_reset();
        tela.next = me_tela_inserir_passo;  
      return( me_telas_start );
      
    }
    
    if( get_tecla() == RETORNAR )
    {
        btt = PLAY;
        execucao.func = me_exec_init;
        tela.next = me_tela_execucao;
        return( me_telas_clean );
    }
    
    return( me_telas_cancelar_execucao );
}




void * me_telas_repetir_ultima_seq( void * arg, void * next )
{
    
    seq.buscarDado(MEMORIA_ATDRS,MEMORIA_ATDRS,&cond_inicial);
    manipula_atuadores_init(cond_inicial);
    seq.buscarReceita(13,vtr_aux);
    ptr_aux = vtr_aux;
    
    if( *ptr_aux == 0xFF )
    {
        seq.deletarReceita(get_tecla() % 0x30 );
        vtr_aux[0] = 0;
    }

        for(char *ptr = vtr_aux ; *ptr != 0; ptr++)     //Varre o vetor_aux e insere os passos na fila (vetor fila)
       {                                                  
            if(*ptr <= 0x64 || *ptr == 0xFE)
            {
                decodifica(&*ptr);
                fifo_add_control(*ptr);
            }
            else if (*ptr > 0x64 && *ptr <= 0xDC)
            {
                decodifica(&*ptr);
                fifo_add_tempo(*ptr);
            }
            else
            {    
                decodifica(&*ptr);
                switch( *ptr )
                {
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':                           
                              *ptr &= ~0x20;  
                               break;
                }            
                alt_atuador(*ptr);                                                  
                fifo_add( ler_atuador(*ptr) ? *ptr : *ptr | 0x20 );
            }
            fifo_print();
       }
      tela.next = me_telas_print_buscar;
      tela.arg  = me_telas_menu;
      return( me_telas_clean );
}

void * me_telas_config_menu( void * arg, void * next )
{
    IHM.printpos(0,0,"1: VERIFICAR MEMORIA");
    IHM.printpos(1,0,"2: FORMATAR");
    IHM.printpos(2,0,"3: RESERVADO");
    IHM.printpos(3,0,"4: INF. DO FIRMWARE");
    return(me_telas_config_menu_tecla);    
}

void * me_telas_config_menu_tecla( void * arg, void * next )
{
    switch( get_tecla() )
    {
        case '1': 
                tela.next = me_telas_config1_msg;
                return( me_telas_clean );
                break;
        case '2': 
                tela.next = me_telas_config2_msg;
                return( me_telas_clean );
                break;
        case '3': 
                tela.next = me_telas_config_terceiro;
                return( me_telas_clean );
                break;
        case '4': 
                tela.next = me_telas_config4_inf;
                return( me_telas_clean );
                break;
        case '*': 
                tela.next = me_telas_menu;
                return( me_telas_clean );
                break;
    }
    return( me_telas_config_menu_tecla );
}

void *me_telas_config1_msg (void * arg, void * next )
{
     fifo_print_reset();
     IHM.printpos(0,1,"ESCOLHA MEMORIA QUE");
     IHM.printpos(1,2,"DESEJA VERIFICAR");
     IHM.printpos(3,0,list);
     
     tela.next = me_telas_config1_tecla;
     return( me_telas_teclado );
}

void * me_telas_config1_tecla ( void * arg, void * next )
{ 
        seq.buscarDado(MEMORIA_ATDRS,teclado , &cond_inicial );
        seq.buscarReceita( teclado , vtr_aux );

        for(char *ptr = vtr_aux ; *ptr != 0; ptr++)     //Varre o vetor_aux e insere os passos na fila (vetor fila)
       {                                                  
            if( CONTROLE_V( *ptr) )
            {
                decodifica(&*ptr);
                fifo_add_control(*ptr);
            }
            else if ( TEMPO_V( *ptr ) )
            {
                decodifica(&*ptr);
                fifo_add_tempo(*ptr);
            }
            else
            {    
                decodifica(&*ptr);
                switch( *ptr )
                {
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':                           
                              *ptr &= ~0x20;  
                               break;
                }            
                alt_atuador(*ptr);                                                  
                fifo_add( ler_atuador(*ptr) ? *ptr : *ptr | 0x20 );
            }
       }
        
    tela.next = me_telas_config1_exec;
    return( me_telas_clean );      
}

void * me_telas_config1_exec (void * arg, void * next )
{  
      fifo_print();
      IHM.printpos(3,0,"* - SAIR  # - APAGAR");
      
      if( get_tecla() == AVANCAR )
      {
          seq.deletarReceita( teclado );
          tela.next = me_telas_config1_excluir;
          return( me_telas_start  );
      }
      if( get_tecla() == RETORNAR )
      {
          tela.next = me_telas_config1_msg;
          return( me_telas_clean );
      }
      
      return( me_telas_config1_exec );     
}

void * me_telas_config1_excluir ( void * arg, void * next )
{
    IHM.printpos(0,2,"MEMORIA EXCLUIDA");
    setT1(200);
    tela.arg = me_telas_clean;
    tela.next = me_telas_config1_msg;
    return( me_telas_delay );

}


void * me_telas_config2_msg(void * arg, void * next )
{
    IHM.printpos(0,1,"CERTEZA QUE DESEJA");
    IHM.printpos(1,3,"DELETAR TODA A");
    IHM.printpos(2,6,"MEMORIA ?");
    IHM.printpos(3,0,SIM_NAO);
    
    tela.next = me_telas_config2_exec;
    tela.arg = me_telas_config_menu;
    return( go_comeback );    
}


void * me_telas_config2_exec(void * arg, void * next )
{
    char cont = 1;
    IHM.printpos(0,6,"LIMPANDO");

    for ( char i = 0; i < 17; i ++)
    { 
        
        seq.formatarMemoria(i * 16, ((i * 16) + 16) );
        IHM.printpos(3, i , "\377");
        IHM.printv(3,16,i * 6.25 , 3);  
        dispLCD_dataReg('%');
        delay(300);

        cont ++;
    }


    if( cont > 15)
    {
        tela.next = me_telas_config2_ready;
        return( me_telas_clean );
    }
    else 
    {
    return( me_telas_config2_exec );
    }
}

void * me_telas_config2_ready(void * arg, void * next )
{
    IHM.printpos(0,7,"LIMPEZA");
    IHM.printpos(1,6,"CONCLUIDA");
    
    setT1(200);
    
    tela.arg = me_telas_clean;
    tela.next = me_telas_config_menu;
    return( me_telas_delay );
}

void * me_telas_config_terceiro(void * arg, void * next )
{
    IHM.printpos(0,3,"RESERVADA PARA");
    IHM.printpos(1,3,"PROXIMA VERSAO");
    IHM.printpos(2,5,"DO FIRMWARE");
    
   if( get_tecla() == '*' )
   {
        tela.next = me_telas_config_menu;
        return( me_telas_clean );
   }
    return( me_telas_config_terceiro );
}
    
void * me_telas_config4_inf (void * arg, void * next )
{
    
    IHM.printpos(0,0,"MICRO : 18F4550(PIC)");
    IHM.printpos(1,0,"EEPROM:   256(bytes)");
    IHM.printpos(3,0,"VERSAO:      slp.v3 ");
    
    if( get_tecla() == '*' )
    {
        tela.next = me_telas_config_menu;
        return( me_telas_clean );
    }
    return( me_telas_config4_inf );
}


#ifndef ME_EXEC_H
#define ME_EXEC_H

extern struct fsmT execucao;


extern char btt;
 
void * me_exec_vazio ( void * arg, void * next );
void * me_exec_init ( void * arg, void * next );
void * me_exec_delay ( void * arg, void * next );
void * me_exec_botoes ( void * arg , void * next );
void * me_exec_principal ( void * arg, void * next );
void * me_exec_ler_sensor (void * arg, void * next );
void * me_exec_indice ( void * arg, void *  next );
void * me_exec_ciclos ( void * arg , void * next );
void * me_exec_ler_step (void * arg, void * next );


#endif

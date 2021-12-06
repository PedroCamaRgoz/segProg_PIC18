#ifndef ME_TELAS_H
#define	ME_TELAS_H

#include "me.h"

extern struct fsmT tela;



void * me_telas_ajuste_inicial ( void * arg, void * next );
void * me_telas_init( void * arg, void * next );
void * me_telas_start( void * arg, void * next );
void * me_telas_logo( void * arg, void * next );
void * me_telas_delay( void * arg, void * next );
void * me_telas_clean( void * arg, void * next );
void * me_telas_menu( void * arg, void * next );
void * me_telas_menu_tecla( void * arg, void * next );
void * me_telas_noprint( void * arg, void * next );
void * me_tela_inserir_passo (void * arg, void * next);
void * me_telas_contar_passo ( void * arg, void * next );
void * go_comeback(void * arg, void * next);
void * go(void * arg, void * next);
void * me_telas_contar_passo ( void * arg, void * next );
// void * me_telas_limpar_fila (void * arg, void * next );

void * me_telas_teclado ( void * arg, void * next );

void * me_tela_salvar_seq (void * arg, void * next );
//void * me_tela_memorias_Seq (void * arg, void * next );
void * me_tela_memoria_escolha (void * arg, void * next );
//void * me_tela_memoria_tecla_seq( void * arg, void * next);
void * me_tela_memoria_escolha_tecla( void * arg, void * next);
//void * me_tela_memoria_salvando_seq( void * arg, void * next );
void * me_tela_memoria_salvando( void * arg, void * next );
void * me_tela_edit_ciclos( void * arg, void * next );
//void * me_tela_buscar_aviso ( void * arg, void * next );
void * me_tela_edit_ciclos_tecla ( void * arg, void * next );
void * me_tela_memoria_aviso ( void * arg, void * next );
void * me_telas_verifica_memoria ( void * arg, void * next );
void * me_telas_buscar( void * arg, void * next );

void * me_telas_config_menu_tecla( void * arg, void * next );
void * me_telas_config_menu( void * arg, void * next );
//void * me_telas_config_verificar(void * arg, void * next );
//void * me_telas_config_limpar(void * arg, void * next );
void * me_telas_config_terceiro(void * arg, void * next );
//void * me_tela_exec_seq (void * arg , void * next );
void * me_tela_execucao (void * arg , void * next );
void * me_tela_buscar_tecla( void * arg, void * next);

void * me_telas_inserir( void * arg, void * next );
void * me_telas_buscar( void * arg, void * next );
//void * me_telas_repetir( void * arg, void * next );
void * me_telas_config( void * arg, void * next );

void ajuste_init ( void );
void * me_telas_ajuste_editar (void * arg, void * next );
void * me_telas_print_fifo ( void * arg, void * next );
void * me_telas_contar_passo ( void * arg, void * next );
void * me_telas_editar_passo ( void * arg, void * next);
void * right_left (void * arg, void * next);
void * me_telas_tempo ( void * arg, void * next );
void *me_telas_controle (void * arg, void * next );
void * me_telas_tempo_config ( void * arg, void * next );
void * me_tela_inserir_contrl_fila ( void * arg, void * next ); // teste;
void * me_tela_inserir_tempo_fila ( void * arg, void * next ); // teste
void *me_telas_num_config ( void * arg, void * next); // teste
void * me_tela_inserir_ciclo_fifo ( void * arg, void * next );
void * me_telas_repetir_ultima_seq( void * arg, void * next );
void * me_telas_cancelar_execucao ( void * arg , void * next );


void * me_telas_config2_msg(void * arg, void * next );
void * me_telas_config2_exec(void * arg, void * next );
void * me_telas_config2_ready(void * arg, void * next );
void *me_tela_cancelar ( void * arg, void * next );
void * me_tela_execucao_print (void * arg , void * next );
void * me_tela_memoria_ocupadas ( void * agr , void * next );
void * me_tela_buscar_tecla( void * arg, void * next);
void * me_telas_print_buscar ( void * arg, void * next );

void *me_telas_config1_msg (void * arg, void * next );
void * me_telas_config1_tecla ( void * arg, void * next );
void * me_telas_config1_exec (void * arg, void * next );
void * me_telas_buscar_vazia ( void * arg, void * next );

void * me_telas_config4_inf (void * arg, void * next );
void * me_telas_config1_excluir ( void * arg, void * next );

#endif	/* ME_TELAS_H */

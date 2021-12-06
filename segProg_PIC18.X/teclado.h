#ifndef TECLADO_H
#define TECLADO_H

#define TECLA_LEFT       '4'
#define TECLA_RIGHT      '6'
#define TECLA_STEP       '1'
#define TECLA_PAUSE      '5'
#define TECLA_PLAY       '#'
//#define TECLA_STOP       '0'
#define TECLA_ENTER      '#'
#define TECLA_DELETE     '*'
#define TECLA_T          '0'
#define TELA_SAIDA_ANL   '9'
#define TECLA_CANCELAR   '0'

void teclado_init( void );
unsigned char get_tecla( void );
unsigned char teclado_borda(void);



#endif

#ifndef SEQUENCIA_H
#define SEQUENCIA_H


int end_receita (int x);
void salvar_receita (unsigned char adrs, unsigned char *data );
void buscar_receita (unsigned char adrs, unsigned char * data );  // adrs = busca receitas 0 a 16, * data = recebe receita buscada e aponta para o vetor
void salvar_dado (char adrs, char col, unsigned char data); //funcao criada para salvar um unico dado de qualquer endereco de memoria da EEPROM
void buscar_dado (char adrs, char col, unsigned char *data); //funcao criada para buscar um unico dado de qualquer endereco de memoria da EEPROM
void deletar_receita (char receita);
void receita_livre ( unsigned char * rec);
//void formatar_receitas (void);
char formatar_receitas ( char x, char y );
//void print_receita_livre (void);
//static void print_receita_livre ( void ) ; 
void print_receita_livre ( unsigned char * newline );

struct sequencia_t
{  
    void (*salvarReceita ) (unsigned char adrs, unsigned char *data );
    void (*buscarReceita) (unsigned char adrs, unsigned char * data );
    void (*salvarDado) (char adrs, char col, unsigned char data); 
    void (*buscarDado)(char adrs, char col, unsigned char *data); 
    void (*deletarReceita) (char receita);
    void (*receitaLivre) ( unsigned char * rec);
    char (*formatarMemoria) (char x, char y);     
   // void (*printReceitaLivre) (void);
    void (*printReceitaLivre) ( unsigned char * newline );
};

extern struct sequencia_t seq;

#endif
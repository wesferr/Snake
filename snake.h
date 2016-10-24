
//Autoria no main.c

typedef struct parte{
	int x, y, ant_x, ant_y;
	struct parte *prox;
} parte_t;

extern char mv;
extern char in;
extern char dif;
extern int max_x;
extern int max_y;
extern int com_x;
extern int com_y;
extern int comida;
extern int VELOCIDADE;
extern int pontuacao;
extern int opcao;

parte_t* inicia_lst(void);
parte_t* insere_lst(parte_t* lst, int x, int y, int antx, int anty);
void fim(parte_t* snake);
void libera_lst(parte_t* lst);
void move_snake(parte_t* snake);
void *process(void *argumento);
void *menu();

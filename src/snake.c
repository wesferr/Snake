#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include "snake.h"

//Autoria no main.c

parte_t* inicia_lst(void){
	return NULL;
}

parte_t* insere_lst(parte_t* lst, int x, int y, int antx, int anty){
	parte_t* novo = (parte_t*)malloc(sizeof(parte_t));
	novo->x = x;
	novo->y = y;
	novo->ant_x = antx;
	novo->ant_y = anty;
	novo->prox = lst;
	lst = novo;
	return novo;
}

void libera_lst(parte_t* lst){
	while(lst != NULL){	
		parte_t* aux = lst->prox;
		free(lst);
		lst = aux;
	}
}

void atualiza_outras(parte_t* snake){
	parte_t* inicio = snake;
	while(snake->prox != NULL){
		int aux_x = snake->ant_x;
		int aux_y = snake->ant_y;
		snake = snake->prox;
		snake->ant_x = snake->x;
		snake->ant_y = snake->y;
		snake->x = aux_x;
		snake->y = aux_y;
		attron(COLOR_PAIR(2));
		mvprintw(snake->ant_x, snake->ant_y,"o");
		attroff(COLOR_PAIR(2));
		if((inicio->x == snake->x) && (inicio->y == snake->y)){
			fim(inicio);
			pthread_exit(NULL);
		}
	}
}

void move_snake(parte_t* snake){

	switch(mv){
		case 5:
			if(snake->y < (max_y-2)){
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->y++;
			}else{
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->y = 2;	
			}
			break;
	
		case 2:
			if(snake->x < (max_x-2)){
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->x++;
			}else{	
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->x = 2;
			}
			break;
	
		case 4:
			if (snake->y > 2){
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->y--;
			}else{
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->y = max_y-2;
			}
			break;
	
		case 3:
			if (snake->x > 2){
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;
				snake->x--;
			}else{
				snake->ant_y = snake->y;
				snake->ant_x = snake->x;		
				snake->x = max_x-2;
			}
			break;
	}
	atualiza_outras(snake);
		
		if(in == 27){
			fim(snake);
			pthread_exit(NULL);
		}
	
	
}
void *process(void* argumento){
	parte_t* snake = (parte_t*) argumento;

	while(TRUE){
		clear();

		attron(COLOR_PAIR(3));
		for(int i = 0; i< max_y;i++)mvprintw(1,i," ");
		for(int i = 0; i< max_y;i++)mvprintw(max_x-1,i," ");
		for(int i = 0; i< max_x;i++)mvprintw(i,0," ");
		for(int i = 0; i< max_x;i++)mvprintw(i,max_y-1," ");
		attroff(COLOR_PAIR(3));

		attron(COLOR_PAIR(1));
		for(int i = 0; i< max_y;i++)mvprintw(0,i," ");
		attroff(COLOR_PAIR(1));

		attron(COLOR_PAIR(2));
		mvprintw(snake->x, snake->y, "0");
		if(comida == 1) mvprintw(com_x, com_y, "x");
		attroff(COLOR_PAIR(2));		

		attron(COLOR_PAIR(1));
		mvprintw(0, 0, "PONTUACAO:%5d", pontuacao);
		attroff(COLOR_PAIR(1));

		attron(COLOR_PAIR(1));
		if(opcao == 0){
			mvprintw(0, 18, "maxx:%3d", max_x);
			mvprintw(0, 27, "maxy:%3d", max_y);
			mvprintw(0, 36, "comx:%3d", com_x);
			mvprintw(0, 45, "comy:%3d", com_y);
			mvprintw(0, 54, "snax:%3d", snake->x);
			mvprintw(0, 63, "snay:%3d", snake->y);
			mvprintw(0, 72, "antx:%3d", snake->ant_x);
			mvprintw(0, 81, "anty:%3d", snake->ant_y);
			if(in == 32){
				snake->x = com_x;
				snake->y = com_y;
				mv = 5;			
			}
		}else{
			mvprintw(0, max_y-22, "APERTE [ESC] PARA SAIR");
			mvprintw(0, max_y/2-2, "SNAKE");
			
		}
		attroff(COLOR_PAIR(1));

		if(comida == 0){
			com_x = 2 + rand()%(max_x-3);
			com_y = 2 + rand()%(max_y-3);	
			comida=1;
		}

		if( (snake->x == com_x) && (snake->y == com_y) ){
			comida = 0;
			pontuacao++;
			snake = insere_lst(snake, snake->x, snake->y, snake->ant_x, snake->ant_y);
		}

		move_snake(snake);
		usleep(VELOCIDADE);
		refresh();
	}

	return NULL;
}

void *menu(){
	while(TRUE){
		clear();
		attron(COLOR_PAIR(1));
		for(int i = 0; i < max_x; i++)
			for(int j = 0;j < max_y; j++)
				mvprintw(i,j," ");
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(1));
		mvprintw(max_x/2-7,max_y/2-5,"INSTRUCOES");
		mvprintw(max_x/2-6,max_y/2-12,"USE AS SETAS PARA JOGAR");
		mvprintw(max_x/2-2,max_y/2-13,"SELECIONE UMA DIFICULDADE");
		mvprintw(max_x/2-1,max_y/2-12,"USE O TECLADO NUMERICO:");
		mvprintw(max_x/2+2, max_y/2-14, "APERTE ENTER PARA CONTINIAR");
		mvprintw(max_x/2+10,max_y/2-22,"[ESC] PARA SAIR DO JOGO A QUALQUER MOMENTO");
		attroff(COLOR_PAIR(1));
		if(dif == 49){
			attron(COLOR_PAIR(2));
			mvprintw(max_x/2,max_y/2-7,"1");
			attroff(COLOR_PAIR(2));
			VELOCIDADE = 240000;
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(max_x/2,max_y/2-7,"1");
			attroff(COLOR_PAIR(1));
		}
		if(dif == 50){
			attron(COLOR_PAIR(2));
			mvprintw(max_x/2,max_y/2-5,"2");
			attroff(COLOR_PAIR(2));
			VELOCIDADE = 200000;
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(max_x/2,max_y/2-5,"2");
			attroff(COLOR_PAIR(1));
		}
		if(dif == 51){
			attron(COLOR_PAIR(2));
			mvprintw(max_x/2,max_y/2-3,"3");
			attroff(COLOR_PAIR(2));
			VELOCIDADE = 160000;
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(max_x/2,max_y/2-3,"3");
			attroff(COLOR_PAIR(1));
		}
		if(dif == 52){
			attron(COLOR_PAIR(2));
			mvprintw(max_x/2,max_y/2-1,"4");
			attroff(COLOR_PAIR(2));
			VELOCIDADE = 120000;
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(max_x/2,max_y/2-1,"4");
			attroff(COLOR_PAIR(1));
		}
		if(dif == 53){
			attron(COLOR_PAIR(2));
			mvprintw(max_x/2,max_y/2+1,"5");
			attroff(COLOR_PAIR(2));
			VELOCIDADE = 80000;
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(max_x/2,max_y/2+1,"5");
			attroff(COLOR_PAIR(1));
		}
		if(dif == 54){
			attron(COLOR_PAIR(2));
			mvprintw(max_x/2,max_y/2+3,"6");
			attroff(COLOR_PAIR(2));
			VELOCIDADE = 40000;
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(max_x/2,max_y/2+3,"6");
			attroff(COLOR_PAIR(1));
		}

		refresh();
		getmaxyx(stdscr, max_x, max_y);
		if (dif == 10)break;
	}
}

void fim(parte_t* snake){
	if(opcao != 0){
		clear();
		attron(COLOR_PAIR(3));
		for(int i = 0; i < max_x; i++)
			for(int j = 0;j < max_y; j++)
				mvprintw(i,j," ");
		mvprintw(max_x/2-1, max_y/2-6, "FIM DE JOGO!");
		mvprintw(max_x/2, max_y/2-7, "PONTUACAO:%5d", pontuacao);
		attroff(COLOR_PAIR(3));
		refresh();
		usleep(5000000);
	}
	endwin();
	libera_lst(snake);
	exit(0);
}

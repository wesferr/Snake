#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include "snake.h"

/*
	AUTORIA:
		Aluno: Wesley Ferreira de Ferreira
		Matricula: 161151193
		Disciplina: Estrutura de dados I 02.16
		Professor: Diego Luis kreutz
	
	Trabalho 2:
		Desenvolver um jogo simples usando ncurses e pthreads.
*/

pthread_t thread;
int max_x = 0, max_y = 0, comida = 0, com_x = 0, com_y = 0, pontuacao = 0, opcao = 1, VELOCIDADE = 90000;
char mv = 5, in = 5, dif = 0;

int main(int argc, char *argv[]){

	if(argc == 2) opcao = atoi(argv[1]);

	initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_BLUE);
	noecho();
	cbreak();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, max_x, max_y);
	srand(time(NULL));
	
	if(opcao != 0){
		pthread_create(&thread, NULL, &menu, NULL);
		while(TRUE){
			dif = getch();
			if(dif == 10) break;
			if(dif == 27) fim(NULL);
		}
	}
	parte_t* snake = inicia_lst();
	snake = insere_lst(snake, max_x/2, max_y/2, 0, 0);

	pthread_create(&thread, NULL, &process, (void*)snake);
	
	while(TRUE){

		in = getch();
		usleep(100);
		if(in == 3 && mv != 2)mv = in;
		if(in == 2 && mv != 3)mv = in;
		if(in == 4 && mv != 5 )mv = in;
		if(in == 5 && mv != 4)mv = in;
			
		getmaxyx(stdscr, max_x, max_y);
	}
}

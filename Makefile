LDFLAGS = -lm -lpthread -lncurses

snake: main.o snake.o snake.h
	gcc -o snake main.o snake.o $(LDFLAGS) -I.
main.o: main.c
	gcc -c main.c $(LDFLAGS) -I.
snake.o: snake.c
	gcc -c snake.c $(LDFLAGS) -I.
clean:
	rm -f snake *.o

CC = gcc
CFLAGS = -Wall -Werror -g

OBJS = server.o

all:
	echo "\n\n    DIGITE\n        make server\n        make client\n        make single (versao para testes apenas)\n"

server: network.o server.o
	$(CC) $(CFLAGS) -o pacman game.c network.o server.o

client: network.o client.o
	$(CC) $(CFLAGS) -o pacman main.c network.o client.o

single: debug.o server.o client.o
	$(CC) $(CFLAGS) -o pacman bagunca2.c debug.o server.o client.o
	echo "\nversao teste SEM cliente-servidor"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f pacman jogo.csv *.o

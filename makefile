CC = gcc
CFLAGS = -Wall -Werror -g

OBJS = server.o

all:
	echo "\n\n    DIGITE\n        make server\n        make client\n        make single (versao para testes apenas)\n"

server: network.o server.o tesoura.o
	$(CC) $(CFLAGS) -o GRR_20234264_ game.c network.o server.o tesoura.o

client: network.o client.o tesoura.o
	$(CC) $(CFLAGS) -o GRR_20234264_ main.c network.o client.o tesoura.o

single: debug.o server.o client.o tesoura.o
	$(CC) $(CFLAGS) -o GRR_20234264_ bagunca2.c debug.o server.o client.o tesoura.o
	echo "\nversao teste SEM cliente-servidor"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f GRR_20234264_GRR_20190133 jogo.csv *.o

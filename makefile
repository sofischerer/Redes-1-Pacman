CC = gcc
CFLAGS = -Wall -Werror -g

OBJS = server.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o pacman main.c $(OBJS)

single: server.o
	$(CC) $(CFLAGS) -o pacman bagunca2.c server.o
	echo "\nversao teste SEM cliente-servidor"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f pacman *.o

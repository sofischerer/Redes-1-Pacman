# Nome do executável final
TARGET = teste

# Compilador a ser usado
CC = gcc

# Flags (opções) de compilação
# -Wall: ativa quase todos os avisos de erro (warnings)
# -Wextra: ativa avisos adicionais
# -g: inclui informações de depuração (útil para o GDB)
#CFLAGS = -Wall -Wextra -g

# Lista de arquivos fonte (.c) - adicione os seus aqui separados por espaço
SRCS = testes.c network.c

# Gera a lista de arquivos objeto (.o) correspondentes aos fontes
OBJS = $(SRCS:.c=.o)

# Regra padrão (primeira regra executada ao rodar apenas 'make')
all: $(TARGET)

# Regra para linkar os objetos e gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra genérica para compilar arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados pela compilação
clean:
	rm -f $(OBJS) $(TARGET)

# Evita conflitos com arquivos que possam ter o mesmo nome das regras
.PHONY: all clean
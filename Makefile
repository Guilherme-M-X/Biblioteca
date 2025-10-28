# Makefile para Sistema de Gestão de Biblioteca
# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = biblioteca
SOURCES = main.c usuarios.c livros.c emprestimos.c arquivo.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = biblioteca.h usuarios.h livros.h emprestimos.h arquivo.h

# Regra principal
all: $(TARGET)

# Compilação do executável
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Compilacao concluida com sucesso!"
	@echo "Execute com: ./$(TARGET)"

# Compilação dos objetos
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Limpar arquivos compilados e dados
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Arquivos compilados removidos!"

# Limpar também os arquivos de dados
clean-all: clean
	rm -f *.dat
	@echo "Arquivos de dados removidos!"

# Recompilar tudo
rebuild: clean all

# Ajuda
help:
	@echo "Opcoes disponiveis:"
	@echo "  make          - Compila o programa"
	@echo "  make run      - Compila e executa"
	@echo "  make clean    - Remove arquivos compilados"
	@echo "  make clean-all - Remove compilados E dados salvos"
	@echo "  make rebuild  - Recompila tudo"
	@echo "  make help     - Mostra esta ajuda"

.PHONY: all run clean clean-all rebuild help

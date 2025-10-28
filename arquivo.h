#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "biblioteca.h"

// Nomes dos arquivos de dados
#define ARQUIVO_LIVROS "livros.dat"
#define ARQUIVO_USUARIOS "usuarios.dat"
#define ARQUIVO_EMPRESTIMOS "emprestimos.dat"

// Protótipos
int salvar_dados(Sistema *s);
int carregar_dados(Sistema *s);
int salvar_livros(Sistema *s);
int salvar_usuarios(Sistema *s);
int salvar_emprestimos(Sistema *s);
int carregar_livros(Sistema *s);
int carregar_usuarios(Sistema *s);
int carregar_emprestimos(Sistema *s);

#endif // ARQUIVO_H

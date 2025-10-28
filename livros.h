#ifndef LIVROS_H
#define LIVROS_H

#include "biblioteca.h"

// Protótipos
void cadastrar_livro(Sistema *s);
void listar_livros(Sistema *s);
void buscar_livro(Sistema *s);
void editar_livro(Sistema *s);
void remover_livro(Sistema *s);
const char* status_livro_string(StatusLivro status);
void menu_livros(Sistema *s, Usuario *usuario_logado);

#endif // LIVROS_H

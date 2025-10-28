#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H

#include "biblioteca.h"

// Protótipos
void realizar_emprestimo(Sistema *s, Usuario *usuario_logado);
void devolver_livro(Sistema *s, Usuario *usuario_logado);
void renovar_emprestimo(Sistema *s, Usuario *usuario_logado);
void listar_emprestimos_usuario(Sistema *s, int id_usuario);
void listar_todos_emprestimos(Sistema *s);
float calcular_multa(time_t data_prevista);
int verificar_limite_emprestimos(Sistema *s, Usuario *u);
void menu_emprestimos(Sistema *s, Usuario *usuario_logado);

#endif // EMPRESTIMOS_H

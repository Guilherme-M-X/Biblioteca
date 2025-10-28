#ifndef USUARIOS_H
#define USUARIOS_H

#include "biblioteca.h"

// Protótipos
Usuario* autenticar_usuario(Sistema *s, char *login, char *senha);
void cadastrar_usuario(Sistema *s, Usuario *admin);
void listar_usuarios(Sistema *s);
void buscar_usuario(Sistema *s);
void editar_usuario(Sistema *s, Usuario *admin);
void remover_usuario(Sistema *s, Usuario *admin);
int verificar_permissao(Usuario *u, int operacao);
const char* tipo_usuario_string(TipoUsuario tipo);
void menu_usuarios(Sistema *s, Usuario *usuario_logado);

#endif // USUARIOS_H

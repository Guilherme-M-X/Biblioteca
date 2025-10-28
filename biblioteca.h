#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Constantes do sistema
#define MAX_TITULO 100
#define MAX_NOME 100
#define MAX_AUTOR 80
#define MAX_EDITORA 60
#define MAX_LIVROS 500
#define MAX_USUARIOS 200
#define MAX_EMPRESTIMOS 1000
#define MAX_SENHA 50
#define DIAS_EMPRESTIMO_ALUNO 15
#define DIAS_EMPRESTIMO_PROFESSOR 30
#define MAX_LIVROS_ALUNO 3
#define MAX_LIVROS_PROFESSOR 5
#define MULTA_POR_DIA 2.50

// Enumerações
typedef enum {
    ALUNO = 1,
    PROFESSOR,
    BIBLIOTECARIO,
    ADMINISTRADOR
} TipoUsuario;

typedef enum {
    DISPONIVEL = 1,
    EMPRESTADO,
    RESERVADO,
    MANUTENCAO
} StatusLivro;

typedef enum {
    ATIVO = 1,
    DEVOLVIDO,
    ATRASADO
} StatusEmprestimo;

// Estruturas
typedef struct {
    int id;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    char editora[MAX_EDITORA];
    int ano;
    char isbn[20];
    int exemplares_totais;
    int exemplares_disponiveis;
    StatusLivro status;
} Livro;

typedef struct {
    int id;
    char nome[MAX_NOME];
    char login[50];
    char senha[MAX_SENHA];
    TipoUsuario tipo;
    char curso_departamento[100];
    int livros_emprestados;
    int ativo;
} Usuario;

typedef struct {
    int id;
    int id_livro;
    int id_usuario;
    time_t data_emprestimo;
    time_t data_devolucao_prevista;
    time_t data_devolucao_real;
    StatusEmprestimo status;
    float multa;
} Emprestimo;

typedef struct {
    Livro livros[MAX_LIVROS];
    Usuario usuarios[MAX_USUARIOS];
    Emprestimo emprestimos[MAX_EMPRESTIMOS];
    int total_livros;
    int total_usuarios;
    int total_emprestimos;
} Sistema;

// Protótipos de funções
void inicializar_sistema(Sistema *s);
void menu_principal(Sistema *s, Usuario *usuario_logado);
void limpar_tela();
void pausar();

#endif // BIBLIOTECA_H

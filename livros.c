#include "biblioteca.h"
#include "livros.h"
#include "arquivo.h"

const char* status_livro_string(StatusLivro status) {
    switch(status) {
        case DISPONIVEL: return "Disponivel";
        case EMPRESTADO: return "Emprestado";
        case RESERVADO: return "Reservado";
        case MANUTENCAO: return "Manutencao";
        default: return "Desconhecido";
    }
}

void cadastrar_livro(Sistema *s) {
    if (s->total_livros >= MAX_LIVROS) {
        printf("\n Limite de livros atingido!\n");
        return;
    }

    Livro novo;
    novo.id = s->total_livros + 1;

    printf("\n");
    printf(" ╔═══════════════════════════════════════════════╗\n");
    printf(" ║         CADASTRO DE NOVO LIVRO                ║\n");
    printf(" ╚═══════════════════════════════════════════════╝\n");
    printf("\n Titulo: ");
    getchar();
    fgets(novo.titulo, MAX_TITULO, stdin);
    novo.titulo[strcspn(novo.titulo, "\n")] = 0;

    printf(" Autor: ");
    fgets(novo.autor, MAX_AUTOR, stdin);
    novo.autor[strcspn(novo.autor, "\n")] = 0;

    printf(" Editora: ");
    fgets(novo.editora, MAX_EDITORA, stdin);
    novo.editora[strcspn(novo.editora, "\n")] = 0;

    printf(" Ano de publicacao: ");
    scanf("%d", &novo.ano);

    printf(" ISBN: ");
    scanf("%s", novo.isbn);

    printf(" Numero de exemplares: ");
    scanf("%d", &novo.exemplares_totais);

    novo.exemplares_disponiveis = novo.exemplares_totais;
    novo.status = DISPONIVEL;

    s->livros[s->total_livros] = novo;
    s->total_livros++;

    printf("\n Livro cadastrado com sucesso! ID: %d\n", novo.id);

    // Salvar automaticamente
    salvar_livros(s);
}

void listar_livros(Sistema *s) {
    printf("\n");
    printf(" ╔═══════════════════════════════════════════════════════════════════╗\n");
    printf(" ║                    CATALOGO DE LIVROS                             ║\n");
    printf(" ╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    if (s->total_livros == 0) {
        printf(" Nenhum livro cadastrado.\n");
        return;
    }

    for (int i = 0; i < s->total_livros; i++) {
        printf(" ┌─────────────────────────────────────────────────────────────┐\n");
        printf(" │ ID: %-3d                                                     │\n", 
               s->livros[i].id);
        printf(" │ Titulo: %-51.51s │\n", s->livros[i].titulo);
        printf(" │ Autor: %-52.52s │\n", s->livros[i].autor);
        printf(" │ Editora: %-50.50s │\n", s->livros[i].editora);
        printf(" │ Ano: %-4d  │  ISBN: %-20s              │\n", 
               s->livros[i].ano, s->livros[i].isbn);
        printf(" │ Exemplares: %d / %d  │  Status: %-20s     │\n",
               s->livros[i].exemplares_disponiveis,
               s->livros[i].exemplares_totais,
               status_livro_string(s->livros[i].status));
        printf(" └─────────────────────────────────────────────────────────────┘\n");
        printf("\n");
    }

    printf(" Total de livros: %d\n", s->total_livros);
}

void buscar_livro(Sistema *s) {
    char termo[100];
    int encontrados = 0;

    printf("\n Buscar por (titulo, autor ou ISBN): ");
    getchar();
    fgets(termo, 100, stdin);
    termo[strcspn(termo, "\n")] = 0;

    printf("\n Resultados da busca:\n\n");

    for (int i = 0; i < s->total_livros; i++) {
        if (strstr(s->livros[i].titulo, termo) != NULL ||
            strstr(s->livros[i].autor, termo) != NULL ||
            strstr(s->livros[i].isbn, termo) != NULL) {

            printf(" ┌─────────────────────────────────────────────────────────────┐\n");
            printf(" │ ID: %-3d                                                     │\n", 
                   s->livros[i].id);
            printf(" │ Titulo: %-51.51s │\n", s->livros[i].titulo);
            printf(" │ Autor: %-52.52s │\n", s->livros[i].autor);
            printf(" │ Disponivel: %d exemplares                                   │\n",
                   s->livros[i].exemplares_disponiveis);
            printf(" └─────────────────────────────────────────────────────────────┘\n");
            printf("\n");
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf(" Nenhum livro encontrado.\n");
    } else {
        printf(" Total encontrado: %d livro(s)\n", encontrados);
    }
}

void editar_livro(Sistema *s) {
    int id;
    printf("\n ID do livro a editar: ");
    scanf("%d", &id);

    if (id < 1 || id > s->total_livros) {
        printf("\n Livro nao encontrado!\n");
        return;
    }

    Livro *livro = &s->livros[id - 1];
    int opcao;

    do {
        printf("\n Editando: %s\n", livro->titulo);
        printf("\n 1. Adicionar exemplares\n");
        printf(" 2. Alterar status\n");
        printf(" 0. Voltar\n");
        printf("\n Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                int qtd;
                printf(" Quantidade a adicionar: ");
                scanf("%d", &qtd);
                livro->exemplares_totais += qtd;
                livro->exemplares_disponiveis += qtd;
                printf(" Exemplares atualizados!\n");
                salvar_livros(s);
                break;
            }
            case 2: {
                printf(" Novo status (1-Disponivel, 2-Emprestado, 3-Reservado, 4-Manutencao): ");
                int status;
                scanf("%d", &status);
                if (status >= 1 && status <= 4) {
                    livro->status = (StatusLivro)status;
                    printf(" Status atualizado!\n");
                    salvar_livros(s);
                }
                break;
            }
        }
    } while(opcao != 0);
}

void menu_livros(Sistema *s, Usuario *usuario_logado) {
    int opcao;

    do {
        printf("\n");
        printf(" ╔═══════════════════════════════════════════════╗\n");
        printf(" ║           GESTAO DE LIVROS                    ║\n");
        printf(" ╚═══════════════════════════════════════════════╝\n");
        printf("\n");
        printf(" 1. Listar todos os livros\n");
        printf(" 2. Buscar livro\n");

        if (usuario_logado->tipo >= BIBLIOTECARIO) {
            printf(" 3. Cadastrar livro\n");
            printf(" 4. Editar livro\n");
        }

        printf(" 0. Voltar\n");
        printf("\n Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                listar_livros(s);
                break;
            case 2:
                buscar_livro(s);
                break;
            case 3:
                if (usuario_logado->tipo >= BIBLIOTECARIO) {
                    cadastrar_livro(s);
                } else {
                    printf("\n Acesso negado!\n");
                }
                break;
            case 4:
                if (usuario_logado->tipo >= BIBLIOTECARIO) {
                    editar_livro(s);
                } else {
                    printf("\n Acesso negado!\n");
                }
                break;
            case 0:
                break;
            default:
                printf("\n Opcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\n Pressione ENTER para continuar...");
            getchar();
            getchar();
        }
    } while(opcao != 0);
}

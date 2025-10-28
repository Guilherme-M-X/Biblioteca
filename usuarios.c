#include "biblioteca.h"
#include "usuarios.h"
#include "arquivo.h"

const char* tipo_usuario_string(TipoUsuario tipo) {
    switch(tipo) {
        case ALUNO: return "Aluno";
        case PROFESSOR: return "Professor";
        case BIBLIOTECARIO: return "Bibliotecario";
        case ADMINISTRADOR: return "Administrador";
        default: return "Desconhecido";
    }
}

Usuario* autenticar_usuario(Sistema *s, char *login, char *senha) {
    for (int i = 0; i < s->total_usuarios; i++) {
        if (strcmp(s->usuarios[i].login, login) == 0 && 
            strcmp(s->usuarios[i].senha, senha) == 0 &&
            s->usuarios[i].ativo == 1) {
            return &s->usuarios[i];
        }
    }
    return NULL;
}

void cadastrar_usuario(Sistema *s, Usuario *admin) {
    if (admin->tipo != ADMINISTRADOR && admin->tipo != BIBLIOTECARIO) {
        printf("\n Acesso negado! Apenas administradores e bibliotecarios.\n");
        return;
    }

    if (s->total_usuarios >= MAX_USUARIOS) {
        printf("\n Limite de usuarios atingido!\n");
        return;
    }

    Usuario novo;
    novo.id = s->total_usuarios + 1;

    printf("\n");
    printf(" ╔═══════════════════════════════════════════════╗\n");
    printf(" ║         CADASTRO DE NOVO USUARIO              ║\n");
    printf(" ╚═══════════════════════════════════════════════╝\n");
    printf("\n Nome completo: ");
    getchar();
    fgets(novo.nome, MAX_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf(" Login: ");
    scanf("%s", novo.login);

    printf(" Senha: ");
    scanf("%s", novo.senha);

    printf("\n Tipo de usuario:\n");
    printf(" 1 - Aluno\n");
    printf(" 2 - Professor\n");
    if (admin->tipo == ADMINISTRADOR) {
        printf(" 3 - Bibliotecario\n");
        printf(" 4 - Administrador\n");
    }
    printf("\n Opcao: ");
    int tipo;
    scanf("%d", &tipo);

    if (tipo < 1 || tipo > 4 || (admin->tipo != ADMINISTRADOR && tipo > 2)) {
        printf("\n Tipo invalido!\n");
        return;
    }

    novo.tipo = (TipoUsuario)tipo;

    printf(" Curso/Departamento: ");
    getchar();
    fgets(novo.curso_departamento, 100, stdin);
    novo.curso_departamento[strcspn(novo.curso_departamento, "\n")] = 0;

    novo.livros_emprestados = 0;
    novo.ativo = 1;

    s->usuarios[s->total_usuarios] = novo;
    s->total_usuarios++;

    printf("\n Usuario cadastrado com sucesso! ID: %d\n", novo.id);

    // Salvar automaticamente
    salvar_usuarios(s);
}

void listar_usuarios(Sistema *s) {
    printf("\n");
    printf(" ╔═══════════════════════════════════════════════════════════════╗\n");
    printf(" ║                    LISTA DE USUARIOS                          ║\n");
    printf(" ╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    if (s->total_usuarios == 0) {
        printf(" Nenhum usuario cadastrado.\n");
        return;
    }

    printf(" ┌────┬──────────────────────┬────────────────┬───────────────┐\n");
    printf(" │ ID │ Nome                 │ Login          │ Tipo          │\n");
    printf(" ├────┼──────────────────────┼────────────────┼───────────────┤\n");

    for (int i = 0; i < s->total_usuarios; i++) {
        if (s->usuarios[i].ativo) {
            printf(" │ %-2d │ %-20.20s │ %-14s │ %-13s │\n",
                   s->usuarios[i].id,
                   s->usuarios[i].nome,
                   s->usuarios[i].login,
                   tipo_usuario_string(s->usuarios[i].tipo));
        }
    }

    printf(" └────┴──────────────────────┴────────────────┴───────────────┘\n");
    printf("\n Total de usuarios: %d\n", s->total_usuarios);
}

void buscar_usuario(Sistema *s) {
    char termo[100];
    int encontrados = 0;

    printf("\n Buscar por (nome ou login): ");
    getchar();
    fgets(termo, 100, stdin);
    termo[strcspn(termo, "\n")] = 0;

    printf("\n Resultados da busca:\n\n");

    for (int i = 0; i < s->total_usuarios; i++) {
        if (s->usuarios[i].ativo &&
            (strstr(s->usuarios[i].nome, termo) != NULL ||
             strstr(s->usuarios[i].login, termo) != NULL)) {
            printf(" ID: %d\n", s->usuarios[i].id);
            printf(" Nome: %s\n", s->usuarios[i].nome);
            printf(" Login: %s\n", s->usuarios[i].login);
            printf(" Tipo: %s\n", tipo_usuario_string(s->usuarios[i].tipo));
            printf(" Curso/Dept: %s\n", s->usuarios[i].curso_departamento);
            printf(" Livros emprestados: %d\n", s->usuarios[i].livros_emprestados);
            printf(" ────────────────────────────────────\n");
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf(" Nenhum usuario encontrado.\n");
    }
}

void menu_usuarios(Sistema *s, Usuario *usuario_logado) {
    int opcao;

    do {
        printf("\n");
        printf(" ╔═══════════════════════════════════════════════╗\n");
        printf(" ║           GESTAO DE USUARIOS                  ║\n");
        printf(" ╚═══════════════════════════════════════════════╝\n");
        printf("\n");
        printf(" 1. Cadastrar usuario\n");
        printf(" 2. Listar usuarios\n");
        printf(" 3. Buscar usuario\n");
        printf(" 0. Voltar\n");
        printf("\n Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrar_usuario(s, usuario_logado);
                break;
            case 2:
                listar_usuarios(s);
                break;
            case 3:
                buscar_usuario(s);
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

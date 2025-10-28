#include "biblioteca.h"
#include "usuarios.h"
#include "livros.h"
#include "emprestimos.h"
#include "arquivo.h"

void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
    getchar();
}

void inicializar_sistema(Sistema *s) {
    s->total_livros = 0;
    s->total_usuarios = 0;
    s->total_emprestimos = 0;

    // Tentar carregar dados salvos
    int dados_carregados = carregar_dados(s);

    // Se não houver dados salvos E não houver usuários, criar admin padrão
    if (!dados_carregados && s->total_usuarios == 0) {
        printf("Criando usuario administrador padrao...\n");

        Usuario admin;
        admin.id = 1;
        strcpy(admin.nome, "Administrador");
        strcpy(admin.login, "admin");
        strcpy(admin.senha, "admin123");
        admin.tipo = ADMINISTRADOR;
        strcpy(admin.curso_departamento, "Sistema");
        admin.livros_emprestados = 0;
        admin.ativo = 1;

        s->usuarios[0] = admin;
        s->total_usuarios = 1;

        printf("Usuario admin criado: login=admin, senha=admin123\n");

        // Salvar imediatamente
        salvar_usuarios(s);
    }

    printf("Sistema inicializado com sucesso!\n");
}

void menu_principal(Sistema *s, Usuario *usuario_logado) {
    int opcao;

    do {
        limpar_tela();
        printf("\n");
        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║       SISTEMA DE GESTAO DE BIBLIOTECA EM C               ║\n");
        printf("╚══════════════════════════════════════════════════════════╝\n");
        printf("\n Usuario: %s (%s)\n", usuario_logado->nome, 
               tipo_usuario_string(usuario_logado->tipo));
        printf("\n");
        printf(" ┌──────────────────────────────────────────────────────┐\n");
        printf(" │                    MENU PRINCIPAL                    │\n");
        printf(" ├──────────────────────────────────────────────────────┤\n");
        printf(" │  1. Gestao de Livros                                 │\n");
        printf(" │  2. Gestao de Emprestimos                            │\n");

        if (usuario_logado->tipo == BIBLIOTECARIO || 
            usuario_logado->tipo == ADMINISTRADOR) {
            printf(" │  3. Gestao de Usuarios                               │\n");
        }

        if (usuario_logado->tipo == ADMINISTRADOR) {
            printf(" │  4. Relatorios e Estatisticas                        │\n");
        }

        printf(" │  9. Salvar dados manualmente                         │\n");
        printf(" │  0. Sair                                               │\n");
        printf(" └──────────────────────────────────────────────────────┘\n");
        printf("\n Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                menu_livros(s, usuario_logado);
                break;
            case 2:
                menu_emprestimos(s, usuario_logado);
                break;
            case 3:
                if (usuario_logado->tipo >= BIBLIOTECARIO) {
                    menu_usuarios(s, usuario_logado);
                } else {
                    printf("\n Acesso negado!\n");
                    pausar();
                }
                break;
            case 4:
                if (usuario_logado->tipo == ADMINISTRADOR) {
                    printf("\n Relatorios em desenvolvimento...\n");
                    pausar();
                } else {
                    printf("\n Acesso negado!\n");
                    pausar();
                }
                break;
            case 9:
                salvar_dados(s);
                pausar();
                break;
            case 0:
                printf("\n Encerrando sistema...\n");
                break;
            default:
                printf("\n Opcao invalida!\n");
                pausar();
        }
    } while(opcao != 0);
}

int main() {
    Sistema sistema;
    char login[50], senha[MAX_SENHA];
    Usuario *usuario_logado = NULL;

    printf("\n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("    SISTEMA DE GESTAO DE BIBLIOTECA EM LINGUAGEM C          \n");
    printf("════════════════════════════════════════════════════════════\n");
    printf("\n");

    inicializar_sistema(&sistema);

    printf("\n\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                    AUTENTICACAO                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n Login: ");
    scanf("%s", login);
    printf(" Senha: ");
    scanf("%s", senha);

    usuario_logado = autenticar_usuario(&sistema, login, senha);

    if (usuario_logado != NULL) {
        printf("\n Login realizado com sucesso!\n");
        printf(" Bem-vindo, %s!\n", usuario_logado->nome);
        pausar();
        menu_principal(&sistema, usuario_logado);

        // Salvar dados ao sair
        printf("\n");
        salvar_dados(&sistema);
    } else {
        printf("\n Login ou senha incorretos!\n");
    }

    printf("\n Sistema finalizado.\n\n");
    return 0;
}

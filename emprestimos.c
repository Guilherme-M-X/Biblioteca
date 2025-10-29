#include "biblioteca.h"
#include "emprestimos.h"
#include "arquivo.h"

float calcular_multa(time_t data_prevista) {
    time_t hoje = time(NULL);
    double diferenca = difftime(hoje, data_prevista);
    int dias_atraso = (int)(diferenca / (60 * 60 * 24));

    if (dias_atraso > 0) {
        return dias_atraso * MULTA_POR_DIA;
    }
    return 0.0;
}

int verificar_limite_emprestimos(Sistema *s, Usuario *u) {
    int limite = 0;

    switch(u->tipo) {
        case ALUNO:
            limite = MAX_LIVROS_ALUNO;
            break;
        case PROFESSOR:
            limite = MAX_LIVROS_PROFESSOR;
            break;
        case BIBLIOTECARIO:
        case ADMINISTRADOR:
            return 1; // Sem limite
    }

    return u->livros_emprestados < limite;
}

void realizar_emprestimo(Sistema *s, Usuario *usuario_logado) {
    int id_livro;

    printf("\n");
    printf(" -------------------------------------------------\n");
    printf(" -         REALIZAR EMPRESTIMO                   -\n");
    printf(" -------------------------------------------------\n");
    printf("\n");

    // Verificar limite de empréstimos
    if (!verificar_limite_emprestimos(s, usuario_logado)) {
        printf(" Limite de emprestimos atingido!\n");
        printf(" Alunos: maximo %d livros\n", MAX_LIVROS_ALUNO);
        printf(" Professores: maximo %d livros\n", MAX_LIVROS_PROFESSOR);
        return;
    }

    printf(" ID do livro: ");
    scanf("%d", &id_livro);

    if (id_livro < 1 || id_livro > s->total_livros) {
        printf("\n Livro nao encontrado!\n");
        return;
    }

    Livro *livro = &s->livros[id_livro - 1];

    if (livro->exemplares_disponiveis <= 0) {
        printf("\n Nao ha exemplares disponiveis!\n");
        return;
    }

    // Criar empréstimo
    Emprestimo novo;
    novo.id = s->total_emprestimos + 1;
    novo.id_livro = id_livro;
    novo.id_usuario = usuario_logado->id;
    novo.data_emprestimo = time(NULL);

    // Calcular data de devolução
    int dias = (usuario_logado->tipo == PROFESSOR) ? 
               DIAS_EMPRESTIMO_PROFESSOR : DIAS_EMPRESTIMO_ALUNO;
    novo.data_devolucao_prevista = novo.data_emprestimo + (dias * 24 * 60 * 60);
    novo.data_devolucao_real = 0;
    novo.status = ATIVO;
    novo.multa = 0.0;

    // Atualizar dados
    livro->exemplares_disponiveis--;
    usuario_logado->livros_emprestados++;

    s->emprestimos[s->total_emprestimos] = novo;
    s->total_emprestimos++;

    printf("\n Emprestimo realizado com sucesso!\n");
    printf(" Livro: %s\n", livro->titulo);
    printf(" Prazo de devolucao: %d dias\n", dias);

    // Mostrar data de devolução
    char buffer[80];
    struct tm *data_dev = localtime(&novo.data_devolucao_prevista);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", data_dev);
    printf(" Devolver ate: %s\n", buffer);

    // Salvar automaticamente
    salvar_emprestimos(s);
    salvar_livros(s);
    salvar_usuarios(s);
}

void devolver_livro(Sistema *s, Usuario *usuario_logado) {
    int id_emprestimo = -1;

    printf("\n");
    printf(" -------------------------------------------------\n");
    printf(" -         DEVOLVER LIVRO                        -\n");
    printf(" -------------------------------------------------\n");
    printf("\n");

    // Listar empréstimos ativos do usuário
    printf(" Seus emprestimos ativos:\n\n");
    int encontrou = 0;

    for (int i = 0; i < s->total_emprestimos; i++) {
        if (s->emprestimos[i].id_usuario == usuario_logado->id &&
            s->emprestimos[i].status == ATIVO) {

            Livro *livro = &s->livros[s->emprestimos[i].id_livro - 1];
            float multa = calcular_multa(s->emprestimos[i].data_devolucao_prevista);

            printf(" ID Emprestimo: %d\n", s->emprestimos[i].id);
            printf(" Livro: %s\n", livro->titulo);

            char buffer[80];
            struct tm *data_dev = localtime(&s->emprestimos[i].data_devolucao_prevista);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y", data_dev);
            printf(" Data prevista: %s\n", buffer);

            if (multa > 0) {
                printf(" ATRASADO! Multa: R$ %.2f\n", multa);
            }
            printf(" -------------------------------------------------\n");
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf(" Voce nao possui emprestimos ativos.\n");
        return;
    }

    printf("\n ID do emprestimo a devolver: ");
    scanf("%d", &id_emprestimo);

    // Buscar empréstimo
    Emprestimo *emp = NULL;
    for (int i = 0; i < s->total_emprestimos; i++) {
        if (s->emprestimos[i].id == id_emprestimo &&
            s->emprestimos[i].id_usuario == usuario_logado->id &&
            s->emprestimos[i].status == ATIVO) {
            emp = &s->emprestimos[i];
            break;
        }
    }

    if (emp == NULL) {
        printf("\n Emprestimo nao encontrado!\n");
        return;
    }

    // Processar devolução
    emp->data_devolucao_real = time(NULL);
    emp->multa = calcular_multa(emp->data_devolucao_prevista);
    emp->status = (emp->multa > 0) ? ATRASADO : DEVOLVIDO;

    Livro *livro = &s->livros[emp->id_livro - 1];
    livro->exemplares_disponiveis++;
    usuario_logado->livros_emprestados--;

    printf("\n Devolucao realizada com sucesso!\n");
    printf(" Livro: %s\n", livro->titulo);

    if (emp->multa > 0) {
        printf(" ATENCAO: Multa por atraso: R$ %.2f\n", emp->multa);
    } else {
        printf(" Devolucao dentro do prazo!\n");
    }

    // Salvar automaticamente
    salvar_emprestimos(s);
    salvar_livros(s);
    salvar_usuarios(s);
}

void renovar_emprestimo(Sistema *s, Usuario *usuario_logado) {
    int id_emprestimo;

    printf("\n ID do emprestimo a renovar: ");
    scanf("%d", &id_emprestimo);

    Emprestimo *emp = NULL;
    for (int i = 0; i < s->total_emprestimos; i++) {
        if (s->emprestimos[i].id == id_emprestimo &&
            s->emprestimos[i].id_usuario == usuario_logado->id &&
            s->emprestimos[i].status == ATIVO) {
            emp = &s->emprestimos[i];
            break;
        }
    }

    if (emp == NULL) {
        printf("\n Emprestimo nao encontrado ou ja devolvido!\n");
        return;
    }

    // Verificar se está atrasado
    if (calcular_multa(emp->data_devolucao_prevista) > 0) {
        printf("\n Nao e possivel renovar emprestimo atrasado!\n");
        printf(" Por favor, devolva o livro e pague a multa.\n");
        return;
    }

    // Renovar por mais dias
    int dias = (usuario_logado->tipo == PROFESSOR) ? 
               DIAS_EMPRESTIMO_PROFESSOR : DIAS_EMPRESTIMO_ALUNO;
    emp->data_devolucao_prevista += (dias * 24 * 60 * 60);

    printf("\n Emprestimo renovado com sucesso!\n");
    printf(" Novo prazo: %d dias\n", dias);

    // Salvar automaticamente
    salvar_emprestimos(s);
}

void listar_emprestimos_usuario(Sistema *s, int id_usuario) {
    printf("\n Historico de emprestimos:\n\n");
    int encontrou = 0;

    for (int i = 0; i < s->total_emprestimos; i++) {
        if (s->emprestimos[i].id_usuario == id_usuario) {
            Livro *livro = &s->livros[s->emprestimos[i].id_livro - 1];

            printf(" Emprestimo #%d\n", s->emprestimos[i].id);
            printf(" Livro: %s\n", livro->titulo);

            char buffer[80];
            struct tm *data = localtime(&s->emprestimos[i].data_emprestimo);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y", data);
            printf(" Data emprestimo: %s\n", buffer);

            if (s->emprestimos[i].status == ATIVO) {
                printf(" Status: ATIVO\n");
            } else {
                printf(" Status: %s\n", 
                       (s->emprestimos[i].status == DEVOLVIDO) ? "DEVOLVIDO" : "ATRASADO");
                if (s->emprestimos[i].multa > 0) {
                    printf(" Multa: R$ %.2f\n", s->emprestimos[i].multa);
                }
            }
            printf(" -------------------------------------------------\n");
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf(" Nenhum emprestimo encontrado.\n");
    }
}

void menu_emprestimos(Sistema *s, Usuario *usuario_logado) {
    int opcao;

    do {
        printf("\n");
        printf(" -------------------------------------------------\n");
        printf(" -         GESTAO DE EMPRESTIMOS                 -\n");
        printf(" -------------------------------------------------\n");
        printf("\n");
        printf(" 1. Realizar emprestimo\n");
        printf(" 2. Devolver livro\n");
        printf(" 3. Renovar emprestimo\n");
        printf(" 4. Meus emprestimos\n");
        printf(" 0. Voltar\n");
        printf("\n Opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                realizar_emprestimo(s, usuario_logado);
                break;
            case 2:
                devolver_livro(s, usuario_logado);
                break;
            case 3:
                renovar_emprestimo(s, usuario_logado);
                break;
            case 4:
                listar_emprestimos_usuario(s, usuario_logado->id);
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

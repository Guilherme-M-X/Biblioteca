#include "biblioteca.h"
#include "arquivo.h"

int salvar_livros(Sistema *s) {
    FILE *arquivo = fopen(ARQUIVO_LIVROS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de livros!\n");
        return 0;
    }

    // Salvar contador
    fwrite(&s->total_livros, sizeof(int), 1, arquivo);

    // Salvar array de livros
    if (s->total_livros > 0) {
        fwrite(s->livros, sizeof(Livro), s->total_livros, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int salvar_usuarios(Sistema *s) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de usuarios!\n");
        return 0;
    }

    // Salvar contador
    fwrite(&s->total_usuarios, sizeof(int), 1, arquivo);

    // Salvar array de usuários
    if (s->total_usuarios > 0) {
        fwrite(s->usuarios, sizeof(Usuario), s->total_usuarios, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int salvar_emprestimos(Sistema *s) {
    FILE *arquivo = fopen(ARQUIVO_EMPRESTIMOS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de emprestimos!\n");
        return 0;
    }

    // Salvar contador
    fwrite(&s->total_emprestimos, sizeof(int), 1, arquivo);

    // Salvar array de empréstimos
    if (s->total_emprestimos > 0) {
        fwrite(s->emprestimos, sizeof(Emprestimo), s->total_emprestimos, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int salvar_dados(Sistema *s) {
    int sucesso = 1;

    printf("\nSalvando dados...\n");

    if (!salvar_livros(s)) {
        printf("Erro ao salvar livros!\n");
        sucesso = 0;
    }

    if (!salvar_usuarios(s)) {
        printf("Erro ao salvar usuarios!\n");
        sucesso = 0;
    }

    if (!salvar_emprestimos(s)) {
        printf("Erro ao salvar emprestimos!\n");
        sucesso = 0;
    }

    if (sucesso) {
        printf("Dados salvos com sucesso!\n");
        printf("  - %d livros salvos\n", s->total_livros);
        printf("  - %d usuarios salvos\n", s->total_usuarios);
        printf("  - %d emprestimos salvos\n", s->total_emprestimos);
    }

    return sucesso;
}

int carregar_livros(Sistema *s) {
    FILE *arquivo = fopen(ARQUIVO_LIVROS, "rb");
    if (arquivo == NULL) {
        // Arquivo não existe ainda (primeira execução)
        return 0;
    }

    // Carregar contador
    fread(&s->total_livros, sizeof(int), 1, arquivo);

    // Validar
    if (s->total_livros > MAX_LIVROS) {
        printf("Erro: dados corrompidos em livros.dat\n");
        s->total_livros = 0;
        fclose(arquivo);
        return 0;
    }

    // Carregar array de livros
    if (s->total_livros > 0) {
        fread(s->livros, sizeof(Livro), s->total_livros, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregar_usuarios(Sistema *s) {
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "rb");
    if (arquivo == NULL) {
        // Arquivo não existe ainda (primeira execução)
        return 0;
    }

    // Carregar contador
    fread(&s->total_usuarios, sizeof(int), 1, arquivo);

    // Validar
    if (s->total_usuarios > MAX_USUARIOS) {
        printf("Erro: dados corrompidos em usuarios.dat\n");
        s->total_usuarios = 0;
        fclose(arquivo);
        return 0;
    }

    // Carregar array de usuários
    if (s->total_usuarios > 0) {
        fread(s->usuarios, sizeof(Usuario), s->total_usuarios, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregar_emprestimos(Sistema *s) {
    FILE *arquivo = fopen(ARQUIVO_EMPRESTIMOS, "rb");
    if (arquivo == NULL) {
        // Arquivo não existe ainda (primeira execução)
        return 0;
    }

    // Carregar contador
    fread(&s->total_emprestimos, sizeof(int), 1, arquivo);

    // Validar
    if (s->total_emprestimos > MAX_EMPRESTIMOS) {
        printf("Erro: dados corrompidos em emprestimos.dat\n");
        s->total_emprestimos = 0;
        fclose(arquivo);
        return 0;
    }

    // Carregar array de empréstimos
    if (s->total_emprestimos > 0) {
        fread(s->emprestimos, sizeof(Emprestimo), s->total_emprestimos, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregar_dados(Sistema *s) {
    int arquivos_carregados = 0;

    printf("Carregando dados salvos...\n");

    if (carregar_livros(s)) {
        printf("  - %d livros carregados\n", s->total_livros);
        arquivos_carregados++;
    }

    if (carregar_usuarios(s)) {
        printf("  - %d usuarios carregados\n", s->total_usuarios);
        arquivos_carregados++;
    }

    if (carregar_emprestimos(s)) {
        printf("  - %d emprestimos carregados\n", s->total_emprestimos);
        arquivos_carregados++;
    }

    if (arquivos_carregados > 0) {
        printf("Dados carregados com sucesso!\n\n");
        return 1;
    } else {
        printf("Nenhum dado anterior encontrado.\n");
        printf("Primeira execucao do sistema.\n\n");
        return 0;
    }
}

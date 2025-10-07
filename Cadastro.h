#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

/*Criar Struct*/
typedef struct cadastro CADASTRO;
struct cadastro{
	char titulo[60];
	char genero[60];
	int id;
};

/*Menu principal*/
void menu_principal(){

    printf("\n           ########## MENU PRINCIPAL ##########           ");
    printf("\n");
    printf("\n  [1] - Consultar                ");
    printf("\n");
    printf("\n  [2] - Emprestimo                ");
    printf("\n");
    printf("\n  [3] - Cadastrar livro           ");
    printf("\n");
    printf("\n  [ESC] - SAIR                    ");
    printf("\n\n");
}

/*Cadastro de livros*/

void cadastro(){
	
	 /*char nome_cliente[50];
    int cpf_cliente;*/
    CADASTRO cad;
    
    // 1. Inicializa o gerador de números aleatórios com o tempo atual
    // Isso garante que a sequência de números seja diferente a cada execução.
    srand(time(NULL));
    
    // 2. Gera um número aleatório
    int numeroAleatorio = rand();
	
	FILE* arquivo;
    arquivo = fopen("livros.txt", "a+");
    
     if(arquivo == NULL){
      	printf ("\n\n ERRO NA ABERTURA DO ARQUIVO \n\n");
		}
    else{
    	printf ("\n           ########## CADASTRO DO LIVRO ##########            ");
	    printf ("\n\nDigite o titulo do livro: ");
	    fflush(stdin);
	    fgets(cad.titulo, 60, stdin);
	    cad.titulo[strcspn(cad.titulo, "\n")] = 0; //retira o \n do final 
	    
	    printf ("Digite o genero do livro: ");
	   fflush(stdin);
	   fgets(cad.genero, 60, stdin);
	   cad.genero[strcspn(cad.genero, "\n")] = 0;
	   
	   cad.id = numeroAleatorio;

	}
	
	if (fclose(arquivo) == 0){
		printf ("\n");
	}else{
		printf ("Erro no fechamento do arquivo.\n");
	}
	
}

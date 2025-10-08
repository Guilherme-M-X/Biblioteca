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

/*Declaração de Variaveis*/
int verifica_genero;
char entra;
/*Fianl de declaração das vbariaveis*/


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


/*Menu Genro*/
void menu_genero(){

    printf("\n           ########## ESCOLHA O GENERO ##########           ");
    printf("\n");
    printf("\n  [1] - Ficcção                ");
    printf("\n");
    printf("\n  [2] - Terror                ");
    printf("\n");
    printf("\n  [3] - Romance           ");
    printf("\n");
    printf("\n  [4] - Infantil                 ");
    printf("\n");
    printf("\n  [5] - Culinaria                ");
    printf("\n");
    printf("\n  [6] - Religioso               ");
    printf("\n");
    printf("\n  [7] - Exatas               ");
    printf("\n");
    printf("\n  [8] - Ciências e Humanidade    ");
    printf("\n");
    printf("\n  [9] - Outros                ");
    printf("\n");
    printf("\n  [ESC] - Cancelar                ");
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
	    
	    /*printf ("Escolha o genero do livro: ");
		   fflush(stdin);
		   fgets(cad.genero, 60, stdin);
		   cad.genero[strcspn(cad.genero, "\n")] = 0;*/
		   
		menu_genero();    
		verifica_genero = 1;
		   
		do{
			
			entra = getch();
			
			switch(entra){				
				case '1':
					
					strcpy(cad.genero, "Ficção");
					verifica_genero = 0;
					break;
							
				case '2':
					
					strcpy(cad.genero, "Terror");
					verifica_genero = 0;
					break;		
				
				
				case '3':
					
					strcpy(cad.genero, "Romance");
					verifica_genero = 0;
					break;
				
				
				case '4':
					
					strcpy(cad.genero, "Infantil");
					verifica_genero = 0;
					break;
			
				
				case '5':
					
					strcpy(cad.genero, "Culinaria");
					verifica_genero = 0;
					break;			

				
				case '6':
					
					strcpy(cad.genero, "Religioso");
					verifica_genero = 0;
					break;			
				
				case '7':
					
					strcpy(cad.genero, "Exatas");
					verifica_genero = 0;
					break;			
				
				case '8':
					
					strcpy(cad.genero, "Ciências e Humanidade");
					verifica_genero = 0;
					break;			
				
				case '9':
					
					strcpy(cad.genero, "Outros");
					verifica_genero = 0;
					break;			
				
				case (char)27:
					
					verifica_genero = 0;
					break;
					
				default :
				
				printf ("\n		Opção invalida				");
				printf ("\n	Esolha outra opção para continuar	");	
				printf ("\n\n")	;
								
			}
			
			
			
		}while (verifica_genero != 0);
	   
	   cad.id = numeroAleatorio;

	}
	
	if (fclose(arquivo) == 0){
		printf ("\n");
	}else{
		printf ("Erro no fechamento do arquivo.\n");
	}
	
}

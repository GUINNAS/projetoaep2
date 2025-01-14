#include <stdio.h>
#include "functions.h"

int main() {
    int option;

    do {
        printf("\nMenu.:\n");
        printf("1. Listar Usuarios\n");
        printf("2. Incluir Usuario\n");
        printf("3. Alterar Usuario\n");
        printf("4. Excluir Usuario\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                listUsers();
                break;
            case 2:
                includeUser ();
                break;
            case 3:
                modifyUser ();
                break;
            case 4:
                deleteUser ();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida, Tente novamente.\n");
        }
    } while (option != 5);

    return 0;
}
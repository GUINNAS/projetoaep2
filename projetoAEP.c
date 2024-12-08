#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_USERS 100
#define MAX_LINE_LENGTH 256
#define FILENAME "usuarios.txt"

unsigned long simpleHash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int validPassword(const char *password) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    int length = strlen(password);

    if (length < 8) return 0;

    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) hasUpper = 1;
        else if (islower(password[i])) hasLower = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else if (ispunct(password[i])) hasSpecial = 1;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

void includeUser() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char username[100], password[100];
    printf("Digite o nome do usuario: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    do {
        printf("Digite a senha do usuario (minimo 8 caracteres, incluindo letras maiusculas, minusculas, numeros e caracteres especiais).: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;

        if (!validPassword(password)) {
            printf("senha invalida! Tente novamente.\n");
        }
    } while (!validPassword(password));

    unsigned long hash = simpleHash(password);
    fprintf(file, "%s,%lu\n", username, hash);
    fclose(file);
    printf("Usuario adicionado com sucesso.\n");
}

void modifyUser() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char *lines[MAX_USERS];
    char line[MAX_LINE_LENGTH];
    int lineCount = 0;

    while (lineCount < MAX_USERS && fgets(line, sizeof(line), file)) {
        lines[lineCount] = strdup(line);
        lineCount++;
    }
    fclose(file);

    printf("Usuarios disponiveis para alteracao.:\n");
    for (int i = 0; i < lineCount; i++) {
        printf("%d: %s", i + 1, lines[i]);
    }

    int index;
    printf("Digite o numero do usuario que deseja alterar.: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > lineCount) {
        printf("Usuario inválido.\n");
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        return;
    }

    char username[100], password[100];
    printf("Digite o novo nome do usuario.: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Digite a nova senha do usuario.: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    unsigned long hash = simpleHash(password);
    snprintf(lines[index - 1], MAX_LINE_LENGTH, "%s,%lu\n", username, hash);

    file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    for (int i = 0; i < lineCount; i++) {
        fputs(lines[i], file);
        free(lines[i]);
    }
    fclose(file);
    printf("Usuario alterado com sucesso.\n");
}
void deleteUser() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char *lines[MAX_USERS];
    char line[MAX_LINE_LENGTH];
    int lineCount = 0;

    while (lineCount < MAX_USERS && fgets(line, sizeof(line), file)) {
        lines[lineCount] = strdup(line);
        lineCount++;
    }
    fclose(file);

    printf("Usuarios disponiveis para exclusao.:\n");
    for (int i = 0; i < lineCount; i++) {
        printf("%d: %s", i + 1, lines[i]);
    }

    int index;
    printf("Digite o numero do usuario que deseja excluir.: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > lineCount) {
        printf("Usuario invalido.\n");
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        return;
    }

    free(lines[index - 1]);
    for (int i = index - 1; i < lineCount - 1; i++) {
        lines[i] = lines[i + 1];
    }
    lineCount--;

    file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    for (int i = 0; i < lineCount; i++) {
        fputs(lines[i], file);
        free(lines[i]);
    }
    fclose(file);
    printf("Usuario excluido com sucesso.\n");
}

void listUsers() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char line[MAX_LINE_LENGTH];
    printf("Usuarios cadastrados.:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

int main() {
    int option;

    do {
        printf("\nMenu:\n");
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
                includeUser();
                break;
            case 3:
                modifyUser();
                break;
            case 4:
                deleteUser();
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
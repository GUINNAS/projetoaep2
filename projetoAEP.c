#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_USERS 100
#define MAX_LINE_LENGTH 256
#define FILENAME "usuarios.txt"

// Função simples de hash (não segura)
unsigned long simpleHash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

int validarSenha(const char *senha) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    int length = strlen(senha);

    if (length < 8) return 0;

    for (int i = 0; i < length; i++) {
        if (isupper(senha[i])) hasUpper = 1;
        else if (islower(senha[i])) hasLower = 1;
        else if (isdigit(senha[i])) hasDigit = 1;
        else if (ispunct(senha[i])) hasSpecial = 1;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

void incluirUsuario() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char nome[100], senha[100];
    printf("Digite o nome do usuário: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    do {
        printf("Digite a senha do usuário (mínimo 8 caracteres, incluindo letras maiusculas, minusculas, numeros e caracteres especiais): ");
        fgets(senha, sizeof(senha), stdin);
        senha[strcspn(senha, "\n")] = 0;

        if (!validarSenha(senha)) {
            printf("Senha inválida! Tente novamente.\n");
        }
    } while (!validarSenha(senha));

    unsigned long hash = simpleHash(senha);
    fprintf(file, "%s,%lu\n", nome, hash);
    fclose(file);
    printf("Usuário adicionado com sucesso.\n");
}

void alterarUsuario() {
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

    printf("Usuários disponíveis para alteração:\n");
    for (int i = 0; i < lineCount; i++) {
        printf("%d: %s", i + 1, lines[i]);
    }

    int index;
    printf("Digite o número do usuário que deseja alterar: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > lineCount) {
        printf("Usuário inválido.\n");
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        return;
    }

    char nome[100], senha[100];
    printf("Digite o novo nome do usuário: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a nova senha do usuário: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;

    unsigned long hash = simpleHash(senha);
    snprintf(lines[index - 1], MAX_LINE_LENGTH, "%s,%lu\n", nome, hash);

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
    printf("Usuário alterado com sucesso.\n");
}
void excluirUsuario() {
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

    printf("Usuários disponíveis para exclusão:\n");
    for (int i = 0; i < lineCount; i++) {
        printf("%d: %s", i + 1, lines[i]);
    }

    int index;
    printf("Digite o número do usuário que deseja excluir: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > lineCount) {
        printf("Usuário inválido.\n");
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        return;
    }

    // Remover o usuário da lista
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
    printf("Usuário excluído com sucesso.\n");
}

void listarUsuarios() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char line[MAX_LINE_LENGTH];
    printf("Usuários cadastrados:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

int main() {
    int option;

    do {
        printf("\nMenu:\n");
        printf("1. Listar Usuários\n");
        printf("2. Incluir Usuário\n");
        printf("3. Alterar Usuário\n");
        printf("4. Excluir Usuário\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                listarUsuarios();
                break;
            case 2:
                incluirUsuario();
                break;
            case 3:
                alterarUsuario();
                break;
            case 4:
                excluirUsuario();
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (option != 5);

    return 0;
}
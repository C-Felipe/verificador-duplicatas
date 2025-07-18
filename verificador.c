#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_EMAIL_LEN 51
#define HASH_TABLE_SIZE 10007

typedef struct Node {
    char email[MAX_EMAIL_LEN];
    struct Node* next;
} Node;

Node* hashTable[HASH_TABLE_SIZE];

int hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);
    return hash % HASH_TABLE_SIZE;
}

void to_lower_str(char* str) {
    for (; *str; str++)
        *str = tolower(*str);
}

int is_valid_email(const char* email) {
    int i;
    for (i = 0; email[i]; i++) {
        if (i >= MAX_EMAIL_LEN - 1) return 0;
        if (!isalnum(email[i]) && email[i] != '-' && email[i] != '_' && email[i] != '@' && email[i] != '.')
            return 0;
    }
    return strchr(email, '@') != NULL;
}

int search_hash(const char* email) {
    int index = hash(email);
    Node* current = hashTable[index];
    while (current) {
        if (strcmp(current->email, email) == 0)
            return 1;
        current = current->next;
    }
    return 0;
}

void insert_hash(const char* email) {
    int index = hash(email);
    Node* newNode = malloc(sizeof(Node));
    strcpy(newNode->email, email);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email) {
    for (int i = 0; i < count; i++)
        if (strcmp(emails[i], email) == 0)
            return 1;
    return 0;
}

void free_hash_table() {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Node* current = hashTable[i];
        while (current) {
            Node* tmp = current;
            current = current->next;
            free(tmp);
        }
        hashTable[i] = NULL;
    }
}

void verificar_duplicatas_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return;
    }

    char line[128];
    char emails[10000][MAX_EMAIL_LEN];
    int emailCount = 0;

    fgets(line, sizeof(line), file); // Pula cabeçalho

    printf("\n[Usando Tabela Hash]\n");
    clock_t start_hash = clock();
    rewind(file);
    fgets(line, sizeof(line), file); // Pula cabeçalho novamente

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",\n\r");
        if (!token) continue;

        char email[MAX_EMAIL_LEN];
        strncpy(email, token, MAX_EMAIL_LEN);
        email[MAX_EMAIL_LEN - 1] = '\0';

        to_lower_str(email);

        if (!is_valid_email(email)) continue;

        if (search_hash(email))
            printf("%s => Já cadastrado\n", email);
        else {
            insert_hash(email);
            printf("%s => Novo\n", email);
        }
    }
    clock_t end_hash = clock();
    double time_hash = (double)(end_hash - start_hash) / CLOCKS_PER_SEC;
    fclose(file);

    file = fopen(filename, "r");
    fgets(line, sizeof(line), file); // Pula cabeçalho novamente

    printf("\n[Usando Busca Linear]\n");
    clock_t start_linear = clock();
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",\n\r");
        if (!token) continue;

        char email[MAX_EMAIL_LEN];
        strncpy(email, token, MAX_EMAIL_LEN);
        email[MAX_EMAIL_LEN - 1] = '\0';

        to_lower_str(email);

        if (!is_valid_email(email)) continue;

        if (search_linear(emails, emailCount, email))
            printf("%s => Já cadastrado\n", email);
        else {
            strcpy(emails[emailCount++], email);
            printf("%s => Novo\n", email);
        }
    }
    clock_t end_linear = clock();
    double time_linear = (double)(end_linear - start_linear) / CLOCKS_PER_SEC;
    fclose(file);

    printf("\nTempo Hash: %.4f segundos\n", time_hash);
    printf("Tempo Linear: %.4f segundos\n", time_linear);

    free_hash_table();
}

int main() {
    int opcao;
    char filename[256];

    while (1) {
        printf("\n===== VERIFICADOR DE DUPLICATAS =====\n");
        printf("1. Verificar duplicatas em lista CSV\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 0) break;
        else if (opcao == 1) {
            printf("Digite o caminho do arquivo CSV: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0';
            verificar_duplicatas_csv(filename);
        } else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}

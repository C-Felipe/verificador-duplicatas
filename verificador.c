#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_EMAIL_LEN 51
#define HASH_TABLE_SIZE 10007
#define BLOOM_SIZE 125000  // 1 milhão de bits / 8 = 125000 bytes

typedef struct Node {
    char email[MAX_EMAIL_LEN];
    struct Node* next;
} Node;

Node* hashTable[HASH_TABLE_SIZE];
unsigned char bloom[BLOOM_SIZE];

// Variáveis globais para armazenar tempos
double tempo_bloom_hash = -1.0;
double tempo_hash = -1.0;
double tempo_linear = -1.0;

// Funções filtro de Bloom
unsigned long djb2(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);
    return hash;
}

unsigned long djb2_variant(const char* str, int seed) {
    unsigned long hash = 5381 + seed;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);
    return hash;
}

void bloom_set(unsigned long bit) {
    bit = bit % (BLOOM_SIZE * 8);
    bloom[bit / 8] |= (1 << (bit % 8));
}

int bloom_check(unsigned long bit) {
    bit = bit % (BLOOM_SIZE * 8);
    return (bloom[bit / 8] & (1 << (bit % 8))) != 0;
}

void bloom_add(const char* email) {
    unsigned long h1 = djb2(email);
    unsigned long h2 = djb2_variant(email, 1);
    unsigned long h3 = djb2_variant(email, 2);
    bloom_set(h1);
    bloom_set(h2);
    bloom_set(h3);
}

int bloom_possibly_contains(const char* email) {
    unsigned long h1 = djb2(email);
    unsigned long h2 = djb2_variant(email, 1);
    unsigned long h3 = djb2_variant(email, 2);
    return bloom_check(h1) && bloom_check(h2) && bloom_check(h3);
}

// Funções hash padrão
int hash_func(const char* str) {
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
    int index = hash_func(email);
    Node* current = hashTable[index];
    while (current) {
        if (strcmp(current->email, email) == 0)
            return 1;
        current = current->next;
    }
    return 0;
}

void insert_hash(const char* email) {
    int index = hash_func(email);
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro de memória!\n");
        exit(1);
    }
    strcpy(newNode->email, email);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
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

// Busca linear
int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email) {
    for (int i = 0; i < count; i++)
        if (strcmp(emails[i], email) == 0)
            return 1;
    return 0;
}

// Verificações
void verificar_bloom_hash(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return;
    }
    char line[128];
    fgets(line, sizeof(line), file); // cabeçalho

    clock_t start = clock();

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",\n\r");
        if (!token) continue;
        char email[MAX_EMAIL_LEN];
        strncpy(email, token, MAX_EMAIL_LEN);
        email[MAX_EMAIL_LEN - 1] = '\0';
        to_lower_str(email);
        if (!is_valid_email(email)) continue;

        if (bloom_possibly_contains(email)) {
            if (search_hash(email))
                printf("%s => Já cadastrado\n", email);
            else {
                insert_hash(email);
                printf("%s => Novo\n", email);
            }
        } else {
            bloom_add(email);
            insert_hash(email);
            printf("%s => Novo\n", email);
        }
    }
    clock_t end = clock();
    tempo_bloom_hash = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo total (Bloom + Hash): %.4f segundos\n", tempo_bloom_hash);
    fclose(file);
    free_hash_table();
    memset(bloom, 0, sizeof(bloom));
}

void verificar_hash(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return;
    }
    char line[128];
    fgets(line, sizeof(line), file); // cabeçalho

    clock_t start = clock();

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
    clock_t end = clock();
    tempo_hash = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo total (Hash): %.4f segundos\n", tempo_hash);
    fclose(file);
    free_hash_table();
}

void verificar_linear(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return;
    }
    char line[128];
    fgets(line, sizeof(line), file); // cabeçalho

    char emails[10000][MAX_EMAIL_LEN];
    int count = 0;

    clock_t start = clock();

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",\n\r");
        if (!token) continue;
        char email[MAX_EMAIL_LEN];
        strncpy(email, token, MAX_EMAIL_LEN);
        email[MAX_EMAIL_LEN - 1] = '\0';
        to_lower_str(email);
        if (!is_valid_email(email)) continue;

        if (search_linear(emails, count, email))
            printf("%s => Já cadastrado\n", email);
        else {
            strcpy(emails[count++], email);
            printf("%s => Novo\n", email);
        }
    }
    clock_t end = clock();
    tempo_linear = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo total (Busca Linear): %.4f segundos\n", tempo_linear);
    fclose(file);
}

void mostrar_relatorio() {
    printf("\n===== RELATÓRIO DE TEMPOS =====\n");
    if (tempo_bloom_hash >= 0)
        printf("Filtro Bloom + Hash: %.4f segundos\n", tempo_bloom_hash);
    else
        printf("Filtro Bloom + Hash: Ainda não executado\n");

    if (tempo_hash >= 0)
        printf("Hash padrão: %.4f segundos\n", tempo_hash);
    else
        printf("Hash padrão: Ainda não executado\n");

    if (tempo_linear >= 0)
        printf("Busca linear: %.4f segundos\n", tempo_linear);
    else
        printf("Busca linear: Ainda não executado\n");

    if (tempo_bloom_hash >= 0 && tempo_hash >= 0) {
        double diff = tempo_hash - tempo_bloom_hash;
        double perc = (diff / tempo_hash) * 100.0;
        printf("Bloom + Hash é %.2f%% %s que Hash padrão\n", 
            perc < 0 ? -perc : perc, perc < 0 ? "mais lento" : "mais rápido");
    }

    if (tempo_hash >= 0 && tempo_linear >= 0) {
        double diff = tempo_linear - tempo_hash;
        double perc = (diff / tempo_linear) * 100.0;
        printf("Hash padrão é %.2f%% %s que Busca linear\n", 
            perc < 0 ? -perc : perc, perc < 0 ? "mais lento" : "mais rápido");
    }
}

int main() {
    int opcao;
    char filename[256];

    while (1) {
        printf("\n===== VERIFICADOR DE DUPLICATAS =====\n");
        printf("1. Verificar com Filtro Bloom + Hash\n");
        printf("2. Verificar com Hash padrão\n");
        printf("3. Verificar com Busca Linear\n");
        printf("4. Mostrar relatório de tempos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 0) break;
        else if (opcao >=1 && opcao <=3) {
            printf("Digite o caminho do arquivo CSV: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0';

            if (opcao == 1) verificar_bloom_hash(filename);
            else if (opcao == 2) verificar_hash(filename);
            else if (opcao == 3) verificar_linear(filename);
        }
        else if (opcao == 4) {
            mostrar_relatorio();
        }
        else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}

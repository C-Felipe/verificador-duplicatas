#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_EMAIL_LEN 51
#define HASH_TABLE_SIZE 10007
#define MAX_EMAILS 10000

typedef struct Node {
    char email[MAX_EMAIL_LEN];
    struct Node* next;
} Node;

typedef struct {
    Node* buckets[HASH_TABLE_SIZE];
} HashTable;

typedef struct {
    char emails[MAX_EMAILS][MAX_EMAIL_LEN];
    int count;
} EmailList;

unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = tolower(*str++)))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_TABLE_SIZE;
}

void init_table(HashTable* table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        table->buckets[i] = NULL;
}

int insert_hash(HashTable* table, const char* email) {
    unsigned long index = hash(email);
    Node* current = table->buckets[index];
    while (current) {
        if (strcmp(current->email, email) == 0)
            return 0;
        current = current->next;
    }
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) {
        printf("Erro de memória!\n");
        exit(1);
    }
    strcpy(new_node->email, email);
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    return 1;
}

void init_list(EmailList* list) {
    list->count = 0;
}

int exists_linear(EmailList* list, const char* email) {
    for (int i = 0; i < list->count; i++) {
        if (strcasecmp(list->emails[i], email) == 0)
            return 1;
    }
    return 0;
}

int insert_linear(EmailList* list, const char* email) {
    if (exists_linear(list, email))
        return 0;
    if (list->count >= MAX_EMAILS) {
        printf("Limite de e-mails atingido!\n");
        exit(1);
    }
    strcpy(list->emails[list->count++], email);
    return 1;
}

int is_valid_email_char(char c) {
    return isalnum(c) || c == '.' || c == '-' || c == '_' || c == '@';
}

void to_lowercase(char* str) {
    for (; *str; ++str)
        *str = tolower(*str);
}

int load_emails_from_file(HashTable* table, EmailList* list, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Não foi possível abrir o arquivo '%s'\n", filename);
        return 0;
    }

    char line[100];
    int count = 0;

    fgets(line, sizeof(line), file); // pula cabeçalho

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        if (token) {
            char email[MAX_EMAIL_LEN];
            strncpy(email, token, MAX_EMAIL_LEN);
            email[MAX_EMAIL_LEN - 1] = '\0';
            to_lowercase(email);

            insert_hash(table, email);
            insert_linear(list, email);
            count++;
        }
    }

    fclose(file);
    return count;
}

int main() {
    HashTable table;
    EmailList listLinear;

    init_table(&table);
    init_list(&listLinear);

    char filename[100];
    char resposta[10];

    printf("Deseja carregar uma lista de e-mails existente? (s/n): ");
    fgets(resposta, sizeof(resposta), stdin);
    resposta[strcspn(resposta, "\n")] = '\0';

    if (tolower(resposta[0]) == 's') {
        printf("Digite o nome ou caminho do arquivo CSV: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';

        int carregados = load_emails_from_file(&table, &listLinear, filename);
        printf("Carregados %d e-mails do arquivo '%s'.\n", carregados, filename);
    } else {
        printf("Iniciando com lista vazia.\n");
    }

    char email[MAX_EMAIL_LEN];
    int totalEmails = listLinear.count;
    int dupHash = 0, dupLinear = 0;

    clock_t start_hash = clock();
    clock_t start_linear = clock();

    printf("\nDigite e-mails para inserir (Enter em branco para encerrar):\n");
    while (1) {
        printf("Informe o e-mail: ");
        if (!fgets(email, sizeof(email), stdin))
            break;
        email[strcspn(email, "\n")] = '\0';
        if (strlen(email) == 0)
            break;

        int valid = 1;
        for (int i = 0; email[i]; i++) {
            if (!is_valid_email_char(email[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("E-mail inválido! Use apenas letras, números, '.', '-', '_' e '@'.\n\n");
            continue;
        }

        to_lowercase(email);

        int insertedHash = insert_hash(&table, email);
        if (!insertedHash) dupHash++;

        int insertedLinear = insert_linear(&listLinear, email);
        if (!insertedLinear) dupLinear++;

        totalEmails++;
        printf("Status: %s (Hash) | %s (Linear)\n\n",
               insertedHash ? "Novo" : "Duplicado",
               insertedLinear ? "Novo" : "Duplicado");
    }

    clock_t end_hash = clock();
    clock_t end_linear = clock();

    double time_hash = ((double)(end_hash - start_hash)) / CLOCKS_PER_SEC;
    double time_linear = ((double)(end_linear - start_linear)) / CLOCKS_PER_SEC;

    printf("Resumo final:\n");
    printf("Total de e-mails processados: %d\n", totalEmails);
    printf("Duplicatas encontradas (Hash): %d\n", dupHash);
    printf("Duplicatas encontradas (Linear): %d\n", dupLinear);
    printf("Tempo gasto (Hash): %.6f segundos\n", time_hash);
    printf("Tempo gasto (Linear): %.6f segundos\n", time_linear);

    return 0;
}

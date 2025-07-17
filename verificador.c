#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EMAIL_LEN 51
#define HASH_TABLE_SIZE 10007

typedef struct Node {
    char email[MAX_EMAIL_LEN];
    struct Node* next;
} Node;

typedef struct {
    Node* buckets[HASH_TABLE_SIZE];
} HashTable;

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = tolower(*str++)))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_TABLE_SIZE;
}

int is_valid_email_char(char c) {
    return isalnum(c) || c == '.' || c == '-' || c == '_' || c == '@';
}

void to_lowercase(char *str) {
    for (; *str; ++str)
        *str = tolower(*str);
}

void init_table(HashTable* table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        table->buckets[i] = NULL;
}

int insert(HashTable* table, const char* email) {
    unsigned long index = hash(email);
    Node* current = table->buckets[index];

    while (current) {
        if (strcmp(current->email, email) == 0)
            return 0;
        current = current->next;
    }

    Node* new_node = malloc(sizeof(Node));
    strcpy(new_node->email, email);
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    return 1;
}

int load_emails_from_file(HashTable* table, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file)
        return 0;

    char line[100];
    int count = 0;

    fgets(line, sizeof(line), file); // pula cabeçalho

    printf("Lista carregada:\n");
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        if (token) {
            char email[MAX_EMAIL_LEN];
            strncpy(email, token, MAX_EMAIL_LEN);
            email[MAX_EMAIL_LEN - 1] = '\0';
            to_lowercase(email);

            int inserted = insert(table, email);
            printf("- %s [%s]\n", email, inserted ? "OK" : "Já cadastrado");
            count++;
        }
    }

    fclose(file);
    return count;
}

void append_email_to_file(const char* filename, const char* email, const char* status) {
    FILE* file = fopen(filename, "a");
    if (file)
        fprintf(file, "%s,%s\n", email, status);
    fclose(file);
}

int main() {
    HashTable table;
    init_table(&table);

    char filename[100] = "emails.csv";

    char opcao[10];
    printf("Deseja carregar uma lista de e-mails existente? (s/n): ");
    fgets(opcao, sizeof(opcao), stdin);
    opcao[strcspn(opcao, "\n")] = '\0';

    if (tolower(opcao[0]) == 's') {
        printf("Digite o nome ou caminho do arquivo CSV: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';

        int loaded = load_emails_from_file(&table, filename);
        printf("Carregados %d e-mails do arquivo '%s'.\n", loaded, filename);
    } else {
        FILE* novo = fopen(filename, "w");
        if (!novo) {
            printf("Erro ao criar o arquivo.\n");
            return 1;
        }
        fprintf(novo, "email,status\n");
        fclose(novo);
        printf("Novo arquivo '%s' criado com cabeçalho.\n", filename);
    }

    char email[MAX_EMAIL_LEN];

    while (1) {
        printf("\nInforme o e-mail (ou 'sair' para encerrar): ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';

        if (strcmp(email, "sair") == 0)
            break;

        int valido = 1;
        for (int i = 0; email[i]; i++) {
            if (!is_valid_email_char(email[i])) {
                valido = 0;
                break;
            }
        }

        if (!valido) {
            printf("E-mail inválido. Use apenas letras, números, '.', '-', '_' e '@'.\n");
            continue;
        }

        to_lowercase(email);
        int inserido = insert(&table, email);

        if (inserido) {
            printf("E-mail cadastrado com sucesso.\n");
            append_email_to_file(filename, email, "OK");
        } else {
            printf("Este e-mail já está cadastrado.\n");
            append_email_to_file(filename, email, "Já cadastrado");
        }
    }

    printf("\nPrograma finalizado.\n");
    return 0;
}

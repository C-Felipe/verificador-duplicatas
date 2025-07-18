#include "verificador.h"

Node* hashTable[HASH_TABLE_SIZE] = {NULL};
unsigned char bloom[BLOOM_SIZE] = {0};

double tempo_bloom_hash = -1.0;
double tempo_hash = -1.0;
double tempo_linear = -1.0;

char emails[MAX_EMAILS][MAX_EMAIL_LEN];
int total_emails = 0;

// ----------- Filtro Bloom ------------

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

// ----------- Tabela Hash ------------

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

// ----------- Busca Linear ------------

int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email) {
    for (int i = 0; i < count; i++)
        if (strcmp(emails[i], email) == 0)
            return 1;
    return 0;
}

// ----------- Utilitários ------------

int is_valid_email(const char* email) {
    int i;
    for (i = 0; email[i]; i++) {
        if (i >= MAX_EMAIL_LEN - 1) return 0;
        if (!isalnum(email[i]) && email[i] != '-' && email[i] != '_' && email[i] != '@' && email[i] != '.')
            return 0;
    }
    return strchr(email, '@') != NULL;
}

int carregar_emails(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return 0;
    }
    char line[128];
    total_emails = 0;
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",\n\r");
        if (!token) continue;

        strncpy(emails[total_emails], token, MAX_EMAIL_LEN);
        emails[total_emails][MAX_EMAIL_LEN - 1] = '\0';

        to_lower_str(emails[total_emails]);

        if (is_valid_email(emails[total_emails])) {
            total_emails++;
            if (total_emails >= MAX_EMAILS) break;
        }
    }
    fclose(file);
    return 1;
}

// ----------- Funções de Verificação ------------

void verificar_bloom_hash_carregado() {
    memset(bloom, 0, sizeof(bloom));
    free_hash_table();

    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];

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

    free_hash_table();
}

void verificar_hash_carregado() {
    free_hash_table();

    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];

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

    free_hash_table();
}

void verificar_linear_carregado() {
    char (*emails_linear)[MAX_EMAIL_LEN] = malloc(sizeof(char[MAX_EMAIL_LEN]) * total_emails);
    if (!emails_linear) {
        printf("Erro de memória ao alocar emails_linear\n");
        return;
    }
    int count = 0;
    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];
        if (search_linear(emails_linear, count, email))
            printf("%s => Já cadastrado\n", email);
        else {
            strcpy(emails_linear[count++], email);
            printf("%s => Novo\n", email);
        }
    }
    clock_t end = clock();
    tempo_linear = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo total (Busca Linear): %.4f segundos\n", tempo_linear);

    free(emails_linear);
}

// ----------- Relatório ------------

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

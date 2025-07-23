#include "verificador.h"

// Tabela hash: array de ponteiros para n�s encadeados (tratamento de colis�o)
Node* hashTable[HASH_TABLE_SIZE] = {NULL};

// Filtro Bloom: vetor de bytes para armazenar bits do filtro
unsigned char bloom[BLOOM_SIZE] = {0};

// Vari�veis globais para armazenar o tempo de execu��o de cada m�todo
double tempo_bloom_hash = -1.0;
double tempo_hash = -1.0;
double tempo_linear = -1.0;

// Armazena os emails carregados; m�ximo definido em MAX_EMAILS e tamanho m�ximo do email em MAX_EMAIL_LEN
char emails[MAX_EMAILS][MAX_EMAIL_LEN];
int total_emails = 0;

// ----------- Filtro Bloom ------------

// Fun��o hash djb2 original modificada para ignorar mai�sculas (tolower)
unsigned long djb2(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c); // hash * 33 + c (ignorando case)
    return hash;
}

// Variante do djb2 com semente para gerar hashes diferentes (para m�ltiplas fun��es hash no filtro Bloom)
unsigned long djb2_variant(const char* str, int seed) {
    unsigned long hash = 5381 + seed;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);
    return hash;
}

// Define um bit no filtro Bloom
void bloom_set(unsigned long bit) {
    bit = bit % (BLOOM_SIZE * 8); // garante que o bit esteja dentro do tamanho do filtro
    bloom[bit / 8] |= (1 << (bit % 8)); // seta o bit correspondente no byte correto
}

// Verifica se um bit est� setado no filtro Bloom
int bloom_check(unsigned long bit) {
    bit = bit % (BLOOM_SIZE * 8);
    return (bloom[bit / 8] & (1 << (bit % 8))) != 0;
}

// Adiciona um email ao filtro Bloom setando bits para suas m�ltiplas fun��es hash
void bloom_add(const char* email) {
    unsigned long h1 = djb2(email);
    unsigned long h2 = djb2_variant(email, 1);
    unsigned long h3 = djb2_variant(email, 2);
    bloom_set(h1);
    bloom_set(h2);
    bloom_set(h3);
}

// Verifica se o email possivelmente est� no filtro Bloom (testa os tr�s bits)
int bloom_possibly_contains(const char* email) {
    unsigned long h1 = djb2(email);
    unsigned long h2 = djb2_variant(email, 1);
    unsigned long h3 = djb2_variant(email, 2);
    return bloom_check(h1) && bloom_check(h2) && bloom_check(h3);
}

// ----------- Tabela Hash ------------

// Fun��o hash para a tabela hash, baseada no djb2 (case-insensitive)
int hash_func(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + tolower(c);
    return hash % HASH_TABLE_SIZE;
}

// Converte string para letras min�sculas (�til para padronizar os emails)
void to_lower_str(char* str) {
    for (; *str; str++)
        *str = tolower(*str);
}

// Busca um email na tabela hash, retorna 1 se encontrado, 0 se n�o
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

// Insere um email na tabela hash (no in�cio da lista encadeada)
void insert_hash(const char* email) {
    int index = hash_func(email);
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro de mem�ria!\n");
        exit(1);
    }
    strcpy(newNode->email, email);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Libera toda a mem�ria alocada na tabela hash, limpando listas encadeadas
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

// Busca linear simples em um array de strings, retorna 1 se encontrado, 0 caso contr�rio
int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email) {
    for (int i = 0; i < count; i++)
        if (strcmp(emails[i], email) == 0)
            return 1;
    return 0;
}

// ----------- Utilit�rios ------------

// Valida se o email � v�lido considerando caracteres permitidos e se cont�m '@'
int is_valid_email(const char* email) {
    int i;
    for (i = 0; email[i]; i++) {
        if (i >= MAX_EMAIL_LEN - 1) return 0; // email muito longo
        // aceita letras, n�meros, '-', '_', '@' e '.'
        if (!isalnum(email[i]) && email[i] != '-' && email[i] != '_' && email[i] != '@' && email[i] != '.')
            return 0;
    }
    return strchr(email, '@') != NULL; // deve conter '@'
}

// Carrega emails de arquivo CSV, armazenando em emails[] ap�s valida��o e padroniza��o para min�sculas
int carregar_emails(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", filename);
        return 0;
    }
    char line[128];
    total_emails = 0;

    fgets(line, sizeof(line), file); // pula o cabe�alho do CSV

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",\n\r"); // pega o email (primeira coluna)

        if (!token) continue;

        strncpy(emails[total_emails], token, MAX_EMAIL_LEN);
        emails[total_emails][MAX_EMAIL_LEN - 1] = '\0';

        to_lower_str(emails[total_emails]); // padroniza para min�sculas

        if (is_valid_email(emails[total_emails])) {
            total_emails++;
            if (total_emails >= MAX_EMAILS) break; // evita overflow
        }
    }
    fclose(file);
    return 1;
}

// ----------- Fun��es de Verifica��o ------------

// Verifica duplicatas usando Filtro Bloom + Hash, imprime resultado e mede tempo
void verificar_bloom_hash_carregado() {
    memset(bloom, 0, sizeof(bloom)); // reseta filtro Bloom
    free_hash_table();               // limpa tabela hash

    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];

        if (bloom_possibly_contains(email)) { // se poss�vel duplicata
            if (search_hash(email))
                printf("%s => J� cadastrado\n", email); // duplicata confirmada
            else {
                insert_hash(email);                   // insere novo
                printf("%s => Novo\n", email);
            }
        } else { // definitivamente novo
            bloom_add(email);   // adiciona ao filtro
            insert_hash(email); // insere na hash
            printf("%s => Novo\n", email);
        }
    }

    clock_t end = clock();
    tempo_bloom_hash = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tempo total (Bloom + Hash): %.4f segundos\n", tempo_bloom_hash);

    free_hash_table(); // limpa tabela hash ap�s uso
}

// Verifica duplicatas usando apenas tabela hash padr�o, imprime resultado e mede tempo
void verificar_hash_carregado() {
    free_hash_table();

    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];

        if (search_hash(email))
            printf("%s => J� cadastrado\n", email);
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

// Verifica duplicatas usando busca linear simples, imprime resultado e mede tempo
void verificar_linear_carregado() {
    // aloca dinamicamente um array para armazenar emails j� vistos
    char (*emails_linear)[MAX_EMAIL_LEN] = malloc(sizeof(char[MAX_EMAIL_LEN]) * total_emails);
    if (!emails_linear) {
        printf("Erro de mem�ria ao alocar emails_linear\n");
        return;
    }
    int count = 0;
    clock_t start = clock();

    for (int i = 0; i < total_emails; i++) {
        const char* email = emails[i];
        if (search_linear(emails_linear, count, email))
            printf("%s => J� cadastrado\n", email);
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

// ----------- Relat�rio ------------

// Mostra um relat�rio comparando os tempos de execu��o dos tr�s m�todos
void mostrar_relatorio() {
    printf("\n===== RELAT�RIO DE TEMPOS =====\n");

    if (tempo_bloom_hash >= 0)
        printf("Filtro Bloom + Hash: %.4f segundos\n", tempo_bloom_hash);
    else
        printf("Filtro Bloom + Hash: Ainda n�o executado\n");

    if (tempo_hash >= 0)
        printf("Hash padr�o: %.4f segundos\n", tempo_hash);
    else
        printf("Hash padr�o: Ainda n�o executado\n");

    if (tempo_linear >= 0)
        printf("Busca linear: %.4f segundos\n", tempo_linear);
    else
        printf("Busca linear: Ainda n�o executado\n");

    // Compara Bloom + Hash com Hash padr�o, mostra percentual de diferen�a
    if (tempo_bloom_hash >= 0 && tempo_hash >= 0) {
        double diff = tempo_hash - tempo_bloom_hash;
        double perc = (diff / tempo_hash) * 100.0;
        printf("Bloom + Hash � %.2f%% %s que Hash padr�o\n",
            perc < 0 ? -perc : perc, perc < 0 ? "mais lento" : "mais r�pido");
    }

    // Compara Hash padr�o com busca linear, mostra percentual de diferen�a
    if (tempo_hash >= 0 && tempo_linear >= 0) {
        double diff = tempo_linear - tempo_hash;
        double perc = (diff / tempo_linear) * 100.0;
        printf("Hash padr�o � %.2f%% %s que Busca linear\n",
            perc < 0 ? -perc : perc, perc < 0 ? "mais lento" : "mais r�pido");
    }
}

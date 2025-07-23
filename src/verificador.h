#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Definições de constantes usadas no programa
#define MAX_EMAIL_LEN 51         // Tamanho máximo do email (inclui '\0')
#define HASH_TABLE_SIZE 10007    // Tamanho da tabela hash
#define BLOOM_SIZE 125000        // Tamanho do filtro Bloom em bytes (1 milhão de bits)
#define MAX_EMAILS 1000000       // Número máximo de emails suportados

// Estrutura de nó para lista encadeada usada na tabela hash para tratamento de colisão
typedef struct Node {
    char email[MAX_EMAIL_LEN];    // Armazena o email
    struct Node* next;            // Ponteiro para o próximo nó
} Node;

// Variáveis globais definidas no arquivo .c, usadas para controle e armazenamento de dados
extern Node* hashTable[HASH_TABLE_SIZE];   // Tabela hash: array de ponteiros para listas encadeadas
extern unsigned char bloom[BLOOM_SIZE];    // Filtro Bloom: vetor de bytes representando bits

extern double tempo_bloom_hash;  // Tempo gasto na verificação Bloom + Hash
extern double tempo_hash;        // Tempo gasto na verificação Hash padrão
extern double tempo_linear;      // Tempo gasto na verificação Busca Linear

extern char emails[MAX_EMAILS][MAX_EMAIL_LEN];  // Armazena os emails carregados
extern int total_emails;                         // Quantidade total de emails carregados

// ----------- Filtro Bloom ------------

// Função hash djb2 case-insensitive (convertendo caracteres para minúsculo)
unsigned long djb2(const char* str);

// Variante do djb2 com semente para gerar diferentes funções hash
unsigned long djb2_variant(const char* str, int seed);

// Define um bit no filtro Bloom
void bloom_set(unsigned long bit);

// Verifica se um bit está definido no filtro Bloom
int bloom_check(unsigned long bit);

// Adiciona um email ao filtro Bloom (setando múltiplos bits)
void bloom_add(const char* email);

// Verifica se um email possivelmente está no filtro Bloom (todos os bits testados são 1)
int bloom_possibly_contains(const char* email);

// ----------- Tabela Hash ------------

// Função hash para a tabela hash baseada em djb2
int hash_func(const char* str);

// Converte uma string para minúsculas
void to_lower_str(char* str);

// Busca um email na tabela hash, retorna 1 se encontrado, 0 caso contrário
int search_hash(const char* email);

// Insere um email na tabela hash (tratamento de colisão por lista encadeada)
void insert_hash(const char* email);

// Libera toda a memória usada pela tabela hash
void free_hash_table();

// ----------- Busca Linear ------------

// Busca linear em array de strings, retorna 1 se email encontrado, 0 se não
int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email);

// ----------- Emails ------------

// Valida se uma string tem formato válido de email (caracteres permitidos + '@')
int is_valid_email(const char* email);

// Carrega emails de um arquivo CSV para o array emails[], retorna 1 se sucesso, 0 se erro
int carregar_emails(const char* filename);

// ----------- Funções de Verificação ------------

// Executa verificação de duplicatas

void verificar_bloom_hash_carregado();

void verificar_hash_carregado();

void verificar_linear_carregado();

// ----------- Relatório ------------

// Exibe relatório dos tempos de execução dos métodos
void mostrar_relatorio();

#endif

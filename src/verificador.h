#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Defini��es de constantes usadas no programa
#define MAX_EMAIL_LEN 51         // Tamanho m�ximo do email (inclui '\0')
#define HASH_TABLE_SIZE 10007    // Tamanho da tabela hash
#define BLOOM_SIZE 125000        // Tamanho do filtro Bloom em bytes (1 milh�o de bits)
#define MAX_EMAILS 1000000       // N�mero m�ximo de emails suportados

// Estrutura de n� para lista encadeada usada na tabela hash para tratamento de colis�o
typedef struct Node {
    char email[MAX_EMAIL_LEN];    // Armazena o email
    struct Node* next;            // Ponteiro para o pr�ximo n�
} Node;

// Vari�veis globais definidas no arquivo .c, usadas para controle e armazenamento de dados
extern Node* hashTable[HASH_TABLE_SIZE];   // Tabela hash: array de ponteiros para listas encadeadas
extern unsigned char bloom[BLOOM_SIZE];    // Filtro Bloom: vetor de bytes representando bits

extern double tempo_bloom_hash;  // Tempo gasto na verifica��o Bloom + Hash
extern double tempo_hash;        // Tempo gasto na verifica��o Hash padr�o
extern double tempo_linear;      // Tempo gasto na verifica��o Busca Linear

extern char emails[MAX_EMAILS][MAX_EMAIL_LEN];  // Armazena os emails carregados
extern int total_emails;                         // Quantidade total de emails carregados

// ----------- Filtro Bloom ------------

// Fun��o hash djb2 case-insensitive (convertendo caracteres para min�sculo)
unsigned long djb2(const char* str);

// Variante do djb2 com semente para gerar diferentes fun��es hash
unsigned long djb2_variant(const char* str, int seed);

// Define um bit no filtro Bloom
void bloom_set(unsigned long bit);

// Verifica se um bit est� definido no filtro Bloom
int bloom_check(unsigned long bit);

// Adiciona um email ao filtro Bloom (setando m�ltiplos bits)
void bloom_add(const char* email);

// Verifica se um email possivelmente est� no filtro Bloom (todos os bits testados s�o 1)
int bloom_possibly_contains(const char* email);

// ----------- Tabela Hash ------------

// Fun��o hash para a tabela hash baseada em djb2
int hash_func(const char* str);

// Converte uma string para min�sculas
void to_lower_str(char* str);

// Busca um email na tabela hash, retorna 1 se encontrado, 0 caso contr�rio
int search_hash(const char* email);

// Insere um email na tabela hash (tratamento de colis�o por lista encadeada)
void insert_hash(const char* email);

// Libera toda a mem�ria usada pela tabela hash
void free_hash_table();

// ----------- Busca Linear ------------

// Busca linear em array de strings, retorna 1 se email encontrado, 0 se n�o
int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email);

// ----------- Emails ------------

// Valida se uma string tem formato v�lido de email (caracteres permitidos + '@')
int is_valid_email(const char* email);

// Carrega emails de um arquivo CSV para o array emails[], retorna 1 se sucesso, 0 se erro
int carregar_emails(const char* filename);

// ----------- Fun��es de Verifica��o ------------

// Executa verifica��o de duplicatas

void verificar_bloom_hash_carregado();

void verificar_hash_carregado();

void verificar_linear_carregado();

// ----------- Relat�rio ------------

// Exibe relat�rio dos tempos de execu��o dos m�todos
void mostrar_relatorio();

#endif

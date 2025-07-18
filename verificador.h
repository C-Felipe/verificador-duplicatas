#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_EMAIL_LEN 51
#define HASH_TABLE_SIZE 10007
#define BLOOM_SIZE 125000
#define MAX_EMAILS 1000000

typedef struct Node {
    char email[MAX_EMAIL_LEN];
    struct Node* next;
} Node;

extern Node* hashTable[HASH_TABLE_SIZE];
extern unsigned char bloom[BLOOM_SIZE];

extern double tempo_bloom_hash;
extern double tempo_hash;
extern double tempo_linear;

extern char emails[MAX_EMAILS][MAX_EMAIL_LEN];
extern int total_emails;

// Bloom filter
unsigned long djb2(const char* str);
unsigned long djb2_variant(const char* str, int seed);
void bloom_set(unsigned long bit);
int bloom_check(unsigned long bit);
void bloom_add(const char* email);
int bloom_possibly_contains(const char* email);

// Hash table
int hash_func(const char* str);
void to_lower_str(char* str);
int search_hash(const char* email);
void insert_hash(const char* email);
void free_hash_table();

// Linear search
int search_linear(char emails[][MAX_EMAIL_LEN], int count, const char* email);

// Emails
int is_valid_email(const char* email);
int carregar_emails(const char* filename);

// Verificações
void verificar_bloom_hash_carregado();
void verificar_hash_carregado();
void verificar_linear_carregado();

// Relatório
void mostrar_relatorio();

#endif

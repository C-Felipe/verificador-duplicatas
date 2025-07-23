#include "verificador.h"

int main() {
    int opcao;
    char input[16];              // buffer para ler entrada do usuário
    char filename[256];
    int lista_carregada = 0;

    while (1) {
        printf("\n===== VERIFICADOR DE DUPLICATAS =====\n");
        printf("1. Carregar lista CSV\n");
        printf("2. Verificar com Filtro Bloom + Hash\n");
        printf("3. Verificar com Hash padrão\n");
        printf("4. Verificar com Busca Linear\n");
        printf("5. Mostrar relatório de tempos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        // Leitura com fgets para capturar qualquer entrada, incluindo letras
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Erro ao ler entrada.\n");
            continue;
        }

        // Tenta converter a entrada para inteiro
        if (sscanf(input, "%d", &opcao) != 1) {
            printf("Entrada inválida! Digite um número entre 0 e 5.\n");
            continue;
        }

        if (opcao == 0) break;

        if (opcao == 1) {
            printf("Digite o caminho do arquivo CSV: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0';

            if (carregar_emails(filename)) {
                printf("Lista carregada com %d emails.\n", total_emails);
                lista_carregada = 1;
                tempo_bloom_hash = tempo_hash = tempo_linear = -1.0;
                free_hash_table();
                memset(bloom, 0, sizeof(bloom));
            } else {
                printf("Falha ao carregar a lista.\n");
            }
        }
        else if (!lista_carregada) {
            printf("Por favor, carregue a lista CSV primeiro (opção 1).\n");
        }
        else if (opcao == 2) {
            verificar_bloom_hash_carregado();
        }
        else if (opcao == 3) {
            verificar_hash_carregado();
        }
        else if (opcao == 4) {
            verificar_linear_carregado();
        }
        else if (opcao == 5) {
            mostrar_relatorio();
        }
        else {
            printf("Opção inválida! Digite um número entre 0 e 5.\n");
        }
    }

    return 0;
}

#include "verificador.h"

int main() {
    int opcao;               // variável para armazenar a opção escolhida pelo usuário
    char filename[256];      // buffer para armazenar o caminho do arquivo CSV
    int lista_carregada = 0; // flag para indicar se a lista foi carregada

    while (1) {
        // Menu de opções para o usuário
        printf("\n===== VERIFICADOR DE DUPLICATAS =====\n");
        printf("1. Carregar lista CSV\n");
        printf("2. Verificar com Filtro Bloom + Hash\n");
        printf("3. Verificar com Hash padrão\n");
        printf("4. Verificar com Busca Linear\n");
        printf("5. Mostrar relatório de tempos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        scanf("%d", &opcao);  // lê a opção do usuário
        getchar();            // consome o '\n' deixado no buffer pelo scanf

        if (opcao == 0) break; // sai do loop e finaliza o programa

        if (opcao == 1) {
            // Solicita o caminho do arquivo CSV para carregar os emails
            printf("Digite o caminho do arquivo CSV: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0'; // remove o '\n' do final

            // Tenta carregar a lista de emails do arquivo
            if (carregar_emails(filename)) {
                printf("Lista carregada com %d emails.\n", total_emails);
                lista_carregada = 1;         // marca que a lista foi carregada
                // Reseta os tempos para indicar que não há resultados atuais
                tempo_bloom_hash = tempo_hash = tempo_linear = -1.0;
                free_hash_table();           // limpa qualquer dado anterior na hash
                memset(bloom, 0, sizeof(bloom)); // zera o filtro Bloom
            } else {
                printf("Falha ao carregar a lista.\n");
            }
        }
        else if (!lista_carregada) {
            // Se a lista não foi carregada e o usuário escolhe qualquer outra opção que precise de lista
            printf("Por favor, carregue a lista CSV primeiro (opção 1).\n");
        }
        else if (opcao == 2) {
            // Executa a verificação usando filtro Bloom + Hash
            verificar_bloom_hash_carregado();
        }
        else if (opcao == 3) {
            // Executa a verificação usando tabela hash padrão
            verificar_hash_carregado();
        }
        else if (opcao == 4) {
            // Executa a verificação usando busca linear
            verificar_linear_carregado();
        }
        else if (opcao == 5) {
            // Mostra o relatório com os tempos de execução das verificações
            mostrar_relatorio();
        }
        else {
            // Caso o usuário digite uma opção inválida
            printf("Opção inválida!\n");
        }
    }

    return 0;
}

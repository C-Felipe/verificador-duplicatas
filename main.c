#include "verificador.h"

int main() {
    int opcao;               // vari�vel para armazenar a op��o escolhida pelo usu�rio
    char filename[256];      // buffer para armazenar o caminho do arquivo CSV
    int lista_carregada = 0; // flag para indicar se a lista foi carregada

    while (1) {
        // Menu de op��es para o usu�rio
        printf("\n===== VERIFICADOR DE DUPLICATAS =====\n");
        printf("1. Carregar lista CSV\n");
        printf("2. Verificar com Filtro Bloom + Hash\n");
        printf("3. Verificar com Hash padr�o\n");
        printf("4. Verificar com Busca Linear\n");
        printf("5. Mostrar relat�rio de tempos\n");
        printf("0. Sair\n");
        printf("Escolha uma op��o: ");
        
        scanf("%d", &opcao);  // l� a op��o do usu�rio
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
                // Reseta os tempos para indicar que n�o h� resultados atuais
                tempo_bloom_hash = tempo_hash = tempo_linear = -1.0;
                free_hash_table();           // limpa qualquer dado anterior na hash
                memset(bloom, 0, sizeof(bloom)); // zera o filtro Bloom
            } else {
                printf("Falha ao carregar a lista.\n");
            }
        }
        else if (!lista_carregada) {
            // Se a lista n�o foi carregada e o usu�rio escolhe qualquer outra op��o que precise de lista
            printf("Por favor, carregue a lista CSV primeiro (op��o 1).\n");
        }
        else if (opcao == 2) {
            // Executa a verifica��o usando filtro Bloom + Hash
            verificar_bloom_hash_carregado();
        }
        else if (opcao == 3) {
            // Executa a verifica��o usando tabela hash padr�o
            verificar_hash_carregado();
        }
        else if (opcao == 4) {
            // Executa a verifica��o usando busca linear
            verificar_linear_carregado();
        }
        else if (opcao == 5) {
            // Mostra o relat�rio com os tempos de execu��o das verifica��es
            mostrar_relatorio();
        }
        else {
            // Caso o usu�rio digite uma op��o inv�lida
            printf("Op��o inv�lida!\n");
        }
    }

    return 0;
}

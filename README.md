# Verificador de Duplicatas de Emails

## Descrição do Projeto

Este projeto consiste em um **verificador de duplicatas de emails**, desenvolvido em linguagem C, que implementa diferentes técnicas para a detecção eficiente de emails repetidos em grandes listas. O programa foi criado como trabalho para a disciplina **Estrutura de Dados II**, ministrada pelo professor **Kennedy Reurison Lopes**, e desenvolvido pelo aluno **Felipe Lopes**.

O sistema utiliza três métodos principais para verificação de duplicatas:

- **Filtro de Bloom + Tabela Hash**: Combina um filtro probabilístico para reduzir o número de buscas na tabela hash.
- **Tabela Hash padrão**: Implementação tradicional de tabela hash com tratamento de colisão por encadeamento.
- **Busca Linear**: Busca sequencial simples, usada para comparação de desempenho.

O programa aceita arquivos CSV com listas de emails, realiza a verificação e exibe um relatório comparativo de tempos para cada método.

Além disso, o sistema conta com uma **interface gráfica simples**, que permite ao usuário interagir com o programa por meio de um menu com opções como: carregar lista, verificar duplicatas e exibir relatório.

---

## Funcionalidades

- Carregamento de lista de emails a partir de arquivo CSV.
- Validação básica do formato dos emails.
- Detecção de duplicatas utilizando:
  - Filtro de Bloom + Hash
  - Hash padrão
  - Busca linear
- Apresentação de relatório comparativo de tempos de execução.
- Interface gráfica (menu interativo via `gui.c`)
- Tratamento de colisões na tabela hash via lista encadeada.
- Busca case-insensitive para garantir maior robustez na comparação de emails.
- Limitação configurável da quantidade máxima de emails a serem processados.

---

## Tecnologias Utilizadas

- Linguagem C
- Estruturas de dados: tabela hash com encadeamento, filtro de Bloom, arrays para busca linear
- Manipulação de arquivos CSV

---

## Estrutura do Código

- **hashTable**: array para tabela hash com tratamento de colisões por lista encadeada.
- **bloom**: array de bytes que representa o filtro de Bloom para filtragem probabilística.
- **emails**: array de strings para armazenar os emails carregados da lista.
- **Funções principais**:
  - `carregar_emails()`: lê arquivo CSV e valida emails.
  - `verificar_bloom_hash_carregado()`: verifica duplicatas usando filtro de Bloom e tabela hash.
  - `verificar_hash_carregado()`: verifica duplicatas usando somente tabela hash.
  - `verificar_linear_carregado()`: verifica duplicatas usando busca linear.
  - `mostrar_relatorio()`: exibe relatório de tempos comparativos.
  - `interface_usuario()`: exibe o menu interativo no terminal e trata as opções.

---

## Como Executar (Sem interface)

1. Compile:
   ```bash
   gcc verificador.c -o verificador

2. Execute:
   ```bash
   ./verificador

## Como Executar (Com interface)

1. Compile:
   ```bash
   gcc gui.c verificador.c -o verificador.exe

2. Execute:
   ```bash
   ./verificador
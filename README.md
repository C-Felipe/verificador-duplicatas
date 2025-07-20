# Verificador de Duplicatas de Emails

## Descrição do Projeto

Projeto desenvolvido como trabalho acadêmico para a disciplina **Estrutura de Dados II**
 
- **Professor:** [Dr. Kennedy Reurison Lopes](https://github.com/kennedyufersa)  
- **Discente:** [Felipe Lopes](https://github.com/C-Felipe) 

Este sistema é um **verificador de duplicatas de emails** implementado em linguagem C, que utiliza três métodos principais para detecção eficiente de emails repetidos em grandes listas:

- **Filtro de Bloom + Tabela Hash:** Combina um filtro probabilístico para reduzir o número de buscas na tabela hash.  
- **Tabela Hash padrão:** Implementação tradicional de tabela hash com tratamento de colisão por encadeamento.  
- **Busca Linear:** Busca sequencial simples, usada para comparação de desempenho.  

O programa aceita arquivos CSV com listas de emails, realiza a verificação e gera um relatório comparativo dos tempos de execução para cada método.

Além disso, o sistema dispõe de uma **interface gráfica simples**, permitindo ao usuário interagir por meio de um menu com opções para carregar lista, verificar duplicatas e exibir relatório.


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
   gcc main.c verificador.c -o verificador

2. Execute:
   ```bash
   ./verificador

## Como Executar (Com interface)

1. Compile:
   ```bash
   gcc gui.c verificador.c -o verificador.exe -mwindows

2. Execute:
   ```bash
   ./verificador
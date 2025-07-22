# Verificador de Duplicatas de Emails

## Descrição do Projeto

Projeto desenvolvido como trabalho acadêmico para a disciplina **Estrutura de Dados II**
 
- **Professor:** [Dr. Kennedy Reurison Lopes](https://github.com/kennedyufersa)  
- **Discentes:** [Felipe Lopes](https://github.com/C-Felipe) e [Felipe Andrade](https://github.com/radiofelipe) 

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

## Atualizações Futuras

O projeto possui potencial para expansão com novas funcionalidades que ampliam sua utilidade e aplicabilidade. Entre as melhorias planejadas para versões futuras, destacam-se:

- **Geração de nova lista sem duplicatas**  
  Implementar uma função que, após a verificação da lista carregada, permita gerar um novo arquivo CSV contendo apenas os emails únicos, eliminando automaticamente todas as entradas duplicadas.  
  Essa funcionalidade será útil para a limpeza e padronização de grandes bases de dados.

- **Mesclagem de duas listas CSV com remoção de duplicatas**  
  Desenvolver uma rotina capaz de carregar e comparar dois arquivos CSV diferentes, unificando seus conteúdos em uma **terceira lista mesclada**, onde todas as duplicatas entre as listas serão automaticamente removidas.  
  Essa função será especialmente útil para integração de bases de dados oriundas de diferentes fontes.

Essas atualizações visam tornar o sistema mais robusto, flexível e aplicável a cenários reais de tratamento e deduplicação de dados.

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
   
  ---

## Download

Baixe o programa aqui:  
[Verificador de Duplicatas v1.0](https://github.com/C-Felipe/verificador-duplicatas/releases/download/v1.0/Verificador.rar)

---

## Tutorial de Uso

1. Faça o download do arquivo ZIP acima.  
2. Extraia o conteúdo do arquivo em uma pasta de sua preferência.  
3. Execute o programa `verificador.exe` (Windows).  
4. Siga as instruções na tela para carregar listas de e-mails via arquivo CSV.  
5. O programa irá identificar e marcar os e-mails duplicados, gerando relatórios para análise.


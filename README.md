# Verificador de Duplicatas de Emails

## Descri��o do Projeto

Projeto desenvolvido como trabalho acad�mico para a disciplina **Estrutura de Dados II**
 
- **Professor:** [Dr. Kennedy Reurison Lopes](https://github.com/kennedyufersa)  
- **Discente:** [Felipe Lopes](https://github.com/C-Felipe) 

Este sistema � um **verificador de duplicatas de emails** implementado em linguagem C, que utiliza tr�s m�todos principais para detec��o eficiente de emails repetidos em grandes listas:

- **Filtro de Bloom + Tabela Hash:** Combina um filtro probabil�stico para reduzir o n�mero de buscas na tabela hash.  
- **Tabela Hash padr�o:** Implementa��o tradicional de tabela hash com tratamento de colis�o por encadeamento.  
- **Busca Linear:** Busca sequencial simples, usada para compara��o de desempenho.  

O programa aceita arquivos CSV com listas de emails, realiza a verifica��o e gera um relat�rio comparativo dos tempos de execu��o para cada m�todo.

Al�m disso, o sistema disp�e de uma **interface gr�fica simples**, permitindo ao usu�rio interagir por meio de um menu com op��es para carregar lista, verificar duplicatas e exibir relat�rio.


---

## Funcionalidades

- Carregamento de lista de emails a partir de arquivo CSV.
- Valida��o b�sica do formato dos emails.
- Detec��o de duplicatas utilizando:
  - Filtro de Bloom + Hash
  - Hash padr�o
  - Busca linear
- Apresenta��o de relat�rio comparativo de tempos de execu��o.
- Interface gr�fica (menu interativo via `gui.c`)
- Tratamento de colis�es na tabela hash via lista encadeada.
- Busca case-insensitive para garantir maior robustez na compara��o de emails.
- Limita��o configur�vel da quantidade m�xima de emails a serem processados.

---

## Tecnologias Utilizadas

- Linguagem C
- Estruturas de dados: tabela hash com encadeamento, filtro de Bloom, arrays para busca linear
- Manipula��o de arquivos CSV

---

## Estrutura do C�digo

- **hashTable**: array para tabela hash com tratamento de colis�es por lista encadeada.
- **bloom**: array de bytes que representa o filtro de Bloom para filtragem probabil�stica.
- **emails**: array de strings para armazenar os emails carregados da lista.
- **Fun��es principais**:
  - `carregar_emails()`: l� arquivo CSV e valida emails.
  - `verificar_bloom_hash_carregado()`: verifica duplicatas usando filtro de Bloom e tabela hash.
  - `verificar_hash_carregado()`: verifica duplicatas usando somente tabela hash.
  - `verificar_linear_carregado()`: verifica duplicatas usando busca linear.
  - `mostrar_relatorio()`: exibe relat�rio de tempos comparativos.
  - `interface_usuario()`: exibe o menu interativo no terminal e trata as op��es.

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
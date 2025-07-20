# Verificador de Duplicatas de Emails

## Descri��o do Projeto

Este projeto consiste em um **verificador de duplicatas de emails**, desenvolvido em linguagem C, que implementa diferentes t�cnicas para a detec��o eficiente de emails repetidos em grandes listas. O programa foi criado como trabalho para a disciplina **Estrutura de Dados II**, ministrada pelo professor **Kennedy Reurison Lopes**, e desenvolvido pelo aluno **Felipe Lopes**.

O sistema utiliza tr�s m�todos principais para verifica��o de duplicatas:

- **Filtro de Bloom + Tabela Hash**: Combina um filtro probabil�stico para reduzir o n�mero de buscas na tabela hash.
- **Tabela Hash padr�o**: Implementa��o tradicional de tabela hash com tratamento de colis�o por encadeamento.
- **Busca Linear**: Busca sequencial simples, usada para compara��o de desempenho.

O programa aceita arquivos CSV com listas de emails, realiza a verifica��o e exibe um relat�rio comparativo de tempos para cada m�todo.

Al�m disso, o sistema conta com uma **interface gr�fica simples**, que permite ao usu�rio interagir com o programa por meio de um menu com op��es como: carregar lista, verificar duplicatas e exibir relat�rio.

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
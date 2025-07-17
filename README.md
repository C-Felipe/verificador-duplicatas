# Verificador de Duplicatas com Tabela Hash

Projeto em C que verifica e registra e-mails únicos usando tabela hash, com suporte a arquivos CSV.

## Funcionalidades atuais

- Verificação rápida de duplicatas usando tabela hash
- Validação básica de e-mails (até 50 caracteres, caracteres permitidos)
- Entrada manual de e-mails
- Importação e exportação de arquivos CSV com colunas `email` e `status`
- Interface de texto simples para uso no terminal

## Como usar

1. Compile:
   ```bash
   gcc verificador.c -o verificador

2. Execute:
   ```bash
   ./verificador

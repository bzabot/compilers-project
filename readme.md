# Projeto de Compiladores

**Autor:** Bruno Zabot — 202302069  
**Data:** 9 de Novembro de 2025

---

## Índice

- [Introdução](#introdução)
- [Como Funciona](#como-funciona)
- [Arquitetura e Implementação](#arquitetura-e-implementação)
  - [Componentes Principais](#componentes-principais)
- [Limitações](#limitações)
- [Executar os Testes](#executar-os-testes)

---

## Introdução

Este projeto implementa uma **Árvore Sintática Abstrata (AST)** de compiladores, que interpreta e executa uma sub-linguagem baseada na linguagem **Ada**.  
O objetivo principal é realizar a análise, interpretação e simulação da execução de pequenos programas escritos nesta linguagem.

---

## Como Funciona

O projeto segue um processo dividido em três fases principais:

- **Análise e Tokenização:**  
  O código-fonte é analisado lexicalmente e convertido em _tokens_.  
  Um analisador sintático (parser) converte esses _tokens_ numa Árvore de Sintaxe Abstrata (AST).

- **Árvore Sintática Abstrata (AST):**  
  A AST é uma estrutura em árvore que representa a lógica do programa.  
  Suporta expressões (ex: operações aritméticas e booleanas) e comandos (como atribuições e controlo de fluxo).

- **Interpretador:**  
  O interpretador executa os comandos da AST, avaliando expressões e exibindo os resultados.

---

## Arquitetura e Implementação

O processo de interpretação do código-fonte segue um fluxo bem definido, desde a leitura do ficheiro até à execução final.

1. O utilizador fornece um ficheiro de código-fonte com sintaxe Ada-like como entrada.
2. O código é processado pelo analisador lexical (`scanner.flex`), que o divide em _tokens_.  
   Estes tokens são então enviados para o analisador sintático (`parser.bison`).
3. O parser, com base nas regras gramaticais definidas, constrói uma Árvore Sintática Abstrata (AST).  
   A estrutura da AST é definida em `ast.h`.
4. O ficheiro `interpreter.c` orquestra este processo e, após a construção da AST, invoca as funções de impressão `printAST`.

---

### Componentes Principais

- **`scanner.flex`:**  
  Responsável pela análise lexical.  
  É configurado para ser _case-insensitive_ e reconhece palavras reservadas, números (inteiros e _float_) e operadores.

- **`parser.bison`:**  
  Contém a gramática da linguagem.  
  Define os tokens, as precedências de operadores e as regras para a construção da AST.  
  Suporta expressões simples, comandos e programas completos na estrutura Ada.

- **`ast.h`:**  
  Define as estruturas de dados para a AST, incluindo nós para programas, comandos (`if`, `while`), expressões aritméticas e booleanas.

- **`interpreter.c`:**  
  O ponto de entrada que chama o `yyparse` para iniciar a análise e, em seguida, percorre a AST para interpretar o código.

---

## Limitações

- Não suporta comentários.

---

## Executar os Testes

Para verificar rapidamente o funcionamento com os exemplos fornecidos, execute o seguinte comando no diretório:

```bash
make test
```

Este comando compila o projeto e executa o interpretador com os ficheiros de exemplo localizados em examples/.

Ou, caso queira rodar um caso específico:

```bash
make && ./interpreter < {your_example}
```

## Requisitos

Certifique-se de ter instalados:

```bash
sudo apt install flex bison gcc make
```

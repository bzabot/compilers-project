# Projeto de Compiladores

**Autor:** Bruno Zabot — 202302069  
**Data:** 11 de Dezembro de 2025

---

## Índice

- [Introdução](#introdução)
- [Como Funciona](#como-funciona)
- [Arquitetura e Implementação](#arquitetura-e-implementação)
  - [Componentes Principais](#componentes-principais)
  - [Fluxo de Compilação](#fluxo-de-compilação)
- [Limitações](#limitações)
- [Executar os Testes](#executar-os-testes)

---

## Introdução

Este projeto implementa um **compilador completo** para uma sub-linguagem baseada na linguagem **Ada**.  
O compilador realiza todas as fases tradicionais: análise léxica e sintática, construção de AST, análise semântica, geração de código intermediário (TAC - Three-Address Code) e geração de código assembly MIPS.

O objetivo é demonstrar o funcionamento de um compilador real, desde a leitura do código-fonte até a emissão de código executável em MIPS.

---

## Como Funciona

O compilador segue um pipeline de compilação completo com as seguintes fases:

1. **Análise Léxica e Sintática:**  
   O código-fonte é tokenizado e parseado, construindo uma Árvore de Sintaxe Abstrata (AST).

2. **Análise Semântica:**  
   Verifica a correção semântica do programa, incluindo declaração de variáveis e tipagem.

3. **Geração de Código Intermediário (TAC):**  
   A AST é traduzida para Three-Address Code (TAC), uma representação intermediária simplificada.

4. **Geração de Código MIPS:**  
   O TAC é convertido em código assembly MIPS executável.

O compilador suporta expressões aritméticas e booleanas, estruturas de controle (if/else, while), entrada/saída (get_line, put_line) e atribuições.

## Arquitetura e Implementação

### Fluxo de Compilação

```
Código Ada → [Scanner] → Tokens → [Parser] → AST → [Análise Semântica]
→ [Geração TAC] → TAC → [Geração MIPS] → Assembly MIPS
```

### Componentes Principais

#### Front-End (Análise)

- **`scanner.flex`:**  
  Analisador léxico que tokeniza o código-fonte.  
  Case-insensitive, reconhece palavras reservadas, literais numéricos e operadores.

- **`parser.bison`:**  
  Analisador sintático que define a gramática da linguagem.  
  Constrói a AST com base nas regras de produção e precedência de operadores.

- **`ast.h` / `ast.c`:**  
  Define e implementa as estruturas de dados da AST:

  - `Expr`: expressões aritméticas
  - `BoolExpr`: expressões booleanas
  - `Cmd`: comandos (atribuição, if/else, while, I/O)
  - `Program`: nó raiz do programa

- **`symbol_table.h` / `symbol_table.c`:**  
  Implementa a tabela de símbolos para rastreamento de variáveis declaradas.

- **`semantics.h` / `semantics.c`:**  
  Realiza análise semântica, verificando:
  - Declaração de variáveis antes do uso
  - Compatibilidade de tipos
  - Escopo de variáveis

#### Middle-End (Código Intermediário)

- **`tac.h` / `tac.c`:**  
  Implementa o Three-Address Code (TAC):

## Executar os Testes

Para compilar o projeto:

```bash
cd src
make
```

Para executar os exemplos fornecidos:

```bash
make test
```

Este comando executa o compilador com os ficheiros de exemplo em `examples/`, gerando:

- TAC (código intermediário) em stderr
- Assembly MIPS em stdout

Para compilar e executar um programa específico:

```bash
./interpreter < examples/complete_program.ada > output.asm
```

Para executar o código MIPS gerado, você pode usar um simulador MIPS como **MARS** ou **SPIM**. Traduz cada instrução TAC para MIPS equivalente

- Gerencia alocação de variáveis na stack (frame pointer)
- Gera prólogo e epílogo do programa
- Implementa syscalls para I/O

- **`interpreter.c`:**  
  Ponto de entrada do compilador que orquestra todo o processo:

  1. Parsing e construção da AST
  2. Análise semântica
  3. Geração de código (TAC → MIPS) e booleanas.

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

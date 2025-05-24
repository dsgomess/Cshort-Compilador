# Analisador Léxico — Linguagem Cshort

Este projeto implementa um **analisador léxico** para a linguagem de programação **Cshort**, desenvolvido em C. O analisador identifica tokens como identificadores, palavras reservadas, operadores, constantes e símbolos.

---

## 🗂️ Estrutura dos Arquivos

/projeto/
├── main.c # Arquivo principal que executa o analisador léxico
├── analex.c # Implementação do analisador léxico
├── analex.h # Cabeçalho do analisador léxico
├── FuncAuxCshort.c # Funções auxiliares (erros, prints)
├── FuncAuxCshort.h # Cabeçalho das funções auxiliares
├── teste.cshort # Arquivo de entrada para análise
├── tasks.json # Configuração de build no VS Code (opcional)
└── README.md # Este arquivo


---

### 🚀 Como Executar o Analisador Léxico

### ✅ Pré-requisitos

- GCC instalado e configurado no PATH
- VS Code, terminal, ou qualquer compilador C
- (Opcional) Extensão C/C++ da Microsoft no VS Code para ajudar na compilação

---

### 💻 Usando o terminal (CMD, PowerShell, Git Bash, Linux, WSL):

1. Compile o projeto:

```bash
gcc main.c analex.c FuncAuxCshort.c -o lexmain

 📄 Como funciona
O analisador lê o arquivo teste.cshort e imprime no terminal todos os tokens reconhecidos, junto com suas categorias:

Palavra Reservada int
ID main
Sinal (
Sinal )
Sinal {
Palavra Reservada char
ID c
Operador Aritmético =
Constante Char 'x'
Sinal ;
...

📄 Exemplo de conteúdo do teste:

int main() {
    char c = 'x';
    int numero = 42;
    float pi = 3.14;
    bool ativo = true;
    char msg[] = "Olá, mundo!";
    
    if (numero > 10) {
        pi = pi + 1.0;
    } else {
        pi = pi - 1.0;
    }

    while (numero > 0) {
        numero = numero - 1;
    }

    return;
}

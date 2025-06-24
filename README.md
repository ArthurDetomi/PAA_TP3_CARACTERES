# Trabalho Prático 3 de Projeto e Análise de Algoritmos

## 📌 Problema

O desafio consiste em desenvolver e analisar um sistema para localizar padrões em arquivos de texto, abordando dois cenários distintos. O primeiro é a busca aproximada em arquivos não comprimidos, onde o objetivo é encontrar ocorrências de um padrão permitindo um determinado número k de erros. O segundo é a busca exata em arquivos comprimidos, onde o mesmo padrão deve ser encontrado diretamente em um arquivo que foi previamente comprimido.

A solução foi implementada para comparar o desempenho de diferentes algoritmos. Para a busca aproximada, foram utilizados a Programação Dinâmica e o Shift-And. Para a busca exata, o algoritmo Boyer-Moore-Horspool (BMH) foi aplicado tanto no arquivo original quanto no arquivo comprimido com o código de Huffman, permitindo uma análise comparativa da eficiência.

---

## 💻 Pré-requisitos

Antes de começar, certifique-se de que:

- Você tem um compilador C instalado (como o `gcc`) e o utilitário `make`.
- Está utilizando um sistema Linux compatível com os laboratórios do DCOMP.
- Leu este README até o final para entender como compilar e executar o projeto.

---

## 📁 Estrutura de Arquivos e Entradas

Para a correta execução do programa, é fundamental que a estrutura de diretórios e arquivos de entrada seja mantida.

Parte 1 e 2: Necessitam de um arquivo de texto e um arquivo de padrões, geralmente localizados em input/.

Parte 2 (Compressão): Para que a compressão Huffman funcione, é obrigatório que o arquivo alfabeto.txt esteja presente no diretório parte2/input/. Este arquivo define o conjunto de caracteres que será usado para gerar a compressão.

---

## ⚙️ Compilando o código

- Para compilar qualquer uma das partes, entre na pasta correspondente e execute:

```bash
cd parte1   # ou cd parte2
make
```

- Para limpar os arquivos binários gerados:

```bash
make clean
```

---

## 🚀 Executando o programa

### Parte 1 – Busca Aproximada

Executável: `./tp3_parte1`

```bash
./tp3_parte1 {estratégia} -i input/entrada.txt input/padroes.txt -o output/saida.txt
```

**Parâmetros:**

- `{estratégia}`:

  - `1` → Programação Dinâmica
  - `2` → Shift-And

- `-i` é **obrigatório** com dois arquivos: texto e padrões.
- `-o` é **opcional**. Se omitido, gera `output/output.txt`.

---

### Parte 2 – Busca Exata em Arquivo Comprimido

Executável: `./tp3_parte2`

```bash
./tp3_parte2 {opcao} -i input/entrada.txt input/padroes.txt -o output/saida.txt
```

**Parâmetros:**

- `{opcao}`:

  - `1` → Busca exata no arquivo **comprimido**
  - `2` → Busca exata no arquivo **original**

- `-i` é **obrigatório** com dois arquivos: texto e padrões.
- `-o` é **opcional**. Se omitido, gera `output/output.txt`.

---

### Exemplos de execução

```bash
./tp3_parte1 2 -i input/texto.txt input/padroes.txt
# Executa busca aproximada com Programação dinâmica

./tp3_parte1 2 -i input/texto.txt input/padroes.txt
# Executa busca aproximada com Shift-And

./tp3_parte2 1 -i input/texto.txt input/padroes.txt -o resultado.txt
# Executa busca exata com BMH no arquivo comprimido e salva em resultado.txt

./tp3_parte2 2 -i input/texto.txt input/padroes.txt -o resultado.txt
# Executa busca exata com BMH no arquivo original e salva em resultado.txt
```

## 📚 Documentação

🔗 [Repositório GitHub](https://github.com/ArthurDetomi/PAA_TP3_CARACTERES)

📄 Documentação:

- [`/docs/tp3.pdf`](/docs/tp3.pdf)

---

## 🤝 Dupla

<table>
    <tr>
        <td align="center">
            <a href="https://github.com/arthurdetomi" title="Perfil do GitHub de Geraldo Arthur">
            <img src="https://avatars.githubusercontent.com/u/99772832?v=4" width="100px;" alt="Foto de Geraldo Arthur Detomi no GitHub"/><br>
            <sub><b>Geraldo Arthur Detomi</b></sub>
            </a>
        </td>
        <td align="center">
            <a href="https://github.com/HomoGodzilla" title="Perfil do GitHub de Oscar Alves">
            <img src="https://avatars.githubusercontent.com/u/89045220?v=4" width="100px;" alt="Foto de Oscar Alves no GitHub"/><br>
            <sub><b>Oscar Alves</b></sub>
            </a>
        </td>
    </tr>
</table>

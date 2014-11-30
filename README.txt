Findpat
==============================

------------------------------
Autores
------------------------------

Alberto Trindade Tavares (att)
Bruna Carolina Baudel de Santana (bcbs)

------------------------------
Instruções de Compilação
------------------------------

Para gerar o executável, execute os comandos:
- cd src/
- make
O executável de findpat é gerado e disponibilizado no diretório /bin

Para apagar o diretório /bin com o executável, execute os comandos:
- cd src/
- make clean

Essas instruções consideram que o usuário está na pasta raiz do projeto. Se o usuário já estiver no diretório /src, não é necessário o primeiro comando.

------------------------------
Instruções de Uso
------------------------------

Uso: ./findpat [OPTIONS] PATTERN TEXTFILE [TEXTFILE...]

Esse comando procura por PATTERN em cada TEXTFILE
Se a opção --pattern for setada, uma lista de padrões será usada no lugar de PATTERN
Múltiplos arquivos podem ser indicados para TEXTFILE através do uso de wildcards

Opções (OPTIONS):
  -h, --help     Imprime instruções de uso (em Inglês)
  -e, --edit     Especifica uma distância de edição máxima para encontrar ocorrências aproximadas
                 de PATTERN ou dos padrões especificados pela opção --pattern, ao invés de ocorrências 
                 exatas, que é a opção padrão
  -p, --pattern  Especifica um arquivo com padrões para serem buscados, um por linha,
                 para serem usados no lugar de PATTERN
  -c, --count    Ao invés de imprimir as linhas de TEXTFILE em que os padrões ocorrem, a quantidade
                 total de ocorrência por arquivo é exibida

------------------------------
Exemplos de Uso
------------------------------

Busca exata:
- Imprime linhas de textfile1.txt onde "ababc" ocorre: ./findpat ababc textfile1.txt
- Imprime a quantidade de ocorrências em textfile1.txt de padrões listados em patternfile.txt: ./findpat -c -p patternfile.txt textfile1.txt

Busca aproximada:
- Imprime linhas de textfile.txt onde "ababc" ocorre com distância de edição máxima 1: ./findpat -e 1 -p ababc textfile.txt
- Imprime a quantidade de ocorrências com distância de edição máxima 2 de padrões listados em patternfile.txt, para cada arquivo de texto prefixado por "textfile": ./findpat -c -e 2 -p patternfile.txt textfile*.txt
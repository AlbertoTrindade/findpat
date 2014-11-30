------------------------------
FERRAMENTA
------------------------------

Findpat

------------------------------
AUTORES
------------------------------

Alberto Trindade Tavares (att)
Bruna Carolina Baudel de Santana (bcbs)


------------------------------
INSTRUÇÕES
------------------------------

Para gerar o executável:
- cd src/
- make

Para rodar o executável:
- cd bin/
- ./findpat [OPTIONS] PATTERN TEXTFILE [TEXTFILE...]

Para apagar o diretório bin com o executável:
- cd src/
- make clean





------------------------------
USO
------------------------------

USAGE: findpat [OPTIONS] PATTERN TEXTFILE [TEXTFILE...]
Search for PATTERN in each TEXTFILE
If --pattern option is set, a list of patterns will be used instead of PATTERN
Multiple files can be indicated for TEXTFILE by using wildcards

Options:
  -h, --help     Print usage and exit
  -e, --edit     Specify a maximun edit distance to find approximate occurrences
                 of PATTERN or patterns specified by --pattern option, instead
                 of the default exact ones
  -p, --pattern  Specify a file with patterns to be searched, one per line,
                 instead of using PATTERN
  -c, --count    Instead of printing the lines in which the patterns occur, the
                 total count of occurrences per file will be shown

------------------------------
EXEMPLOS
------------------------------

Busca exata:

- Buscar 1 padrão: ./findpat -c ababc textfile1.txt

- Buscar mais de 1 padrão: ./findpat -c -p patternfile.txt textfile1.txt

Busca aproximada:

- Buscar 1 padrão: ./findpat -c -e 2 -p ababc textfile.txt

- Buscar mais de 1 padrão: ./findpat -c -e 2 -p patternfile.txt textfile*.txt





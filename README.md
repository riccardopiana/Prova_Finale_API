# Prova Finale di Algoritmi e Principi dell'Informatica - A.A. 2022-2023

Prova finale per il corso di algoritmi e principi dell'informatica A.A. 2022-2023 Politecnico di Milano.

Docente: Martineghi Davide

**Valutazione**: 30/30

## Obiettivo del progetto

Implementare un'autostrada descritta come una sequenza di stazioni di servizio e calcolare il percorso ottimo tra due di queste.

Il programma deve produrre un output corretto rispettando i seguenti vincoli di memoria e tempo:

| Valutazione | Memoria | Tempo |       Esito        |
|-------------|--------:|------:|:------------------:|
| 18          | 128 MiB | 19 s  | :white_check_mark: |
| 21          | 118 MiB | 15 s  | :white_check_mark: |
| 24          | 108 MiB | 10 s  | :white_check_mark: |
| 27          |  98 MiB |  6 s  | :white_check_mark: |
| 30          |  88 MiB |  4 s  | :white_check_mark: |
| 30L         |  78 MiB |  1 s  |        :x:         |

## Scelte progettuali

L'autostrada è stata implementata come un [albero rosso-nero](https://it.wikipedia.org/wiki/RB-Albero).

## Come eseguire un test

Per eseguire i test bisogna:
1. Compilare il programma con `make your_c_file`
2. Eseguire il programma con `./your_c_file > output.txt < Tests/test_you_want_to_run.txt`
3. Comparare `output.txt` con `test_you_want_to_run.output.txt`

## Strumenti utilizzati

| Descrizione         | Strumento                                  |
|---------------------|--------------------------------------------|
| IDE                 | [VSCode](https://code.visualstudio.com/)  |
| Compilatore        | [gcc](https://gcc.gnu.org/)                |
| Misurazione memoria | [Valgrind - Massif](https://valgrind.org/) |
| Sistema operativo   | Windows 11                     |

## Copyright e licenza

Il progetto è distribuito sotto [MIT license](LICENSE).

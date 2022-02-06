# Implementação de algoritmos de teoria dos grafos

Algoritmos a serem explorados serão:

| Algoritmo | Pasta |
| --------- | ----- |
| Dijkstra  | dijkstra |
| Prim      | prim |
| Ford-Fulkerson | ford-fulkerson |
| Bellman-Ford | bellman-ford |

Building system utilizado: \
Makefile

# Compilando e executando

Para compilar os programas, basta entrar na pasta desejada e digitar `make`, que ele criará um executável com o nome do algoritmo.

Nos sistemas linux, o executável terá a extensão `.out`, e nos sistemas Windows, terá a extensão `.exe`.

Para compilar e executar os binários, basta digitar `make run` (lembrando que para tal, deverás modificar a variável do Makefile chamada `RUN_ARGS`, que são os argumentos da execução do binário). Esse comando foi utilizado para teste dos algoritmos, mas pode ser útil para ter uma noção da execução dos mesmos.

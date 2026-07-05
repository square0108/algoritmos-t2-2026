# algoritmos-t2-2026
Tarea 2 del curso Análisis de Algoritmos, Universidad de Concepción

## Compilación

```
g++ src/test.cpp -I include/ -o graph.out
```

## Ejecución

```
./graph.out <path to .mtx file> <--BF | --FW> <-0 | -1> [--print-results | --print-csv]
```

donde,
- `--BF` para Bellman-Ford, `--FW` para Floyd-Warshall.
- `-0` si los vértices están indexados a 0, `-1` para indexados a 1.
- (Opcional) `--print-results` para imprimir la matriz de adyacencia, lista de adyacencia, matriz de distancias y matriz de predecesores., `--print-csv` para imprimir en formato .csv, donde cada entrada es (nodo1),(nodo2),(dist. camino más corto desde nodo1 hasta nodo2).
# Route Planning Algorithms

Route planning algorithms are fundamental for
many applications, such as the GPS or communication networks.
This study benchmarked Dijkstra, A* and their bidirectional
variants, each tested against three priority queue architectures:
the Binary Min-Heap, the Fibonacci Heap and the Pairing
Heap. Experiments were conducted on real-world road networks
and synthetically generated graphs. Although the Fibonacci
Heap, Pairing Heap and bidirectional searches offer a superior
asymptotic complexity in theory, their practical performance
is inferior due to constant-factor overheads. A* paired with a
Binary Min-Heap proves to be the most effective combination
for sparse road networks.

## Getting Started

### Prerequisites

- GCC
- DIMACS road networks of California and New York (both coordinates and connections)
- Python3 with `pandas`, `datashader` and `Pillow` installed

```bash
pip install pandas datashader pillow
```

The DIMACS graphs can be downloaded [here](https://www.diag.uniroma1.it/~challenge9/download.shtml)

### Running the performance tests

```bash
./runCode.sh
```

This compiles with the `-O3` flag for better optimizations and runs `performance_test`, printing a results table to the terminal.

### Visualizing the calculated path

In `Source Files/routealgorithms.c`, `Draw` must be `1` in order for the algorithms to save their calculations to the `nodes.csv` file.

Run only one algorithm one time, otherwise the calculated data in `nodes.csv` will be overwritten and only the last algorithm's calculations will be saved.

Once data is saved in `nodes.csv`, use `plotPath.sh` to generate the visualization image (default image path: `california_graph_render.png`, can be changed in `pathPlotter.py`).

```bash
./plotPath.sh
```

In this image, each node will be colored:
- Grey: Unvisited nodes
- Green: visited nodes
- Red: Nodes on the final path

## Configuration

All configurations are done in `main.c` and `Source Files/routealgorithms.c`.

### `main.c`

#### Switch road network:

Choose which graph to use by activating the one needed. The vertices amount must also be matched

```c
int vertices = 1890815;  // California
// int vertices = 264346; // New York
 
long** coords = load_graph_dimacs(graph, "./Resources/USA-road-d.CAL.gr", "./Resources/USA-road-d.CAL.co");
// long** coords = load_graph_dimacs(graph, "./Resources/USA-road-d.NY.gr", "./Resources/USA-road-d.NY.co");
```

#### Change iterations and benchmark count:

edit the call in `main`:
 
```c
performance_test(/* iterations */ 75, /* benchmarks */ 40);
```

Benchmarks determines how many different source-target pairs will be used. Iterations will test each source-target pair that many times. Benchmarks must always be less or equal to the amount of source-target pairs.

#### Toggle algorithms:

comment/uncomment the test calls at the bottom of `performance_test`

```c
test_dijkstra(...);
test_bidirectional_dijkstra(...);
test_A_star(...);
test_bidirectional_A_star(...);
test_dijkstra_min(...);
test_dijkstra_fib(...);
test_dijkstra_pairing(...);
```

### `Source Files/routealgorithms.c`

At the top of this file, two flags can be found:

#### Debug flag:

Set to `1` to log the steps done by the algorithm in a `[algorithm-name].txt` file ([algorithm-name] is changed with the actual name of the algorithm).

#### Draw flag:

Set to `1` to save the calculations done by the algorithm in `nodes.csv` to later be used for generating visualisations.

## Output Format

`perfomance_test` prints a table of results for each algorithm/heap structure like:

```
Sources | Targets
  ...   |   ...

--------------------------------------------------------------------------------------
|                            Dijkstra California                                      |
--------------------------------------------------------------------------------------

Di(s)       Vi:Di
 ...    |    ...
```

In which `Di(s)` is the mean runtime of the algorithm for a given source-target pair (`Di` is for dijkstra, this changes for other algorithms). `Vi:Di` is the mean number of nodes visited.
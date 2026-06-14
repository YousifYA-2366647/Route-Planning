#ifndef ROUTEALGORITHMS_H
#define ROUTEALGORITHMS_H

#include "minheap.h"
#include "fibheap.h"
#include "pairingheap.h"
#include "graph.h"
#include "queue.h"
#include "stack.h"

#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct RouteResult {
    int* path;
    int maxNodes;
    int nodesVisited;
    double distance;
} RouteResult;

RouteResult* DFS(Graph* graph, int source, int target);

RouteResult* BFS(Graph* graph, int source, int target);

RouteResult** Dijsktra_all_pairs(Graph* graph, int source);

RouteResult* Dijkstra_min(Graph* graph, long** coords, int source, int target);

RouteResult* Dijkstra_fib(Graph* graph, long** coords, int source, int target);

RouteResult* Dijkstra_pairing(Graph* graph, long** coords, int source, int target);

RouteResult* Bidirectional_Dijkstra_min(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

RouteResult* Bidirectional_Dijkstra_fib(Graph* graph, Graph* reversedGraph, int source, int target);

RouteResult* Bidirectional_Dijkstra_pairing(Graph* graph, Graph* reversedGraph, int source, int target);

RouteResult* A_Star_min(Graph* graph, long** coords, int source, int target);

RouteResult* A_Star_fib(Graph* graph, long** coords, int source, int target);

RouteResult* A_Star_pairing(Graph* graph, long** coords, int source, int target);

RouteResult* Bidirectional_A_Star_min(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

RouteResult* Bidirectional_A_Star_fib(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

RouteResult* Bidirectional_A_Star_pairing(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

#endif
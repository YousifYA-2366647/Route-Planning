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

// Used to keep track of the performance of routeplanning algorithms
// Keeps track of:
// - the path found from source to target
// - the nodes that were visited during the search
// - the distance from source to target
typedef struct RouteResult {
    int* path;
    int maxNodes;
    int nodesVisited;
    double distance;
} RouteResult;

// A standard Depth-First Search using a Stack
RouteResult* DFS(Graph* graph, int source, int target);

// A standard Breadth-First Search using a Queue
RouteResult* BFS(Graph* graph, int source, int target);

// Dijkstra run from a source to all other nodes
RouteResult** Dijsktra_all_pairs(Graph* graph, int source);

// Dijkstra implementation using a minheap
RouteResult* Dijkstra_min(Graph* graph, long** coords, int source, int target);

// Dijkstra implementation using a fibonacci heap
RouteResult* Dijkstra_fib(Graph* graph, long** coords, int source, int target);

// Dijkstra implementation using a pairing heap
RouteResult* Dijkstra_pairing(Graph* graph, long** coords, int source, int target);

// Bidirectional Dijkstra using a minheap
RouteResult* Bidirectional_Dijkstra_min(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

// Bidirectional Dijkstra using a fibonacci heap
RouteResult* Bidirectional_Dijkstra_fib(Graph* graph, Graph* reversedGraph, int source, int target);

// Bidirectional Dijkstra using a pairing heap
RouteResult* Bidirectional_Dijkstra_pairing(Graph* graph, Graph* reversedGraph, int source, int target);

// A* using a minheap
RouteResult* A_Star_min(Graph* graph, long** coords, int source, int target);

// A* using a Fibonacci Heap
RouteResult* A_Star_fib(Graph* graph, long** coords, int source, int target);

// A* using a Pairing Heap
RouteResult* A_Star_pairing(Graph* graph, long** coords, int source, int target);

// Bidirectional A* using a Min-Heap
RouteResult* Bidirectional_A_Star_min(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

// Bidirectional A* using a Fibonacci Heap
RouteResult* Bidirectional_A_Star_fib(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

// Bidirectional A* using a Pairing Heap
RouteResult* Bidirectional_A_Star_pairing(Graph* graph, Graph* reversedGraph, long** coords, int source, int target);

#endif
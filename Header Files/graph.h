#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

typedef struct AdjacencyListNode {
    struct AdjacencyListNode* next;
    int node;
    double weight;
} AdjacencyListNode;

typedef struct Graph {
    int vertexAmount;
    bool directed;
    double localityRadius;
    AdjacencyListNode** adjacencyList;
} Graph;

Graph* create_graph(int amountOfVertices, double localityRadius, bool directed);

AdjacencyListNode* create_node(int node);

void add_edge(Graph* graph, int source, int target, double weight);

bool edge_exists(Graph* graph, int source, int target);

void shuffle_vertices(int* vertices, int length);

void connect_lattice(Graph* graph, int* vertices, int offset, int* edgeCounter, int maxEdges);

void load_random_connected_graph(Graph* graph, int edgeAmount, int degree);

long** load_random_graph_coords(Graph* graph, int edgeAmount, int degree);

void load_erdos_renyi_model_graph(Graph* graph, int edgeAmount, double prob);

void load_realistic_random_graph(Graph* graph, int edgeAmount);

void load_generated_graph(Graph* graph);

void load_grid_graph(Graph* graph, int rows, double prob);

void save_graph_to_csv(Graph* graph, char fileName[]);

void load_graph_from_csv(Graph* graph, char fileName[]);

void load_graph_stanford(Graph* graph, char fileName[]);

long** load_graph_dimacs(Graph* graph, char dataFileName[], char coordFileName[]);

float* get_degrees(Graph* graph);

Graph* reverse_graph(Graph* graph);

void print_graph(Graph* graph);

void print_graph_descriptive(Graph* graph);

void free_graph(Graph* graph);

#endif
#ifndef GRAPH_C
#define GRAPH_C

#include "../Header Files/graph.h"
#include <math.h>

Graph* create_graph(int amountOfVertices, double localityRadius, bool directed) {
    Graph* graph = malloc(sizeof(Graph));
    graph->vertexAmount = amountOfVertices;
    graph->directed = directed;
    graph->localityRadius = localityRadius;
    graph->adjacencyList = calloc(amountOfVertices, sizeof(AdjacencyListNode*));

    return graph;
}

AdjacencyListNode* create_node(int node) {
    AdjacencyListNode* listNode = malloc(sizeof(AdjacencyListNode));
    listNode->node = node;
    listNode->weight = 0.0f;
    listNode->next = NULL;

    return listNode;
}

void add_edge(Graph* graph, int source, int target, double weight) {
    AdjacencyListNode* targetEdge = create_node(target);
    
    targetEdge->weight = weight;
    targetEdge->next = graph->adjacencyList[source];
    graph->adjacencyList[source] = targetEdge;

    if (!graph->directed) {
        AdjacencyListNode* sourceEdge = create_node(source);

        sourceEdge->weight = weight;
        sourceEdge->next = graph->adjacencyList[target];
        graph->adjacencyList[target] = sourceEdge;
    }
}

bool edge_exists(Graph* graph, int source, int target) {
    AdjacencyListNode* curNode = graph->adjacencyList[source];

    while (curNode) {
        if (curNode->node == target) {
            return true;
        }
        curNode = curNode->next;
    }

    return false;
}

void shuffle_vertices(int* vertices, int length) {
    for (int i = 0; i < length; i++) {
        int index = rand() % length;
        int temp = vertices[i];
        vertices[i] = vertices[index];
        vertices[index] = temp;
    }
}

void connect_lattice(Graph* graph, int* vertices, int offset, int* edgeCounter, int maxEdges) {
    double maxDistance = graph->localityRadius;
    for (int i = 0; i < graph->vertexAmount; i++) {
        if (*edgeCounter >= maxEdges) {
            return;
        }
        int vert1 = vertices[i];
        int vert2 = vertices[(i+offset)%graph->vertexAmount];

        int dif = abs(vert1 - vert2);

        if (rand()/(float)RAND_MAX < 0.9f && dif > maxDistance) continue;

        if (!edge_exists(graph, vertices[i], vertices[(i+offset)%graph->vertexAmount])) {
            double weight = sqrt(pow(vert1-vert2, 2));
            add_edge(graph, vert1, vert2, weight);
            (*edgeCounter)++;
        }
    }
}

void load_random_connected_graph(Graph* graph, int edgeAmount, int degree) {
    int* vertices = malloc(graph->vertexAmount * sizeof(int));

    for (int i = 0; i < graph->vertexAmount; i++) {
        vertices[i] = i;
    }
    

    int edgeCounter = 0;
    while (edgeCounter < edgeAmount) {
        shuffle_vertices(vertices, graph->vertexAmount);
        for (int i = 1; i <= degree; i++) {
            connect_lattice(graph, vertices, i, &edgeCounter, edgeAmount);
        }
    }

    free(vertices);
}

long** load_random_graph_coords(Graph* graph, int edgeAmount, int degree) {
    long** coords = malloc(2 * sizeof(long*));
    coords[0] = malloc(graph->vertexAmount * sizeof(long));  /* latitudes  */
    coords[1] = malloc(graph->vertexAmount * sizeof(long));  /* longitudes */

    long lat_min =  48000000L, lat_range = 5500000L;
    long lon_min =   2500000L, lon_range = 7000000L;

    for (int i = 0; i < graph->vertexAmount; i++) {
        coords[0][i] = lat_min + (long)rand() % lat_range;
        coords[1][i] = lon_min + (long)rand() % lon_range;
    }

    /* Load the graph exactly like load_random_connected_graph does. */
    int* vertices = malloc(graph->vertexAmount * sizeof(int));
    for (int i = 0; i < graph->vertexAmount; i++)
        vertices[i] = i;

    int edgeCounter = 0;
    while (edgeCounter < edgeAmount) {
        shuffle_vertices(vertices, graph->vertexAmount);
        for (int i = 1; i <= degree; i++)
            connect_lattice(graph, vertices, i, &edgeCounter, edgeAmount);
    }

    free(vertices);
    return coords;
}

void load_realistic_random_graph(Graph* graph, int edgeAmount) {
    int edgeCounter = 0;
    int localityRadius = graph->localityRadius;
    while (edgeCounter < edgeAmount) {
        int v1 = rand() % graph->vertexAmount;

        int range = (rand() % localityRadius) + 1; 
        int v2 = (v1 + range) % graph->vertexAmount;

        double diff = (float)abs(v1 - v2);
        if (diff == 0) continue;

        double prob = 1.0f / diff;
        if ((rand() / (float)RAND_MAX) < prob) {
            if (!edge_exists(graph, v1, v2)) {
                add_edge(graph, v1, v2, ((float)rand()/(float)RAND_MAX) / prob);
                edgeCounter++;
            }
        }
    }
}

void load_erdos_renyi_model_graph(Graph* graph, int edgeAmount, double prob) {
    int edgeCounter = 0;
    for (int i = 0; i < graph->vertexAmount; i++) {
        for (int j = 0; j < graph->vertexAmount; j++) {
            if (i == j) continue;
            if (rand() / (float)RAND_MAX < prob) {
                add_edge(graph, i, j, (float)(abs(j - i)) * rand() / (float)RAND_MAX);
                edgeCounter++;
            }
            if (edgeCounter >= edgeAmount) return;
        }
    }
}

void load_generated_graph(Graph* graph) {
    
}

float* get_degrees(Graph* graph) {
    float* degrees = malloc(graph->vertexAmount * sizeof(float));
    for (int i = 0; i < graph->vertexAmount; i++) {
        double degree_counter = 0;
        AdjacencyListNode* curNode = graph->adjacencyList[i];
        while (curNode) {
            degree_counter++;
            curNode = curNode->next;
        }
        degrees[i] = degree_counter;
    }
    return degrees;
}

void load_grid_graph(Graph* graph, int rows, double prob) {
    int cols = (int)ceil(graph->vertexAmount / (float)rows);
    srand(time(NULL));

    for (int k = 0; k < graph->vertexAmount; k++) {
        int curRow = k / cols;
        int curCol = k % cols;

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dy == 0 && dx == 0) continue;

                int neighbourRow = curRow + dy;
                int neighbourCol = curCol + dx;

                if (neighbourRow < 0 || neighbourRow >= rows) continue;
                if (neighbourCol < 0 || neighbourCol >= cols) continue;

                int neighbour = neighbourRow * cols + neighbourCol;
                if (neighbour < 0 || neighbour >= graph->vertexAmount) continue;

                if (rand() / (float)RAND_MAX < prob && !edge_exists(graph, k, neighbour)) {
                    double weight = sqrtf((float)(dx*dx + dy*dy));
                    add_edge(graph, k, neighbour, weight);
                }
            }
        }
    }
}


void save_graph_to_csv(Graph* graph, char fileName[]) {
    FILE* csv_file = fopen(fileName, "w");
    for (int i = 0; i < graph->vertexAmount; i++) {
        AdjacencyListNode* curNode = graph->adjacencyList[i];
        while (curNode) {
            fprintf(csv_file, "%d,%d\n", 
                i, 
                curNode->node);
            curNode = curNode->next;
        }
    }
    fclose(csv_file);
}

void load_graph_from_csv(Graph* graph, char fileName[]) {
    FILE* csv_file = fopen(fileName, "r");
    char line[256];
    bool directed = graph->directed;
    graph->directed = true;
    while (fgets(line, sizeof(line), csv_file)) {
        char* from = strtok(line, ",");
        char* weight = strtok(NULL, ",");
        char* to = strtok(NULL, ",");

        int from_int = atoi(from);
        double weight_float = atof(weight);
        int to_int = atoi(to);

        add_edge(graph, from_int, to_int, weight_float);
    }
    graph->directed = directed;
    fclose(csv_file);
}

void load_graph_stanford(Graph* graph, char fileName[]) {
    FILE* file = fopen(fileName, "r");
    char line[256];
    graph->directed = false;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue;
        char* from = strtok(line, "\t");
        char* to = strtok(NULL, "\t");

        int from_int = atoi(from);
        int to_int = atoi(to);
        //double weight_float = sqrt(pow(from_int - to_int, 2) + pow(from_int - to_int, 2));
        //double weight_float = (float)abs(from_int - to_int);
        //double weight_float = 1.0;
        double weight_float = 1.0 / (abs(from_int - to_int) + 1);

        add_edge(graph, from_int, to_int, weight_float);
    }
    fclose(file);
}

long** load_graph_dimacs(Graph* graph, char dataFileName[], char coordFileName[]) {
    FILE* coordFile = fopen(coordFileName, "r");
    if (!coordFile) { printf("Error opening file: %s\n", coordFileName); return NULL; }

    long** coords = malloc(2 * sizeof(long*));

    long* xs = malloc(graph->vertexAmount * sizeof(long));
    long* ys = malloc(graph->vertexAmount * sizeof(long));

    coords[0] = xs;
    coords[1] = ys;

    char coordLine[256];
    while (fgets(coordLine, sizeof(coordLine), coordFile)) {
        if (coordLine[0] == 'v') {
            int vertex;
            long xCoord, yCoord;
            sscanf(coordLine, "v %d %ld %ld", &vertex, &xCoord, &yCoord);
            xs[vertex - 1] = xCoord;
            ys[vertex - 1] = yCoord;
        }
    }
    fclose(coordFile);

    FILE* dataFile = fopen(dataFileName, "r");
    if (!dataFile) { printf("Error opening file: %s\n", dataFileName); return NULL; }
    
    char line[256];
    while (fgets(line, sizeof(line), dataFile)) {
        if (line[0] == 'a') {
            int from, to;
            double weight;
            sscanf(line, "a %d %d %lf", &from, &to, &weight);
            add_edge(graph, from - 1, to - 1, weight);
        }
    }
    fclose(dataFile);

    return coords;
}

Graph* reverse_graph(Graph* graph) {
    Graph* reversed = create_graph(graph->vertexAmount, graph->localityRadius, graph->directed);

    for (int i = 0; i < graph->vertexAmount; i++) {
        AdjacencyListNode* curNode = graph->adjacencyList[i];

        while (curNode) {
            int v = curNode->node;
            double weight = curNode->weight;

            add_edge(reversed, v, i, weight);

            curNode = curNode->next;
        }
    }

    return reversed;
}

void print_graph(Graph* graph) {
    printf("%d\n", graph->vertexAmount);
    for (int i = 0; i < graph->vertexAmount; i++) {
        AdjacencyListNode* curNode = graph->adjacencyList[i];
        while (curNode) {
            printf("%d %d\n", 
                i, 
                curNode->node);
            curNode = curNode->next;
        }
    }
}

void print_graph_descriptive(Graph* graph) {
    printf("%d\n", graph->vertexAmount);
    for (int i = 0; i < graph->vertexAmount; i++) {
        AdjacencyListNode* curNode = graph->adjacencyList[i];
        while (curNode) {
            printf("%d --%.2f--> %d\n", 
                i, 
                curNode->weight,
                curNode->node);
            curNode = curNode->next;
        }
    }
}

void free_graph(Graph* graph) {
    if (!graph || !graph->adjacencyList) return;

    for (int i = 0; i < graph->vertexAmount; i++) {
        AdjacencyListNode* curNode = graph->adjacencyList[i];
        while (curNode) {
            AdjacencyListNode* temp = curNode->next;
            free(curNode);
            curNode = temp;
        }
    }
    free(graph->adjacencyList);
    free(graph);
}

#endif
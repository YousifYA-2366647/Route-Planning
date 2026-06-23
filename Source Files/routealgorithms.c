#ifndef ROUTEALGORITHMS_C
#define ROUTEALGORITHMS_C

#include "../Header Files/routealgorithms.h"

#define INTEGER_TO_DEGREE 1000000.0
#define METER_PER_DEGREE 111000.0
#define MILE_PER_METER 0.000621371
#define EARTH_RADIUS 3959.8
#define M_PI 3.14159265358979323846
#define SCALE_FACTOR 10000.0
#define DECIMETER_PER_METER 10.0

#define DEBUG 0
#define DRAW 0

static FILE* g_log_file = NULL;

void debug_init(const char* filename) {
    g_log_file = fopen(filename, "w");
}

void debug_close() {
    if (g_log_file) fclose(g_log_file);
}

#define DEBUG_PRINT(...) do { \
    if (DEBUG && g_log_file) fprintf(g_log_file, __VA_ARGS__); \
} while (0)

void save_nodes_to_csv(const char* filename, int vertexAmount, long* xCoords, long* yCoords, bool* visitedStatus, bool* onPathStatus) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    fprintf(fp, "label,xCoord,yCoord,visited,onPath\n");

    for (int i = 0; i < vertexAmount; i++) {
        fprintf(
            fp,
            "%d,%ld,%ld,%d,%d\n",
            i,
            xCoords[i],
            yCoords[i],
            visitedStatus[i] ? 1 : 0,
            onPathStatus[i] ? 1 : 0
        );
    }

    fclose(fp);
    printf("Graph state saved to %s\n", filename);
}

RouteResult* BFS(Graph* graph, int source, int target) {
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    Queue* queue = create_queue(graph->vertexAmount);
    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            dist[i] = 0.0f;
        }
        else {
            dist[i] = INFINITY;
        }
        result->path[i] = -1;
        visited[i] = false;
    }

    insert_queue(queue, 0.0f, source);

    while (!queue_empty(queue)) {
        QueueElement* curElement = pop_queue(queue);

        visited[curElement->value] = true;
        result->nodesVisited++;

        if (curElement->value == target) {
            result->distance = dist[curElement->value];
            free(dist);
            free(visited);
            free_queue(queue);
            return result;
        }

        AdjacencyListNode* neighbour = graph->adjacencyList[curElement->value];
        while (neighbour) {
            if (element_in_queue(queue, neighbour->node) || visited[neighbour->node]) {
                neighbour = neighbour->next;
                continue;
            };
            dist[neighbour->node] = neighbour->weight + curElement->key;
            insert_queue(queue, dist[neighbour->node], neighbour->node);
            result->path[neighbour->node] = curElement->value;

            neighbour = neighbour->next;
        }

        free(curElement);
    }

    free(result->path);
    free(result);
    free(dist);
    free(visited);
    free_queue(queue);
    return NULL;
}

RouteResult* DFS(Graph* graph, int source, int target) {
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    CustomStack* stack = create_stack(graph->vertexAmount);
    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            dist[i] = 0.0f;
        }
        else {
            dist[i] = INFINITY;
        }
        result->path[i] = -1;
        visited[i] = false;
    }

    insert_stack(stack, 0.0f, source);

    while (!stack_empty(stack)) {
        StackElement* curElement = pop_top_stack(stack);

        visited[curElement->value] = true;
        result->nodesVisited++;

        if (curElement->value == target) {
            result->distance = dist[curElement->value];
            free(dist);
            free(visited);
            free_stack(stack);
            return result;
        }

        AdjacencyListNode* neighbour = graph->adjacencyList[curElement->value];
        while (neighbour) {
            if (element_in_stack(stack, neighbour->node) || visited[neighbour->node]) {
                neighbour = neighbour->next;
                continue;
            };
            dist[neighbour->node] = neighbour->weight + curElement->key;
            insert_stack(stack, dist[neighbour->node], neighbour->node);
            result->path[neighbour->node] = curElement->value;

            neighbour = neighbour->next;
        }

        free(curElement);
    }

    free(result->path);
    free(result);
    free(dist);
    free(visited);
    free_stack(stack);
    return NULL;
}

RouteResult* Dijkstra_min(Graph* graph, long** coords, int source, int target) {
    if (DEBUG) debug_init("dijkstra_log.txt");

    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    long* xCoords = coords[0];
    long* yCoords = coords[1];

    Minheap* heap = create_minheap(graph->vertexAmount);

    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));

    for (int i = 0; i < graph->vertexAmount; i++) {
        dist[i] = (i == source) ? 0.0 : INFINITY;
        result->path[i] = -1;
        visited[i] = false;
    }

    insert_element_minheap(heap, 0.0, source);
    bool targetReached = false;

    while (!minheap_empty(heap)) {
        HeapNode* curNode = extract_min_minheap(heap);

        if (curNode->value == target) {
            result->distance = dist[curNode->value];
            visited[curNode->value] = true;
            targetReached = true;
            free(curNode);
            break;
        }

        visited[curNode->value] = true;
        result->nodesVisited++;
        
        for(
            AdjacencyListNode* neighbour = graph->adjacencyList[curNode->value]; 
            neighbour != NULL; 
            neighbour = neighbour->next
        ) {
            double weight = neighbour->weight;

            if (!visited[neighbour->node] && weight >= 0) {
                double newDist = dist[curNode->value] + weight;

                if (newDist < dist[neighbour->node]) {
                    dist[neighbour->node] = newDist;
                    result->path[neighbour->node] = curNode->value;

                    if (element_exists_minheap(heap, neighbour->node)) {
                        decrease_key_minheap(heap, neighbour->node, dist[neighbour->node]);
                    } else {
                        insert_element_minheap(heap, dist[neighbour->node], neighbour->node);
                    }
                }
            }
        }

        free(curNode);
    }

    if (DRAW) {
        bool* onPath = malloc(graph->vertexAmount * sizeof(bool));

        if (targetReached) {
            int current = target;
            while (current != -1) {
                onPath[current] = true;
                current = result->path[current];
            }
        }

        save_nodes_to_csv(
            "nodes.csv",
            graph->vertexAmount,
            xCoords,
            yCoords,
            visited,
            onPath
        );

        free(onPath);
    }

    free(dist);
    free(visited);
    free_minheap(heap);
    debug_close();

    if (targetReached) {
        return result;
    } else {
        free(result->path);
        free(result);
        return NULL;
    }
}

RouteResult* Dijkstra_fib(Graph* graph, long** coords, int source, int target) {
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    Fibheap* heap = create_fibheap();
    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));
    FibNode** heapNodes = malloc(graph->vertexAmount * sizeof(FibNode*));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            dist[i] = 0.0f;
        }
        else {
            dist[i] = INFINITY;
        }
        result->path[i] = -1;
        heapNodes[i] = NULL;
        visited[i] = false;
    }

    heapNodes[source] = fib_insert_element(heap, 0.0f, source);
    bool targetReached = false;

    while (!fib_empty_heap(heap)) {
        FibNode* curNode = fib_extract_min(heap);
        heapNodes[curNode->value] = NULL;

        if (curNode->value == target) {
            result->distance = dist[curNode->value];
            targetReached = true;
            free(curNode);
            break;
        }

        visited[curNode->value] = true;
        result->nodesVisited++;

        for(
            AdjacencyListNode* neighbour = graph->adjacencyList[curNode->value]; 
            neighbour != NULL; 
            neighbour = neighbour->next
        ) {
            if (visited[neighbour->node] || neighbour->weight < 0) continue;
            
            double total_dist = dist[curNode->value] + neighbour->weight;
            if (dist[neighbour->node] > total_dist) {
                dist[neighbour->node] = total_dist;
                result->path[neighbour->node] = curNode->value;

                if (heapNodes[neighbour->node] != NULL) {
                    fib_decrease_key(heap, heapNodes[neighbour->node], dist[neighbour->node]);
                }
                else {
                    heapNodes[neighbour->node] = fib_insert_element(heap, dist[neighbour->node], neighbour->node);
                }
            }
        }
        free(curNode);
    }

    if (DRAW) {
        bool* onPath = malloc(graph->vertexAmount * sizeof(bool));

        if (targetReached) {
            int current = target;
            while (current != -1) {
                onPath[current] = true;
                current = result->path[current];
            }
        }

        save_nodes_to_csv(
            "nodes.csv",
            graph->vertexAmount,
            coords[0],
            coords[1],
            visited,
            onPath
        );

        free(onPath);
    }

    free(dist);
    free(visited);
    free(heapNodes);
    fib_free_heap(heap);

    if (targetReached) {
        return result;
    }
    else {
        free(result->path);
        free(result);
        return NULL;
    }
}

RouteResult* Dijkstra_pairing(Graph* graph, long** coords, int source, int target) {
    if (DEBUG) debug_init("dijkstra.txt");
    
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    PairingHeap* heap = create_pairingheap();
    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));
    PairingHeapNode** heapNodes = malloc(graph->vertexAmount * sizeof(PairingHeapNode*));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            dist[i] = 0.0f;
        }
        else {
            dist[i] = INFINITY;
        }
        result->path[i] = -1;
        heapNodes[i] = NULL;
        visited[i] = false;
    }

    heapNodes[source] = insert_pairingheap_element(heap, 0.0f, source);
    bool targetReached = false;

    while (!empty_pairingheap(heap)) {
        PairingHeapNode* curNode = delete_min_pairing(heap);
        heapNodes[curNode->value] = NULL;

        if (curNode->value == target) {
            result->distance = dist[curNode->value];
            targetReached = true;
            free(curNode);
            break;
        }

        visited[curNode->value] = true;
        result->nodesVisited++;

        for(
            AdjacencyListNode* neighbour = graph->adjacencyList[curNode->value]; 
            neighbour != NULL; 
            neighbour = neighbour->next
        ) {
            if (visited[neighbour->node] || neighbour->weight < 0) continue;
            
            double total_dist = dist[curNode->value] + neighbour->weight;
            if (dist[neighbour->node] > total_dist) {
                dist[neighbour->node] = total_dist;
                result->path[neighbour->node] = curNode->value;

                if (heapNodes[neighbour->node] != NULL) {
                    decrease_key_pairingheap(heap, heapNodes[neighbour->node], dist[neighbour->node]);
                }
                else {
                    heapNodes[neighbour->node] = insert_pairingheap_element(heap, dist[neighbour->node], neighbour->node);
                }
            }
        }
        free(curNode);
    }

    if (DRAW) {
        bool* onPath = malloc(graph->vertexAmount * sizeof(bool));

        if (targetReached) {
            int current = target;
            while (current != -1) {
                onPath[current] = true;
                current = result->path[current];
            }
        }

        save_nodes_to_csv(
            "nodes.csv",
            graph->vertexAmount,
            coords[0],
            coords[1],
            visited,
            onPath
        );

        free(onPath);
    }

    free(dist);
    free(visited);
    free_pairingheap(heap);
    free(heapNodes);
    debug_close();

    if (targetReached) {
        return result;
    } else {
        free(result->path);
        free(result);
        return NULL;
    }
}

RouteResult** Dijsktra_all_pairs(Graph* graph, int source) {
    if (source < 0 || source >= graph->vertexAmount) {
        return NULL;
    }

    Minheap* heap = create_minheap(graph->vertexAmount);
    RouteResult** results = malloc(graph->vertexAmount * sizeof(RouteResult*));

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            dist[i] = 0.0f;
        }
        else {
            dist[i] = INFINITY;
        }

        results[i] = malloc(sizeof(RouteResult));
        results[i]->distance = INFINITY;

        visited[i] = false;
    }

    insert_element_minheap(heap, 0.0f, source);

    while (!minheap_empty(heap)) {
        HeapNode* curNode = extract_min_minheap(heap);

        visited[curNode->value] = true;
        results[curNode->value]->distance = dist[curNode->value];

        AdjacencyListNode* neighbour = graph->adjacencyList[curNode->value];
        while (neighbour) {
            double weight = neighbour->weight;
            if (!visited[neighbour->node] &&
                weight >= 0 &&
                dist[neighbour->node] > dist[curNode->value] + weight
            ) {
                dist[neighbour->node] = dist[curNode->value] + weight;

                if (element_exists_minheap(heap, neighbour->node)) {
                    double oldDist = get_key_minheap(heap, neighbour->node);
                    if (dist[neighbour->node] < oldDist) {
                        decrease_key_minheap(heap, neighbour->node, dist[neighbour->node]);
                    }
                }
                else {
                    insert_element_minheap(heap, dist[neighbour->node], neighbour->node);
                }
            }

            neighbour = neighbour->next;
        }
        free(curNode);
    }

    free(dist);
    free(visited);
    free_minheap(heap);
    return results;
}

RouteResult* Bidirectional_Dijkstra_min(Graph* graph, Graph* reversedGraph, long** coords, int source, int target) {
    if (DEBUG) debug_init("bidijkstra_log.txt");
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    // Make a heap for both the forward and backward search
    Minheap* forwardHeap = create_minheap(graph->vertexAmount);
    Minheap* backwardHeap = create_minheap(graph->vertexAmount);

    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    typedef struct NodeData {
        double dist;
        double visited;
        int parent;
    } NodeData;

    // Create arrays containing information about the nodes, one array for each search direction
    NodeData* forwardNodes = malloc(graph->vertexAmount * sizeof(NodeData));
    NodeData* backwardNodes = malloc(graph->vertexAmount * sizeof(NodeData));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            forwardNodes[i].dist = 0.0f;
            backwardNodes[i].dist = INFINITY;
        }
        else if (i == target) {
            forwardNodes[i].dist = INFINITY;
            backwardNodes[i].dist = 0.0f;
        }
        else {
            forwardNodes[i].dist = INFINITY;
            backwardNodes[i].dist = INFINITY;
        }
        forwardNodes[i].parent = -1;
        backwardNodes[i].parent = -1;
        result->path[i] = -1;
        forwardNodes[i].visited = false;
        backwardNodes[i].visited = false;
    }

    // Insert the source and target into their appropriate priority queue
    insert_element_minheap(forwardHeap, 0.0f, source);
    insert_element_minheap(backwardHeap, 0.0f, target);
    bool targetReached = false;
    int meetingNode = -1;

    while (!minheap_empty(forwardHeap) && !minheap_empty(backwardHeap)) {

        double forwardMin = peek_min_key(forwardHeap);
        double backwardMin = peek_min_key(backwardHeap);

        // If the sum of the closestby nodes in the queues is bigger than the known shortest distance,
        // then the already known distance is the shortest one
        if (forwardMin + backwardMin >= result->distance) {
            targetReached = true;
            break;
        }

        // Choose which search to relax based on their priority
        if (forwardHeap->curSize <= backwardHeap->curSize) {
            HeapNode* forwardCurNode = extract_min_minheap(forwardHeap);

            if (backwardNodes[forwardCurNode->value].visited) {
                double totalDistance = forwardNodes[forwardCurNode->value].dist + backwardNodes[forwardCurNode->value].dist;
                if (totalDistance < result->distance) {
                    result->distance = totalDistance;
                    meetingNode = forwardCurNode->value;
                }
            }

            forwardNodes[forwardCurNode->value].visited = true;
            result->nodesVisited++;
            
            // Relax forward
            AdjacencyListNode* forwardNeighbour = graph->adjacencyList[forwardCurNode->value];
            while (forwardNeighbour) {
                // Skip if already visited
                if (forwardNodes[forwardNeighbour->node].visited || forwardNeighbour->weight < 0) {
                    forwardNeighbour = forwardNeighbour->next;
                    continue;
                }

                double total_dist = forwardNodes[forwardCurNode->value].dist + forwardNeighbour->weight;

                if (forwardNodes[forwardNeighbour->node].dist > total_dist) {
                    // Update known distance if the distance through curNode is shorter than the previously known path
                    forwardNodes[forwardNeighbour->node].dist = total_dist;
                    forwardNodes[forwardNeighbour->node].parent = forwardCurNode->value;

                    // Update the node in the queue aswell, decrease its key if it's already in the heap, insert it otherwise
                    if (element_exists_minheap(forwardHeap, forwardNeighbour->node)) {
                        decrease_key_minheap(forwardHeap, forwardNeighbour->node, forwardNodes[forwardNeighbour->node].dist);
                    }
                    else {
                        insert_element_minheap(forwardHeap, forwardNodes[forwardNeighbour->node].dist, forwardNeighbour->node);
                    }
                }

                // If a neighbouring node has already been visited by the backward search, 
                // then a path between the source and target has been found
                if (backwardNodes[forwardNeighbour->node].visited) {
                    double totalDistance = forwardNodes[forwardCurNode->value].dist + forwardNeighbour->weight + backwardNodes[forwardNeighbour->node].dist;
                    if (totalDistance < result->distance) {
                        result->distance = totalDistance;
                        meetingNode = forwardNeighbour->node;
                    } 
                }

                forwardNeighbour = forwardNeighbour->next;
            }
            free(forwardCurNode);
        }
        else {
            HeapNode* backwardCurNode = extract_min_minheap(backwardHeap);

            if (forwardNodes[backwardCurNode->value].visited) {
                double totalDistance = backwardNodes[backwardCurNode->value].dist + forwardNodes[backwardCurNode->value].dist;
                if (totalDistance < result->distance) {
                    result->distance = totalDistance;
                    meetingNode = backwardCurNode->value;
                }
            }

            backwardNodes[backwardCurNode->value].visited = true;
            result->nodesVisited++;

            // Relax backward
            AdjacencyListNode* backwardNeighbour = reversedGraph->adjacencyList[backwardCurNode->value];
            while (backwardNeighbour) {
                if (backwardNodes[backwardNeighbour->node].visited || backwardNeighbour->weight < 0) {
                    backwardNeighbour = backwardNeighbour->next;
                    continue;
                }

                double total_dist = backwardNodes[backwardCurNode->value].dist + backwardNeighbour->weight;

                if (backwardNodes[backwardNeighbour->node].dist > total_dist) {
                    backwardNodes[backwardNeighbour->node].dist = total_dist;
                    backwardNodes[backwardNeighbour->node].parent = backwardCurNode->value;

                    if (element_exists_minheap(backwardHeap, backwardNeighbour->node)) {
                        decrease_key_minheap(backwardHeap, backwardNeighbour->node, backwardNodes[backwardNeighbour->node].dist);
                    }
                    else {
                        insert_element_minheap(backwardHeap, backwardNodes[backwardNeighbour->node].dist, backwardNeighbour->node);
                    }
                }

                if (forwardNodes[backwardNeighbour->node].visited) {
                    double totalDistance = forwardNodes[backwardNeighbour->node].dist + backwardNeighbour->weight + backwardNodes[backwardCurNode->value].dist;
                    if (totalDistance < result->distance) {
                        result->distance = totalDistance;
                        meetingNode = backwardNeighbour->node;
                    }
                }

                backwardNeighbour = backwardNeighbour->next;
            }

            free(backwardCurNode);
        }
    }

    // Unnecessary for normal execution, only used for visualization
    if (DRAW) {
        bool* visited = malloc(graph->vertexAmount * sizeof(bool));
        bool* onPath = malloc(graph->vertexAmount * sizeof(bool));

        if (targetReached && meetingNode != -1) {
            int v = meetingNode;

            while (v != -1) {
                onPath[v] = true;
                v = forwardNodes[v].parent;
            }

            v = meetingNode;

            while (v != -1) {
                onPath[v] = true;
                v = backwardNodes[v].parent;
            }
        }

        for (int i = 0; i < graph->vertexAmount; i++) {
            visited[i] = forwardNodes[i].visited || backwardNodes[i].visited;
        }

        save_nodes_to_csv("nodes.csv", graph->vertexAmount, coords[0], coords[1], visited, onPath);
    }

    // Cleanup
    free_minheap(forwardHeap);
    free_minheap(backwardHeap);
    free(forwardNodes);
    free(backwardNodes);
    debug_close();

    if (targetReached) {
        return result;
    }
    else {
        free(result->path);
        free(result);
        return NULL;
    }
}

RouteResult* Bidirectional_Dijkstra_fib(Graph* graph, Graph* reversedGraph, int source, int target) {

}

RouteResult* Bidirectional_Dijkstra_pairing(Graph* graph, Graph* reversedGraph, int source, int target) {

}

static inline double euclidian_heuristic(long fromX, long fromY, long toX, long toY) {
    double lat = fromY / 1000000.0 * M_PI / 180.0;
    double dx = (fromX - toX) * cos(lat) * 1.11;
    double dy = (fromY - toY) * 1.11;
    return sqrt(dx * dx + dy * dy);
}

double hamilton_heuristic(long fromX, long fromY, long toX, long toY) {
    double lat = fromY / 1000000.0 * M_PI / 180.0;
    double dx = (fromX - toX) * cos(lat) * 1.11;
    double dy = (fromY - toY) * 1.11;
    return (fabs(dx) + fabs(dy));
}

double circular_heuristic(int from, int to) {
    return 0.5f * abs(from - to);
}

double deg2rad(double deg) {
    return deg * (M_PI / 180.0);
}

double haversine_heuristic(long fromX, long fromY, long toX, long toY) {
    double lat1 = deg2rad(fromX/INTEGER_TO_DEGREE);
    double lon1 = deg2rad(fromY/INTEGER_TO_DEGREE);
    double lat2 = deg2rad(toX/INTEGER_TO_DEGREE);
    double lon2 = deg2rad(toY/INTEGER_TO_DEGREE);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = sin(dLat/2) * sin(dLat/2) + cos(lat1)*cos(lat2)*sin(dLon/2)*sin(dLon/2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return EARTH_RADIUS * c * SCALE_FACTOR;
}

RouteResult* A_Star_min(Graph* graph, long** coords, int source, int target) {
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    if (DEBUG) debug_init("A_star_log.txt");

    long* xCoords = coords[0];
    long* yCoords = coords[1];

    Minheap* heap = create_minheap(graph->vertexAmount);

    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    typedef struct NodeData {
        double g_score;
        double f_score;
        double h_score;
        bool visited;
    } NodeData;

    NodeData* nodes = malloc(graph->vertexAmount * sizeof(NodeData));

    for (int i = 0; i < graph->vertexAmount; i++) {
        nodes[i].visited = false;
        result->path[i] = -1;

        if (i == source) {
            nodes[i].g_score = 0.0;
            nodes[i].h_score = euclidian_heuristic(
                xCoords[i], yCoords[i],
                xCoords[target], yCoords[target]
            );
            nodes[i].f_score = nodes[i].h_score;
        } else {
            nodes[i].g_score = INFINITY;
            nodes[i].f_score = INFINITY;
            nodes[i].h_score = NAN;
        }
    }

    insert_element_minheap(heap, 0.0, source);

    bool targetReached = false;

    while (!minheap_empty(heap)) {
        HeapNode* curNode = extract_min_minheap(heap);

        if (curNode->value == target) {
            result->distance = nodes[curNode->value].g_score;
            targetReached = true;
            free(curNode);
            break;
        }

        nodes[curNode->value].visited = true;
        result->nodesVisited++;

        AdjacencyListNode* neighbour = graph->adjacencyList[curNode->value];
        while (neighbour) {
            double tentative_g_score = nodes[curNode->value].g_score + neighbour->weight;

            if (!nodes[neighbour->node].visited && tentative_g_score < nodes[neighbour->node].g_score) {
                nodes[neighbour->node].g_score = tentative_g_score;

                if (isnan(nodes[neighbour->node].h_score)) {
                    nodes[neighbour->node].h_score = euclidian_heuristic(
                        xCoords[neighbour->node], yCoords[neighbour->node],
                        xCoords[target], yCoords[target]
                    );
                }

                nodes[neighbour->node].f_score =
                    tentative_g_score + nodes[neighbour->node].h_score;

                if (element_exists_minheap(heap, neighbour->node)) {
                    decrease_key_minheap(heap, neighbour->node, nodes[neighbour->node].f_score);
                } else {
                    insert_element_minheap(heap, nodes[neighbour->node].f_score, neighbour->node);
                }

                result->path[neighbour->node] = curNode->value;
            }

            neighbour = neighbour->next;
        }

        free(curNode);
    }

    

    if (DRAW) {
        bool* onPath = malloc(graph->vertexAmount * sizeof(bool));
        bool* visited = malloc(graph->vertexAmount * sizeof(bool));

        if (targetReached) {
            int current = target;
            while (current != -1) {
                onPath[current] = true;
                current = result->path[current];
            }
        }
        for (int i = 0; i < graph->vertexAmount; i++) {
            visited[i] = nodes[i].visited;
        }

        save_nodes_to_csv(
            "nodes.csv",
            graph->vertexAmount,
            xCoords,
            yCoords,
            visited,
            onPath
        );

        free(visited);
        free(onPath);
    }

    DEBUG_PRINT("Found heuristic: %f, real distance: %f\n", nodes[source].h_score, result->distance < INFINITY ? result->distance: 0);
    // --- Cleanup ---
    free(nodes);
    free_minheap(heap);

    if (targetReached) {
        return result;
    } else {
        free(result->path);
        free(result);
        return NULL;
    }
}

RouteResult* A_Star_pairing(Graph* graph, long** coords, int source, int target) {
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    PairingHeap* heap = create_pairingheap();
    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    double* dist = malloc(graph->vertexAmount * sizeof(double));
    bool* visited = malloc(graph->vertexAmount * sizeof(bool));
    double* heuristics = malloc(graph->vertexAmount * sizeof(double));
    PairingHeapNode** heapNodes = malloc(graph->vertexAmount * sizeof(PairingHeapNode*));

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            dist[i] = 0.0f;
        }
        else {
            dist[i] = INFINITY;
        }
        result->path[i] = -1;
        heuristics[i] = circular_heuristic(i, target);
        heapNodes[i] = NULL;
        visited[i] = false;
    }

    heapNodes[source] = insert_pairingheap_element(heap, 0.0f, source);

    while (!empty_pairingheap(heap)) {
        PairingHeapNode* curNode = delete_min_pairing(heap);

        if (curNode->value == target) {
            result->distance = dist[curNode->value];
            free(dist);
            free(visited);
            free(heapNodes);
            free(curNode);
            free_pairingheap(heap);
            return result;
        }

        visited[curNode->value] = true;
        result->nodesVisited++;

        AdjacencyListNode* neighbour = graph->adjacencyList[curNode->value];
        while (neighbour) {
            double weight = neighbour->weight;
            double g = dist[curNode->value] + weight;

            double h = heuristics[neighbour->node];

            double f = g + h;
            if (!visited[neighbour->node] &&
                dist[neighbour->node] > g
            ) {
                dist[neighbour->node] = g;

                if (heapNodes[neighbour->node]) {
                    decrease_key_pairingheap(heap, heapNodes[neighbour->node], f);
                    result->path[neighbour->node] = curNode->value;
                }
                else {
                    heapNodes[neighbour->node] = insert_pairingheap_element(heap, f, neighbour->node);
                    result->path[neighbour->node] = curNode->value;
                }
            }

            neighbour = neighbour->next;
        }
        free(curNode);
    }

    free(result->path);
    free(result);
    free(dist);
    free(visited);
    free(heapNodes);
    free_pairingheap(heap);
    return NULL;
}

RouteResult* Bidirectional_A_Star_min(Graph* graph, Graph* reversedGraph, long** coords, int source, int target) {
    if (source == target || source < 0 || source >= graph->vertexAmount || target < 0 || target >= graph->vertexAmount) {
        return NULL;
    }

    long* xCoords = coords[0];
    long* yCoords = coords[1];

    Minheap* forwardHeap = create_minheap(graph->vertexAmount);
    Minheap* backwardHeap = create_minheap(graph->vertexAmount);
    RouteResult* result = malloc(sizeof(RouteResult));
    result->distance = INFINITY;
    result->path = malloc(graph->vertexAmount * sizeof(int));
    result->nodesVisited = 0;
    result->maxNodes = graph->vertexAmount;

    typedef struct NodeData {
        double g_score;
        double h_score;
        double p_score;
        double f_score;
        int parent;
        bool visited;
    } NodeData;

    NodeData* forwardNodes = malloc(graph->vertexAmount * sizeof(NodeData));
    NodeData* backwardNodes = malloc(graph->vertexAmount * sizeof(NodeData));
    double hb_t = euclidian_heuristic(
        xCoords[target], 
        yCoords[target], 
        xCoords[source], 
        yCoords[source]
    );
    double hf_s = euclidian_heuristic(
        xCoords[source], 
        yCoords[source], 
        xCoords[target], 
        yCoords[target]
    );

    for (int i = 0; i < graph->vertexAmount; i++) {
        if (i == source) {
            forwardNodes[i].g_score = 0.0f;
            forwardNodes[i].h_score = hf_s;
            forwardNodes[i].p_score = 0.0;
            forwardNodes[i].f_score = forwardNodes[i].p_score;
        }
        else {
            forwardNodes[i].g_score = INFINITY;
            forwardNodes[i].h_score = NAN;
            forwardNodes[i].p_score = NAN;
            forwardNodes[i].f_score = INFINITY;
        }

        if (i == target) {
            backwardNodes[i].g_score = 0.0f;
            backwardNodes[i].h_score = hb_t;
            backwardNodes[i].p_score = 0.0;
            backwardNodes[i].f_score = backwardNodes[i].p_score;
        }
        else {
            backwardNodes[i].g_score = INFINITY;
            backwardNodes[i].h_score = NAN;
            backwardNodes[i].p_score = NAN;
            backwardNodes[i].f_score = INFINITY;
        }
        result->path[i] = -1;
        forwardNodes[i].visited = false;
        backwardNodes[i].visited = false;
        forwardNodes[i].parent = -1;
        backwardNodes[i].parent = -1;
    }

    insert_element_minheap(forwardHeap, 0.0f, source);
    insert_element_minheap(backwardHeap, 0.0f, target);
    bool targetReached = false;
    int meetingNode = -1;

    while (!minheap_empty(forwardHeap) && !minheap_empty(backwardHeap)) {
        double forwardMin = peek_min_key(forwardHeap);
        double backwardMin = peek_min_key(backwardHeap);

        if (forwardMin + backwardMin >= result->distance) {
            targetReached = true;
            break;
        }

        if (forwardHeap->curSize <= backwardHeap->curSize) {
            HeapNode* forwardCurNode = extract_min_minheap(forwardHeap);

            if (forwardNodes[forwardCurNode->value].visited) {
                free(forwardCurNode);
                continue;
            }

            if (backwardNodes[forwardCurNode->value].visited) {
                double totalDistance = forwardNodes[forwardCurNode->value].g_score + backwardNodes[forwardCurNode->value].g_score;
                if (totalDistance < result->distance) {
                    result->distance = totalDistance;
                    meetingNode = forwardCurNode->value;
                }
            }

            forwardNodes[forwardCurNode->value].visited = true;
            result->nodesVisited++;

            for (
                AdjacencyListNode* forwardNeighbour = graph->adjacencyList[forwardCurNode->value]; 
                forwardNeighbour != NULL; 
                forwardNeighbour = forwardNeighbour->next
            ) {
                if (forwardNodes[forwardNeighbour->node].visited || forwardNeighbour->weight < 0) continue;

                double tentative_g_score = forwardNodes[forwardCurNode->value].g_score + forwardNeighbour->weight;

                if (tentative_g_score < forwardNodes[forwardNeighbour->node].g_score) {
                    forwardNodes[forwardNeighbour->node].g_score = tentative_g_score;
                    forwardNodes[forwardNeighbour->node].parent = forwardCurNode->value;

                    if (isnan(forwardNodes[forwardNeighbour->node].h_score)) {
                        forwardNodes[forwardNeighbour->node].h_score = euclidian_heuristic(
                            xCoords[forwardNeighbour->node], 
                            yCoords[forwardNeighbour->node], 
                            xCoords[target], 
                            yCoords[target]
                        );
                        backwardNodes[forwardNeighbour->node].h_score = euclidian_heuristic(
                            xCoords[forwardNeighbour->node], 
                            yCoords[forwardNeighbour->node], 
                            xCoords[source], 
                            yCoords[source]
                        );
                    }
                    
                    forwardNodes[forwardNeighbour->node].p_score = 
                        0.5 * (forwardNodes[forwardNeighbour->node].h_score - backwardNodes[forwardNeighbour->node].h_score)
                        + 0.5 * hb_t;

                    forwardNodes[forwardNeighbour->node].f_score = 
                        forwardNodes[forwardNeighbour->node].g_score + forwardNodes[forwardNeighbour->node].p_score;
                    
                    insert_element_minheap(forwardHeap, forwardNodes[forwardNeighbour->node].f_score, forwardNeighbour->node);

                    if (backwardNodes[forwardNeighbour->node].visited) {
                        double totalDistance = forwardNodes[forwardNeighbour->node].g_score + backwardNodes[forwardNeighbour->node].g_score;
                        if (totalDistance < result->distance) {
                            result->distance = totalDistance;
                            meetingNode = forwardNeighbour->node;
                        } 
                    }
                }
            }
            free(forwardCurNode);
        }
        else {
            HeapNode* backwardCurNode = extract_min_minheap(backwardHeap);

            if (backwardNodes[backwardCurNode->value].visited) {
                free(backwardCurNode);
                continue;
            }

            if (forwardNodes[backwardCurNode->value].visited) {
                double totalDistance = forwardNodes[backwardCurNode->value].g_score + backwardNodes[backwardCurNode->value].g_score;
                if (totalDistance < result->distance) {
                    result->distance = totalDistance;
                    meetingNode = backwardCurNode->value;
                }
            }

            backwardNodes[backwardCurNode->value].visited = true;
            result->nodesVisited++;

            for (
                AdjacencyListNode* backwardNeighbour = reversedGraph->adjacencyList[backwardCurNode->value];
                backwardNeighbour != NULL;
                backwardNeighbour = backwardNeighbour->next
            ) {
                if (backwardNodes[backwardNeighbour->node].visited || backwardNeighbour->weight < 0) continue;

                double tentative_g_score = backwardNodes[backwardCurNode->value].g_score + backwardNeighbour->weight;

                if (backwardNodes[backwardNeighbour->node].g_score > tentative_g_score) {
                    backwardNodes[backwardNeighbour->node].g_score = tentative_g_score;
                    backwardNodes[backwardNeighbour->node].parent = backwardCurNode->value;

                    if (isnan(backwardNodes[backwardNeighbour->node].h_score)) {
                        backwardNodes[backwardNeighbour->node].h_score = euclidian_heuristic(
                            xCoords[backwardNeighbour->node], 
                            yCoords[backwardNeighbour->node], 
                            xCoords[source], 
                            yCoords[source]
                        );
                        forwardNodes[backwardNeighbour->node].h_score = euclidian_heuristic(
                            xCoords[backwardNeighbour->node], 
                            yCoords[backwardNeighbour->node], 
                            xCoords[target], 
                            yCoords[target]
                        );
                    }

                    backwardNodes[backwardNeighbour->node].p_score = 
                        0.5 * (backwardNodes[backwardNeighbour->node].h_score - forwardNodes[backwardNeighbour->node].h_score)
                        + 0.5 * hf_s;

                    backwardNodes[backwardNeighbour->node].f_score = 
                        backwardNodes[backwardNeighbour->node].g_score + backwardNodes[backwardNeighbour->node].p_score;
                    
                    insert_element_minheap(backwardHeap, backwardNodes[backwardNeighbour->node].f_score, backwardNeighbour->node);

                    if (forwardNodes[backwardNeighbour->node].visited) {
                        double totalDistance = forwardNodes[backwardNeighbour->node].g_score + backwardNodes[backwardNeighbour->node].g_score;
                        if (totalDistance < result->distance) {
                            result->distance = totalDistance;
                            meetingNode = backwardNeighbour->node;
                        }
                    }
                }
            }
            free(backwardCurNode);
        }
    }

    if (DRAW) {
        bool* visited = malloc(graph->vertexAmount * sizeof(bool));
        bool* onPath = malloc(graph->vertexAmount * sizeof(bool));

        if (targetReached && meetingNode != -1) {
            int v = meetingNode;

            while (v != -1) {
                onPath[v] = true;
                v = forwardNodes[v].parent;
            }

            v = meetingNode;

            while (v != -1) {
                onPath[v] = true;
                v = backwardNodes[v].parent;
            }
        }

        for (int i = 0; i < graph->vertexAmount; i++) {
            visited[i] = forwardNodes[i].visited || backwardNodes[i].visited;
        }

        save_nodes_to_csv("nodes.csv", graph->vertexAmount, coords[0], coords[1], visited, onPath);
    }

    free(forwardNodes);
    free(backwardNodes);
    free_minheap(forwardHeap);
    free_minheap(backwardHeap);

    if (targetReached) {
        return result;
    }
    else {
        free(result->path);
        free(result);
        return NULL;
    }
}

RouteResult* Bidirectional_A_Star_fib(Graph* graph, Graph* reversedGraph, long** coords, int source, int target) {

}

RouteResult* Bidirectional_A_Star_pairing(Graph* graph, Graph* reversedGraph, long** coords, int source, int target) {

}

#endif
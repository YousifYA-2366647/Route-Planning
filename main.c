#include <stdio.h>

#include "Header Files/graph.h"
#include "Header Files/routealgorithms.h"

#include <time.h>
#include <stdint.h>

typedef struct {
    float mean;
    float variance;
    float stdDev;
} Stats;

Stats calculate_stats(float* data, int n) {
    Stats s = {0};
    if (n <= 0) return s;

    float sum = 0.0f;
    for (int i = 0; i < n; i++) sum += data[i];
    s.mean = sum / n;

    if (n > 1) {
        float sq_sum = 0.0f;
        for (int i = 0; i < n; i++) {
            sq_sum += powf(data[i] - s.mean, 2);
        }
        s.variance = sq_sum / (n - 1);
        s.stdDev = sqrtf(s.variance);
    }
    return s;
}

void test_dijkstra(
    int iterations, int benchmarks, Graph* graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                            Dijkstra California                                      |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-11s %-10s|\n",
        "Di(s)", "Vi:Di");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = Dijkstra_min(graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void test_dijkstra_min(
    int iterations, int benchmarks, Graph* graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                       Dijkstra Minheap California                                  |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-11s %-10s|\n",
        "Di(s)", "Vi:Di");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = Dijkstra_min(graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void test_dijkstra_fib(
    int iterations, int benchmarks, Graph* graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                    Dijkstra Fibheap California                                      |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-11s %-10s|\n",
        "Di(s)", "Vi:Di");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = Dijkstra_fib(graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void test_dijkstra_pairing(
    int iterations, int benchmarks, Graph* graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                     Dijkstra Pairing California                                     |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-9s %-9s %-11s %-10s|\n",
        "From", "To", "Di(s)", "Vi:Di");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = Dijkstra_pairing(graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", source, target, stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void test_bidirectional_dijkstra(
    int iterations, int benchmarks, Graph* graph, Graph* reversed_graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                           Bidirectional Dijkstra California                         |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-11s %-10s|\n",
        "BiDi(s)", "Vi:BiDi");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = Bidirectional_Dijkstra_min(graph, reversed_graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void test_A_star(
    int iterations, int benchmarks, Graph* graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                               A* California                                         |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-11s %-10s|\n",
        "A*(s)", "Vi:A*");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;
        // int source = 478530;
        // int target = 151260;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = A_Star_min(graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void test_bidirectional_A_star(
    int iterations, int benchmarks, Graph* graph, Graph* reversed_graph, long** coords, int* sources, int* targets
) {
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                       Bidirectional A* California                                   |\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|%-11s %-10s|\n",
        "biA*(s)", "Vi:biA*");
    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= benchmarks; i++) {

        float* times = malloc(iterations * sizeof(float));
        uint64_t* visits = calloc(iterations, sizeof(uint64_t));

        int source = sources[i-1];
        int target = targets[i-1];
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {

            clock_t s2 = clock();
            RouteResult* result = Bidirectional_A_Star_min(graph, reversed_graph, coords, source, target);
            times[j] = (float)(clock() - s2) / CLOCKS_PER_SEC;

            if (result) { visits[j] = result->nodesVisited; free(result->path); free(result); }
        }

        Stats stats = calculate_stats(times, iterations);
        uint64_t totalVisits = 0;
        for (int j = 0; j < iterations; j++) { totalVisits += visits[j]; }
        float meanVisits = totalVisits / (float)iterations;

        printf("%-9.4f | %-8.1f\n", stats.mean, meanVisits);

        free(times); 
        free(visits);
    }
}

void known_graph_performance_test(int iterations, int benchmarks) {
    srand((unsigned int)time(NULL));

    int vertices = 1890815;
    //int vertices = 264346;
    //int vertices = 10000, edges = 1000000;

    Graph* graph = create_graph(vertices, 10000, true);
    long** coords = load_graph_dimacs(graph, "./Resources/USA-road-d.CAL.gr", "./Resources/USA-road-d.CAL.co");
    //long** coords = load_graph_dimacs(graph, "./Resources/USA-road-d.NY.gr", "./Resources/USA-road-d.NY.co");
    //long** coords = load_random_graph_coords(graph, edges, (int)(log(edges) + 0.5));
    Graph* reversed_graph = graph->directed ? reverse_graph(graph): graph;

    int* sources = malloc(benchmarks * sizeof(int));
    int* targets = malloc(benchmarks* sizeof(int));

    printf("Sources | Targets\n");
    for (int i = 1; i <= benchmarks; i++) {
        sources[i-1] = 378530; // just a source that is in the middle of california
        //sources[i-1] = 164346; // just a source that is in the middle of new york
        //sources[i-1] = vertices/2; // just a source that is in the middle of the random graph
        //sources[i-1] = rand() % vertices; // random source

        //targets[i-1] = (i * (vertices / benchmarks)) % vertices;
        //targets[i-1] = rand() % vertices; // random target
        targets[i-1] = 178530;
        printf("%-7d | %d\n", sources[i-1], targets[i-1]);
    }

    // FILE* stFile = fopen("sources_targets.txt", "r");

    // int counter = 0;
    // while (fscanf(stFile, "%d %d", &sources[counter], &targets[counter]) == 2) {
    //     counter++;
    //     if (counter >= benchmarks) {
    //         break;
    //     }
    // }

    // fclose(stFile);

    //test_dijkstra(iterations, benchmarks, graph, coords, sources, targets);
    //test_bidirectional_dijkstra(iterations, benchmarks, graph, reversed_graph, coords, sources, targets);
    //test_A_star(iterations, benchmarks, graph, coords, sources, targets);
    test_bidirectional_A_star(iterations, benchmarks, graph, reversed_graph, coords, sources, targets);
    
    // test_dijkstra_min(iterations, benchmarks, graph, coords, sources, targets);
    // test_dijkstra_fib(iterations, benchmarks, graph, coords, sources, targets);
    // test_dijkstra_pairing(iterations, benchmarks, graph, coords, sources, targets);

    if (graph->directed) free_graph(reversed_graph);
    free(sources);
    free(targets);
    free_graph(graph);
    free(coords[0]);
    free(coords[1]);
    free(coords);
}

int main() {
    srand(time(NULL));
    //Graph* graph = create_graph(1000, 100, false);

    //load_random_connected_graph(graph, 2000, 2);

    //load_erdos_renyi_model_graph(graph, 250, 0.1f);

    //load_realistic_random_graph(graph, 150);

    //load_graph_from_csv(graph, "./Resources/graphInfo.csv");

    //load_grid_graph(graph, 4, 0.6f);

    //load_graph_stanford(graphCA, "./Resources/roadNet-CA.txt");


    // int source = 0, target = 300;

    // RouteResult* dResult = Dijkstra_min(graphCA, source, target);

    // print_graph(graphCA);
    // //print_graph(graph);
    // printf("---\n");

    // if (dResult) {
    //     int curNode = target;
    //     while (dResult->path[curNode] != -1) {
    //         printf("%d %d\n", curNode, dResult->path[curNode]);
    //         curNode = dResult->path[curNode];
    //     }
    //     free(dResult);
    // }

    //print_graph_descriptive(graph);

    //save_graph_to_csv(graph, "./Resources/graphInfo.csv");

    //free_graph(graph);

    //load_random_connected_graph(graph, 350);

    //load_grid_graph(graph, 4, 0.1f);

    //heap_performance_test(30, 10);

    known_graph_performance_test(1, 1);

    //test_algorithms();

    // PairingHeap* heap = create_pairingheap();

    // for (int i = 0; i < 20; i++) {
    //     insert_pairingheap_element(heap, ((float)rand()/(float)RAND_MAX), i);
    // }

    // delete_min_pairing(heap);
    // delete_min_pairing(heap);

    // print_pairingheap(heap);

    // free_pairingheap(heap);

    return 0;
}
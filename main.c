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

// Calculates the mean, variance and standard deviation of a series of experiments
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

// Tests Dijkstra's algorithm 
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests the Binary Min-Heap on Dijkstra's algorithm
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests the Fibonacci Heap on Dijkstra's algorithm
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests the Pairing Heap on Dijkstra's algorithm
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests Bidirectional Dijkstra
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests A*
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests Bidirectional A*
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
        uint32_t* visits = calloc(iterations, sizeof(uint32_t));

        int source = sources[i-1];
        int target = targets[i-1];

        // Static source-target pair used for the visualisation script
        // int source = 478530;
        // int target = 1719333;

        for (int j = 0; j < iterations; j++) {
            // Measure time taken and visited nodes while the algorithm runs.
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

// Tests all route-planning algorithms and heap structures on a road network.
// Generates 'benchmarks' amount of source-target pairs to test the algorithms on, and tests each pair 'iterations' times.
// Total tests: benchmarks * iterations
void performance_test(int iterations, int benchmarks) {
    srand((unsigned int)time(NULL));

    int vertices = 1890815;                     // Amount of vertices in the California road network, comment out when using the California road network.
    //int vertices = 264346;                    // Amount of vertices in the New York road network, comment out when using the New York road network.
    //int vertices = 10000, edges = 1000000;    // Amount of vertices in the random graph, can be changed as needed.

    Graph* graph = create_graph(vertices, 10000, true);

    // Load in the coordinates of vertices in the road networks, comment out when needed.
    long** coords = load_graph_dimacs(graph, "./Resources/USA-road-d.CAL.gr", "./Resources/USA-road-d.CAL.co");
    //long** coords = load_graph_dimacs(graph, "./Resources/USA-road-d.NY.gr", "./Resources/USA-road-d.NY.co");
    // long** coords = load_random_graph_coords(graph, edges, (int)(log(edges) + 0.5));  // UNUSED: coordinates are meaningless in random graphs

    // Reversed graph for bidirectional search
    Graph* reversed_graph = graph->directed ? reverse_graph(graph): graph;

    int* sources = malloc(benchmarks * sizeof(int));
    int* targets = malloc(benchmarks* sizeof(int));

    printf("Sources | Targets\n");
    for (int i = 1; i <= benchmarks; i++) {
        sources[i-1] = 378530;                                      // randomly chosen node in the middle of California
        //sources[i-1] = 164346;                                    // randomly chosen node in the middle of New York
        //sources[i-1] = vertices/2;                                // randomly chosen node in the middle of a random graph
        //sources[i-1] = rand() % vertices;                         // random source

        targets[i-1] = (i * (vertices / benchmarks)) % vertices;    // uniformly chosen nodes
        //targets[i-1] = rand() % vertices;                         // random target
        //targets[i-1] = 178530;                                    // randomly chosen node at the outskirts of California

        printf("%-7d | %d\n", sources[i-1], targets[i-1]);
    }

    // Test each algorithm
    test_dijkstra(iterations, benchmarks, graph, coords, sources, targets);
    test_bidirectional_dijkstra(iterations, benchmarks, graph, reversed_graph, coords, sources, targets);
    test_A_star(iterations, benchmarks, graph, coords, sources, targets);
    test_bidirectional_A_star(iterations, benchmarks, graph, reversed_graph, coords, sources, targets);
    
    // Test each heap structure on Dijkstra
    test_dijkstra_min(iterations, benchmarks, graph, coords, sources, targets);
    test_dijkstra_fib(iterations, benchmarks, graph, coords, sources, targets);
    test_dijkstra_pairing(iterations, benchmarks, graph, coords, sources, targets);

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

    performance_test(1, 1);

    return 0;
}
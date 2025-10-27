// Compile: g++ q1.cpp -o q1 -fopenmp
// Run: ./q1
#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Check if it's safe to color vertex v with color c
bool isSafe(int v, const vector<vector<int>> &graph, const vector<int> &color, int c) {
    for (int i = 0; i < graph.size(); i++) {
        if (graph[v][i] && color[i] == c)
            return false;
    }
    return true;
}

// Parallel coloring function (fixed: taskwait moved inside loop, task for v==0 only)
void graphColoring(const vector<vector<int>> &graph, int m, vector<int> color, int v, int &count) {
    int n = graph.size();
    if (v == n) {
        #pragma omp atomic
        count++;
        return;
    }
    if (v == 0) {
        // Only spawn parallel tasks for the first vertex
        #pragma omp parallel for schedule(dynamic)
        for (int col = 1; col <= m; col++) {
            vector<int> color_copy = color;
            if (isSafe(v, graph, color_copy, col)) {
                color_copy[v] = col;
                graphColoring(graph, m, color_copy, v+1, count);
            }
        }
    } else {
        for (int col = 1; col <= m; col++) {
            if (isSafe(v, graph, color, col)) {
                color[v] = col;
                graphColoring(graph, m, color, v+1, count);
            }
        }
    }
}

int main() {
    // Example: 4-node graph
    int n = 4;
    vector<vector<int>> graph = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 0}
    };
    int m = 3; // Number of colors
    vector<int> color(n, 0);
    int count = 0;

    // Serial baseline for correctness:
    int serial_count = 0;
    // Use a regular function, not a lambda, to avoid capture issues
    function<void(const vector<vector<int>>&, int, vector<int>, int, int&)> serial_graphColoring;
    serial_graphColoring = [&](const vector<vector<int>>& graph, int m, vector<int> color, int v, int& count) {
        int n = graph.size();
        if (v == n) {
            count++;
            return;
        }
        for (int col = 1; col <= m; col++) {
            if (isSafe(v, graph, color, col)) {
                color[v] = col;
                serial_graphColoring(graph, m, color, v + 1, count);
            }
        }
    };
    serial_graphColoring(graph, m, color, 0, serial_count);

    // Parallel coloring (OpenMP)
    double start = omp_get_wtime();
    // Only need one real parallel region (all done inside graphColoring for v==0)
    graphColoring(graph, m, color, 0, count);
    double end = omp_get_wtime();

    cout << "Total valid colorings (parallel): " << count << endl;
    cout << "Total valid colorings (serial check): " << serial_count << endl;
    if (count != serial_count) {
        cout << "ERROR: Parallel and serial counts do not match! Check for correctness issues." << endl;
    } else {
        cout << "Parallel result matches serial. Correct!" << endl;
    }
    cout << "Parallel Execution Time: " << end - start << " seconds" << endl;
    return 0;
}

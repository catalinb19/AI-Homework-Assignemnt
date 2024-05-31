#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <chrono> // Include the chrono library for measuring execution time

using namespace std;
using namespace std::chrono;

int n; // number of cities
vector<pair<int, int>> cities; // city coordinates
vector<vector<int>> dist; // distance matrix
vector<bool> visited; // visited array to track visited cities
int minCost = INT_MAX; // variable to store the minimum cost of the tour

// Function to calculate Euclidean distance between two cities
int calculateDistance(pair<int, int> city1, pair<int, int> city2) {
    int dx = city1.first - city2.first;
    int dy = city1.second - city2.second;
    return round(sqrt(dx * dx + dy * dy));
}

// Depth-First Search function to find the minimum cost Hamiltonian cycle
void dfs(int city, int count, int cost, int start) {
    // Base case: if all cities have been visited and there is a path back to the starting city
    if (count == n && dist[city][start]) {
        // Update the minimum cost if the current tour cost is less
        minCost = min(minCost, cost + dist[city][start]);
        return;
    }
    // Recursively visit unvisited cities
    for (int i = 0; i < n; ++i) {
        if (!visited[i] && dist[city][i]) {
            visited[i] = true; // Mark the city as visited
            dfs(i, count + 1, cost + dist[city][i], start); // Recur for the next city
            visited[i] = false; // Backtrack and mark the city as unvisited
        }
    }
}

int main() {
    // Example: 4 cities with coordinates
    n = 4;
    cities = {
        {0, 0},   // City 1: (0, 0)
        {1, 2},   // City 2: (1, 2)
        {3, 1},   // City 3: (3, 1)
        {4, 3}    // City 4: (4, 3)
    };

    // Initialize distance matrix and calculate distances between all pairs of cities
    dist.resize(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = calculateDistance(cities[i], cities[j]);
        }
    }

    visited.resize(n, false); // Initialize the visited array
    visited[0] = true; // Start the tour from the first city

    // Measure the execution time
    auto start = high_resolution_clock::now();
    dfs(0, 1, 0, 0); // Start the DFS from the first city
    auto end = high_resolution_clock::now();

    // Calculate the duration of the DFS
    auto duration = duration_cast<microseconds>(end - start);

    // Output the minimum cost and the execution time
    cout << "Minimum cost (DFS): " << minCost << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include <cmath> // For sqrt and round

using namespace std;
using namespace chrono;

// Struct to represent a node in the search space
struct Node {
    int city, cost, count; // current city, accumulated cost, number of cities visited
    vector<bool> visited; // visited cities array
    bool operator>(const Node& other) const { // comparison operator for priority queue
        return cost > other.cost;
    }
};

int n; // number of cities
vector<pair<int, int>> cities; // city coordinates
vector<vector<int>> dist; // distance matrix

// Function to calculate Euclidean distance between two cities
int calculateDistance(pair<int, int> city1, pair<int, int> city2) {
    int dx = city1.first - city2.first;
    int dy = city1.second - city2.second;
    return round(sqrt(dx * dx + dy * dy));
}

// Uniform Cost Search function to find the minimum cost Hamiltonian cycle
int uniformCostSearch() {
    priority_queue<Node, vector<Node>, greater<Node>> pq; // priority queue to explore the nodes
    vector<bool> initialVisited(n, false); // initial visited array
    initialVisited[0] = true; // start with the first city
    pq.push({ 0, 0, 1, initialVisited }); // push the initial node to the queue
    int minCost = INT_MAX; // variable to store the minimum cost

    while (!pq.empty()) {
        Node current = pq.top(); pq.pop(); // get the node with the smallest cost
        if (current.count == n && dist[current.city][0]) { // if all cities are visited and there's a path back to start
            minCost = min(minCost, current.cost + dist[current.city][0]); // update the minimum cost
            continue;
        }
        // Explore all unvisited neighboring cities
        for (int i = 0; i < n; ++i) {
            if (!current.visited[i] && dist[current.city][i]) { // if the city is unvisited and there's a path
                vector<bool> newVisited = current.visited; // copy the visited array
                newVisited[i] = true; // mark the city as visited
                pq.push({ i, current.cost + dist[current.city][i], current.count + 1, newVisited }); // push the new node to the queue
            }
        }
    }
    return minCost; // return the minimum cost found
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

    // Initialize distance matrix and calculate distances
    dist.resize(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = calculateDistance(cities[i], cities[j]);
        }
    }

    // Measure the execution time in microseconds
    auto start = high_resolution_clock::now();
    int result = uniformCostSearch();
    auto end = high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration = duration_cast<microseconds>(end - start).count();

    // Output the minimum cost and the execution time
    cout << "Minimum cost (Uniform Cost Search): " << result << endl;
    cout << "Execution time: " << duration << " microseconds" << endl;

    return 0;
}

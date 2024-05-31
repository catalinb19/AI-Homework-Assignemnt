#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <chrono>
#include <cmath> // For sqrt and round
#include <fstream>
#include <sstream>

using namespace std;
using namespace chrono;

// Struct to represent a node in the A* search space
struct Node {
    int city, g_cost, h_cost, count; // current city, accumulated cost, heuristic cost, number of cities visited
    vector<bool> visited; // visited cities array
    bool operator>(const Node& other) const { // comparison operator for priority queue
        return (g_cost + h_cost) > (other.g_cost + other.h_cost); // Comparison based on f_cost = g_cost + h_cost
    }
};

int n; // number of cities
vector<pair<int, int>> cities; // city coordinates
vector<vector<int>> dist; // distance matrix

// Function to calculate Euclidean distance between two cities
int calculateDistance(pair<int, int> city1, pair<int, int> city2) {
    int dx = city1.first - city2.first;
    int dy = city1.second - city2.second;
    return round(sqrt(dx * dx + dy * dy)); // Euclidean distance formula
}

// Function to calculate the heuristic cost (MST + nearest neighbor to start)
int heuristic(const vector<bool>& visited, int current_city) {
    vector<int> min_edge(n, INT_MAX); // Minimum edges for each city
    int mst_cost = 0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            for (int j = 0; j < n; ++j) {
                if (i != j && !visited[j]) {
                    min_edge[i] = min(min_edge[i], dist[i][j]); // Find minimum edge for each unvisited city
                }
            }
            mst_cost += min_edge[i]; // Sum of minimum edges (approximate MST cost)
        }
    }

    // Find the nearest neighbor to the start city
    int nearest_to_start = INT_MAX;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            nearest_to_start = min(nearest_to_start, dist[current_city][i] + dist[i][0]); // Sum of distances to current city and back to start
        }
    }

    return mst_cost + nearest_to_start; // Total heuristic cost
}

// A* Search function to find the minimum cost Hamiltonian cycle
int aStarSearch() {
    priority_queue<Node, vector<Node>, greater<Node>> pq; // Priority queue to explore nodes
    vector<bool> initialVisited(n, false); // Initial visited array
    initialVisited[0] = true; // Start with the first city
    pq.push({ 0, 0, heuristic(initialVisited, 0), 1, initialVisited }); // Push the initial node to the queue
    int minCost = INT_MAX; // Variable to store the minimum cost

    while (!pq.empty()) {
        Node current = pq.top(); pq.pop(); // Get the node with the smallest f_cost
        if (current.count == n && dist[current.city][0]) { // If all cities are visited and there's a path back to start
            minCost = min(minCost, current.g_cost + dist[current.city][0]); // Update the minimum cost
            continue;
        }
        // Explore all unvisited neighboring cities
        for (int i = 0; i < n; ++i) {
            if (!current.visited[i] && dist[current.city][i]) { // If the city is unvisited and there's a path
                vector<bool> newVisited = current.visited; // Copy the visited array
                newVisited[i] = true; // Mark the city as visited
                int new_g_cost = current.g_cost + dist[current.city][i]; // Update the g_cost
                int new_h_cost = heuristic(newVisited, i); // Calculate the new h_cost
                pq.push({ i, new_g_cost, new_h_cost, current.count + 1, newVisited }); // Push the new node to the queue
            }
        }
    }
    return minCost; // Return the minimum cost found
}

// Function to parse the TSPLIB file and extract city coordinates
void parseFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.find("DIMENSION") != string::npos) {
            n = stoi(line.substr(line.find(":") + 1)); // Extract the number of cities
            cities.resize(n); // Resize the cities vector
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            for (int i = 0; i < n; ++i) {
                int index, x, y;
                file >> index >> x >> y; // Read city index and coordinates
                cities[index - 1] = { x, y }; // Store the coordinates in the cities vector
            }
        }
    }

    file.close(); // Close the file
}

int main() {
    // Parse TSPLIB file
    parseFile("att48(11).tsp");

    // Initialize distance matrix and calculate distances
    dist.resize(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            dist[i][j] = calculateDistance(cities[i], cities[j]); // Calculate distances between all pairs of cities
        }
    }

    // Measure the execution time
    auto start = high_resolution_clock::now();
    int result = aStarSearch();
    auto end = high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration = duration_cast<microseconds>(end - start);

    // Output the minimum cost and the execution time
    cout << "Minimum cost (A* Search): " << result << endl;
    cout << "Execution time: " << duration.count() << " microseconds" << endl;

    return 0;
}

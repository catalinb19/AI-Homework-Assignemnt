#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>
#include <cmath> // For sqrt and round
#include <fstream>
#include <sstream> // For file reading and string stream operations

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
    return round(sqrt(dx * dx + dy * dy)); // Euclidean distance formula
}

// Function to parse the .tsp file and extract city coordinates
void parseFile(const string& filename) {
    ifstream file(filename); // Open the file
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(1); // Exit if the file cannot be opened
    }

    string line;
    while (getline(file, line)) {
        if (line.find("DIMENSION") != string::npos) {
            n = stoi(line.substr(line.find(":") + 1)); // Extract number of cities
            cities.resize(n); // Resize the cities vector
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            // Read city coordinates
            for (int i = 0; i < n; ++i) {
                int index, x, y;
                file >> index >> x >> y; // Read index, x, and y coordinates
                cities[index - 1] = { x, y }; // Store coordinates
            }
        }
    }

    file.close(); // Close the file
}

// Uniform Cost Search function to find the minimum cost Hamiltonian cycle
int uniformCostSearch() {
    priority_queue<Node, vector<Node>, greater<Node>> pq; // Priority queue to explore nodes
    vector<bool> initialVisited(n, false); // Initial visited array
    initialVisited[0] = true; // Start with the first city
    pq.push({ 0, 0, 1, initialVisited }); // Push the initial node to the queue
    int minCost = INT_MAX; // Variable to store the minimum cost

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop(); // Get the node with the smallest cost
        if (current.count == n && dist[current.city][0]) { // If all cities are visited and there's a path back to start
            minCost = min(minCost, current.cost + dist[current.city][0]); // Update the minimum cost
            continue;
        }
        // Explore all unvisited neighboring cities
        for (int i = 0; i < n; ++i) {
            if (!current.visited[i] && dist[current.city][i]) { // If the city is unvisited and there's a path
                vector<bool> newVisited = current.visited; // Copy the visited array
                newVisited[i] = true; // Mark the city as visited
                pq.push({ i, current.cost + dist[current.city][i], current.count + 1, newVisited }); // Push the new node to the queue
            }
        }
    }
    return minCost; // Return the minimum cost found
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

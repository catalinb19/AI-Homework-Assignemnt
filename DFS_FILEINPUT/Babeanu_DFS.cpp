#include <iostream>
#include <vector>
#include <climits>
#include <chrono> // Include the chrono library for measuring execution time
#include <fstream> // Include the fstream library for file operations
#include <sstream> // Include the sstream library for string stream operations
#include <cmath> // Include the cmath library for sqrt function

using namespace std;
using namespace std::chrono;

int n; // number of cities
vector<vector<int>> dist; // distance matrix
vector<bool> visited; // visited array to track visited cities
int minCost = INT_MAX; // variable to store the minimum cost of the tour

// Function to parse the .tsp file and extract distances
void parseTSP(const string& filename) {
    ifstream file(filename); // Open the .tsp file
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        exit(1); // Exit if file cannot be opened
    }

    string line;
    bool coord_section = false; // Flag to indicate if we are in the coordinates section
    vector<pair<int, int>> coords; // Vector to store coordinates of cities

    while (getline(file, line)) {
        if (line.find("DIMENSION") != string::npos) {
            stringstream ss(line);
            string token;
            ss >> token >> n; // Extract the number of cities
            dist.resize(n, vector<int>(n, 0)); // Resize the distance matrix
        }
        else if (line == "EDGE_WEIGHT_TYPE : EUC_2D") {
            coord_section = true; // We have reached the coordinates section
        }
        else if (line == "NODE_COORD_SECTION") {
            // Read city coordinates
            for (int i = 0; i < n; ++i) {
                int index, x, y;
                file >> index >> x >> y; // Read index, x, and y coordinates
                coords.push_back(make_pair(x, y)); // Store the coordinates
            }
        }
    }

    file.close(); // Close the file

    // Calculate distances between cities based on coordinates
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int dx = coords[i].first - coords[j].first;
                int dy = coords[i].second - coords[j].second;
                dist[i][j] = dist[j][i] = round(sqrt(dx * dx + dy * dy)); // Calculate Euclidean distance
            }
        }
    }
}

// Depth-First Search function to find the minimum cost Hamiltonian cycle
void dfs(int city, int count, long long cost, int start) {
    if (count == n && dist[city][start]) {
        minCost = min(minCost, static_cast<int>(cost + dist[city][start])); // Ensure minCost is of type int
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
    // Parse TSPLIB file
    parseTSP("att48(11).tsp");

    auto start = high_resolution_clock::now(); // Start time measurement

    visited.resize(n, false); // Initialize the visited array
    dfs(0, 1, 0, 0); // Start with the first city, increment count by 1, initial cost is 0, start city is also 0

    auto stop = high_resolution_clock::now(); // Stop time measurement
    auto duration = duration_cast<microseconds>(stop - start); // Calculate the duration

    // Output the minimum cost and the execution time
    cout << "Minimum Cost: " << minCost << endl;
    cout << "Execution Time: " << duration.count() << " microseconds" << endl;

    return 0;
}

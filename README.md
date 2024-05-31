# Minimum Cost Calculation for TSP

This repository contains three projects to calculate the minimum cost for a predefined dataset using different algorithms for the Traveling Salesman Problem (TSP):

- **Depth-First Search (DFS)**: `Babeanu_DFS`
- **Uniform Cost Search (UCS)**: `Babeanu_UniformCostSearch`
- **A* Search**: `Babeanu_A_star_Search`

## File Parsers

Each algorithm includes a file parser that reads inputs from a .tsp file:
- **DFS File Input Parser**: `Babeanu_DFS_FileInput`
- **UCS File Input Parser**: `Babeanu_UniformCostSearch_FileInput`
- **A* File Input Parser**: `Babeanu_A_star_Search_FileInput`

## Datasets

The following datasets from TSPLIB have been selected and reduced for demonstration purposes:
- `a280(9).tsp`: Reduced to 9 cities
- `berlin52(10).tsp`: Reduced to 10 cities
- `att48(11).tsp`: Reduced to 11 cities

## Usage

To select the desired input file, modify the filename inside the quotation marks as shown below:
- parseTSP("att48(11).tsp")

# The Lost Ruins of Algorithmia 🏛️

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white)
![Algorithms](https://img.shields.io/badge/Algorithms-Advanced-success)
![Data Structures](https://img.shields.io/badge/Data_Structures-Optimized-blue)

## 📌 Overview

This repository contains my C++ solutions for **The Lost Ruins of Algorithmia**, the final project for my Algorithm Design course. The project involves solving a series of increasingly complex pathfinding, resource collection, and network flow problems on a 2D grid representation of ancient ruins. 

The primary focus of these implementations is **execution speed and memory optimization**, targeting maximum benchmark scores similar to competitive programming standards.

## 🚀 Problems & Implemented Algorithms

*   **Problem 1: The Time-Limited Expedition**
    *   **Goal:** Maximize the number of treasures collected on a path from `S` to `E` within a strict move budget `M`.
    *   **Approach:** Breadth-First Search (BFS) combined with Bitmask Dynamic Programming over treasure subsets.
    *   **Optimizations:** Flattened 1D DP table for cache locality, efficient bitwise iteration using `__builtin_ctz`, and early pruning of unreachable nodes.

*   **Problem 2: Mapping the Ruins**
    *   **Goal:** Compute the average minimum number of moves between all distinct reachable pairs of passable cells.
    *   **Approach:** All-Pairs Shortest Path using repeated BFS from every passable vertex, since edges are unweighted unit distances.
    *   **Optimizations:** Maintained running accumulators for the total distance sum and valid pair count directly during BFS traversal, completely avoiding an $O(V^2)$ distance matrix and strictly keeping memory usage minimal.

*   **Problem 3: The Minimal Expedition**
    *   **Goal:** Find the minimum number of moves from `S` to `E` while collecting at least `K` treasures.
    *   **Approach:** Precomputed shortest paths between all points of interest (`S`, `E`, and `T`) using BFS, followed by Bitmask DP to find the minimal distance path satisfying the treasure threshold.
    *   **Optimizations:** Reduced the continuous grid-walking problem to a discrete subset DP. This restricts the time complexity to $O(T \cdot RC + 2^T \cdot T^2)$ instead of naively exploring grid permutations.

*   **Problem 4: The Rescue Mission**
    *   **Goal:** Maximize the number of vertex-disjoint escape paths from `S` to `E`.
    *   **Approach:** Dinic's Algorithm for Maximum Flow.
    *   **Optimizations:** Modeled as a unit-capacity network with unit-vertex capacities, achieving an optimized time complexity of $O(E \sqrt{V})$ rather than the standard $O(V^2 E)$ for Dinic's.

## 🛠️ Technical Highlights

*   **Memory Layout:** Replaced vector-of-vectors with flattened 1D arrays for DP tables to drastically reduce CPU cache misses.
*   **Algorithm Reduction:** Strategically mapped grid constraints to classical graph problems (e.g., TSP variants, Maximum Flow) to apply proven optimal bounds.
*   **Complexity Analysis:** Detailed mathematical breakdowns of time and memory complexities (available in `docs/solution-report.md`).

## ⚙️ How to Compile and Run

```bash
# Compile with GCC
g++ -O3 src/p1_time_limited_expedition.cpp -o p1

# Run with standard input
./p1 < tests/p1/input1.txt
### 2. GitHub Description and Tags

```

## 📄 Documentation

For a deep dive into the algorithmic choices, correctness proofs, and complexity analysis, please refer to the [Solution Report](docs/solution-report.md) included in this repository.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
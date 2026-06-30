# Smart Power Grid & Fault Routing System

A robust C++ simulation of a Smart Electrical Grid, designed to optimize power distribution, minimize transmission losses, and provide real-time fault management.

##  Overview
This project models a power network as a **Weighted Undirected Graph**. It helps Grid Administrators calculate optimal transmission routes based on two distinct parameters: physical distance (infrastructure cost) and power loss (transmission efficiency). The system also features a critical "Fault Re-routing" protocol to restore power during infrastructure failures.

##  Key Features
* **Infrastructure Optimization:** Uses Dijkstra's algorithm to find the shortest cable path, minimizing capital expenditure.
* **Efficiency Analysis:** Implements Dijkstra's algorithm to identify the path with the lowest power dissipation percentage.
* **Fault Resilience:** Utilizes Breadth-First Search (BFS) to perform "Minimum-Hop" re-routing when a substation is taken offline, ensuring high availability.
* **Administrator Control Center:** A secure, simulation-based interface to add/remove substations and manipulate transmission lines in real-time.

##  Technical Stack
* **Language:** C++ (Standard C++11)
* **Data Structures:** Graphs (Adjacency List using HashMaps), Priority Queues (Min-Heap), Queues, Stacks.
* **Algorithms:** Dijkstra's Algorithm, Breadth-First Search (BFS).

## How to Run
1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/smart-power-grid.git](https://github.com/your-username/smart-power-grid.git)
    cd smart-power-grid
    ```
2.  **Compile the code:**
    ```bash
    g++ -std=c++11 main.cpp -o grid
    ```
3.  **Execute the system:**
    ```bash
    ./grid
    ```

##  Project Architecture
* `PowerGrid.h`: Contains the core `PowerGridNetwork` class, graph data structures, and optimization algorithms.
* `main.cpp`: Driver code that handles the user interface, administrator logic, and terminal-based simulation.


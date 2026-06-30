#ifndef POWERGRID_H
#define POWERGRID_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <climits>
#include <utility>
#include <stack>
using namespace std;
using ll = long long;


extern unordered_map<int, string> gridNames;

class PowerGridNetwork {
public:
    class Substation {
    public:
        // First is distance (km), Second is Power Loss (%)
        std::unordered_map<int, pair<int,int>> connections; 
    };

    // Static map to store substations, with integer keys
    static std::unordered_map<int, Substation> gridNodes;

    bool containsSubstation(int vname) const {
        return gridNodes.find(vname) != gridNodes.end();
    }

    void addSubstation(int vname) {
        Substation vtx;
        gridNodes[vname] = vtx; 
    }
    
    void removeSubstation(int vname) {
        if (gridNodes.find(vname) == gridNodes.end()) {
            std::cout << "Substation " << vname << " does not exist in the grid.\n";
            return;
        }

        Substation& vtx = gridNodes[vname];
        std::vector<int> keys;
        for (const auto& pair : vtx.connections) {
            keys.push_back(pair.first); 
        }

        for (int key : keys) {
            gridNodes[key].connections.erase(vname);
        }
        gridNodes.erase(vname);
    }

    void addTransmissionLine(int vname1, int vname2, int distance, int powerLoss) {
        if (gridNodes.find(vname1) == gridNodes.end() || gridNodes.find(vname2) == gridNodes.end()) {
            cout << "ENTER VALID SUBSTATION NUMBERS!!\n";
            return;
        }
        else if(gridNodes[vname1].connections.find(vname2) != gridNodes[vname1].connections.end()) {
            cout << "A TRANSMISSION LINE ALREADY EXISTS!!\n";
            return;
        }
        gridNodes[vname1].connections[vname2].first = distance;
        gridNodes[vname2].connections[vname1].first = distance;
        gridNodes[vname1].connections[vname2].second = powerLoss;
        gridNodes[vname2].connections[vname1].second = powerLoss;
    }

    void removeTransmissionLine(int vname1, int vname2) {
        if (gridNodes.find(vname1) == gridNodes.end() || gridNodes.find(vname2) == gridNodes.end()) {
            cout << "ENTER VALID SUBSTATION NUMBERS!!\n";
            return;
        }
        else if(gridNodes[vname1].connections.find(vname2) == gridNodes[vname1].connections.end()) {
            cout << "NO TRANSMISSION LINE EXISTS BETWEEN THESE NODES!!\n";
            return;
        }
        gridNodes[vname1].connections.erase(vname2);
        gridNodes[vname2].connections.erase(vname1);
    }

    void shortestCableRoute(int source, int destination) {
        std::unordered_map<int, int> dist;
        std::unordered_map<int, int> parent;
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq; 

        for (auto& vertex : gridNodes) {
            dist[vertex.first] = INT_MAX;
            parent[vertex.first] = -1;
        }

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == destination) break;

            for (const auto& nbr : gridNodes[u].connections) {
                int v = nbr.first;
                int weight = nbr.second.first; // Distance

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[destination] == INT_MAX) {
            std::cout << "No routing path found. Grid is isolated.\n";
        } else {
            std::vector<int> path;
            for (int v = destination; v != -1; v = parent[v]) {
                path.push_back(v);
            }
            std::reverse(path.begin(), path.end());

            cout << "\n[OPTIMAL CABLE ROUTE]:\n";
            for (int i = 0; i < path.size(); i++) {
                std::cout << gridNames[path[i]];
                if (i != path.size() - 1) std::cout << " -> ";
            }
            std::cout << "\nTotal Cable Distance: " << dist[destination] << " kms\n";
        }
    }

    void minimumPowerLossRoute(int source, int destination) {
        std::unordered_map<int, int> loss;  
        std::unordered_map<int, int> parent; 
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

        for (auto& vertex : gridNodes) {
            loss[vertex.first] = INT_MAX;
            parent[vertex.first] = -1;
        }

        loss[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == destination) break;

            for (const auto& nbr : gridNodes[u].connections) {
                int v = nbr.first;
                int weight = nbr.second.second; // Power Loss

                if (loss[u] + weight < loss[v]) {
                    loss[v] = loss[u] + weight;
                    parent[v] = u;
                    pq.push({loss[v], v});
                }
            }
        }

        if (loss[destination] == INT_MAX) {
            std::cout << "No routing path found. Grid is isolated.\n";
        } else {
            std::vector<int> path;
            for (int v = destination; v != -1; v = parent[v]) {
                path.push_back(v);
            }
            std::reverse(path.begin(), path.end());

            cout << "\n[EFFICIENT POWER ROUTE]:\n";
            for (int i = 0; i < path.size(); i++) {
                std::cout << gridNames[path[i]];
                if (i != path.size() - 1) std::cout << " -> ";
            }
            std::cout << "\nEstimated Total Power Loss: " << loss[destination] << " %\n";
        }
    }

    void faultReRoutingHops(int source, int destination) {
        std::unordered_map<int, int> distance;  
        std::unordered_map<int, int> intermediateHops;  
        std::unordered_map<int, int> parent; 
        std::queue<int> q; 

        for (auto& vertex : gridNodes) {
            distance[vertex.first] = -1; 
            intermediateHops[vertex.first] = -1; 
            parent[vertex.first] = -1; 
        }

        distance[source] = 0;
        intermediateHops[source] = 0;  
        q.push(source);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (u == destination) {
                break;
            }

            for (auto& nbr : gridNodes[u].connections) {
                int v = nbr.first;

                if (distance[v] == -1) {
                    distance[v] = distance[u] + 1; 
                    intermediateHops[v] = intermediateHops[u] + 1; 
                    parent[v] = u; 
                    q.push(v);
                }
            }
        }

        if (distance[destination] == -1) {
            std::cout << "CRITICAL FAILURE: No backup path found from " << gridNames[source] << " to " << gridNames[destination] << ".\n";
        } else {
            std::cout << "\n[FAULT RE-ROUTING PROTOCOL ACTIVATED]\n";
            std::cout << "Minimum Transformers/Substations to cross: " << intermediateHops[destination] << "\n";
            std::cout << "Re-routing Path: ";
            std::stack<int> path;  
            int current = destination;
            while (current != -1) {
                path.push(current);
                current = parent[current];
            }

            while (!path.empty()) {
                std::cout << gridNames[path.top()];
                if(path.size() != 1) std::cout << " ==> ";
                path.pop();
            }
            std::cout << std::endl;
        }
    }

    void Create_Power_Grid() {
        for (int i = 0; i < 10; ++i) {
            addSubstation(i);  
        }
        addTransmissionLine(0, 1, 8, 12);    
        addTransmissionLine(1, 2, 10, 15);   
        addTransmissionLine(2, 4, 8, 10);    
        addTransmissionLine(2, 3, 6, 8);     
        addTransmissionLine(3, 5, 9, 14);    
        addTransmissionLine(5, 6, 7, 11);    
        addTransmissionLine(6, 7, 6, 9);     
        addTransmissionLine(8, 9, 15, 20);   
        addTransmissionLine(0, 5, 20, 30);   
        addTransmissionLine(1, 4, 12, 18);   
        addTransmissionLine(3, 8, 18, 25);   
        addTransmissionLine(2, 7, 14, 22);   
        addTransmissionLine(7, 8, 9, 12);    
        addTransmissionLine(6, 8, 25, 35);   
    }

    void display_Substations() const {
        std::cout << "\n[ACTIVE SUBSTATIONS IN THE GRID]\n";
        vector<pair<ll,string>> vec;
        for (const auto& pair : gridNodes) {
            vec.push_back({pair.first+1, gridNames[pair.first]});
        }
        sort(vec.begin(), vec.end());
        for(auto k:vec) {
            std::cout << k.first << ". " << k.second << "\n";
        }
        std::cout << "\n";
    }

    void display_Network() const {
        std::cout << "\t              SMART POWER GRID NETWORK\n";
        std::cout << "\t              ------------------------\n";
        std::cout << "       --------------------------------------------------------\n";
        std::cout << "FROM GRID----------------TO GRID----------------DIST(KM)---LOSS(%)\n";
        
        for (const auto& pair : gridNodes) {
            const int& key = pair.first;
            const Substation& vtx = pair.second;
            std::string str = gridNames[key] + " =>\n";  

            for (const auto& nbr : vtx.connections) {
                std::string nbr_str = gridNames[nbr.first];  
                int dist = nbr.second.first;  
                int loss  = nbr.second.second;  

                str += "            \t         " + nbr_str;
                str += "\t\t";  
                str += std::to_string(dist);
                str += "       \t   ";
                str += std::to_string(loss);
                str += "       \n";  
            }
            std::cout << str;
        }
        std::cout << "       --------------------------------------------------------\n";
    }
};
// static member definition
std::unordered_map<int, PowerGridNetwork::Substation> PowerGridNetwork::gridNodes;
#endif
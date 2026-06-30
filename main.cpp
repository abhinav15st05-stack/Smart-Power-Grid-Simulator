#include <iostream>
#include <string>
#include "PowerGrid.h"

using namespace std;
using ll = long long;

// Initialize the global structures
unordered_map<int, string> gridNames;

int main() {
    // Adding first 10 substations
    gridNames[0] = "Alpha Main Grid   ";
    gridNames[1] = "Sector-12 Node    ";
    gridNames[2] = "City Center Sub   ";
    gridNames[3] = "Industrial Hub    ";
    gridNames[4] = "Tech Park Trans.  ";
    gridNames[5] = "North Zone Grid   ";
    gridNames[6] = "West End Station  ";
    gridNames[7] = "Airport Power Hub ";
    gridNames[8] = "South Valley Node ";
    gridNames[9] = "Delta Backup Grid ";

    PowerGridNetwork g;
    g.Create_Power_Grid();
    
    cout << "\n\t\t\t*** SMART POWER DISTRIBUTION SYSTEM ***\n";
    
    while(true) {
        cout << "\n\t\t\t\t~ CONTROL PANEL ~\n\n";
        cout << "1. LIST ALL ACTIVE SUBSTATIONS\n";
        cout << "2. SHOW TRANSMISSION NETWORK MAP\n";
        cout << "3. CALCULATE ROUTE FOR MINIMUM CABLE DISTANCE (Cost Optimization)\n";
        cout << "4. SIMULATE FAULT RE-ROUTING (Minimum Hops/Transformers)\n";
        cout << "5. CALCULATE ROUTE FOR MINIMUM POWER LOSS (Efficiency Optimization)\n";
        cout << "6. GRID CONTROL CENTER (ADMINISTRATOR ACCESS)\n";
        cout << "7. SYSTEM SHUTDOWN (EXIT)\n";
        cout << "\nENTER COMMAND (1 to 7): ";
        
        string input;
        cin >> input;
        if (!(input.length() == 1 && isdigit(input[0]))) {
            cout << "\n!! INVALID COMMAND SEQUENCE !!\n\n";
            continue;
        }
        int choice = input[0] - '0';
        cout << "\n=======================================================\n";
        
        if(choice == 7) {
            cout << "Shutting down system...\n";
            break;
        }
        else if(choice == 1) {
            g.display_Substations();
        }
        else if(choice == 2) {
            g.display_Network();
        }
        else if(choice == 3) {
            g.display_Substations();
            cout << "ENTER SOURCE SUBSTATION NUMBER: ";
            ll x; cin >> x;
            cout << "ENTER DESTINATION SUBSTATION NUMBER: ";
            ll y; cin >> y;
            g.shortestCableRoute(x-1, y-1);
        }
        else if(choice == 4) {
            g.display_Substations();
            cout << "ENTER FAULT ORIGIN (SOURCE) NUMBER: ";
            ll x; cin >> x;
            cout << "ENTER TARGET (DESTINATION) NUMBER: ";
            ll y; cin >> y;
            g.faultReRoutingHops(x-1, y-1);
        }
        else if(choice == 5) {
            g.display_Substations();
            cout << "ENTER SOURCE SUBSTATION NUMBER: ";
            ll x; cin >> x;
            cout << "ENTER DESTINATION SUBSTATION NUMBER: ";
            ll y; cin >> y;
            g.minimumPowerLossRoute(x-1, y-1);
        }
        else if(choice == 6) {
            cout << "ENTER MASTER CLEARANCE KEY (12345): ";
            string s;
            cin >> s;
            if(s == "12345") {
                cout << "\n!! GRID CONTROL CENTER UNLOCKED !!\n\n";
                while(true) {
                    cout << "\n\t\t~ ADMIN ACTIONS ~\n";
                    cout << "1. INSTALL NEW SUBSTATION\n";
                    cout << "2. SIMULATE SUBSTATION FAILURE (Remove Node)\n";
                    cout << "3. LAY NEW TRANSMISSION LINE\n";
                    cout << "4. CUT TRANSMISSION LINE (Simulate Line Fault)\n";
                    cout << "5. RETURN TO MAIN MENU\n";
                    cout << "ENTER COMMAND (1 to 5): ";
                    
                    string str;
                    cin >> str;
                    ll num = str[0] - '0';
                    if(num == 1) {
                        cout << "ENTER SUBSTATION NAME (No spaces, use underscores): ";
                        string s1;
                        cin >> s1;
                        g.addSubstation(gridNames.size());
                        while(s1.size() < 18) {
                            s1 += " ";
                        }
                        gridNames[gridNames.size()] = s1;
                        cout << "Node successfully added to grid.\n";
                    }
                    else if(num == 2) {
                        g.display_Substations();
                        cout << "ENTER SUBSTATION TO TAKE OFFLINE (NUMBER): ";
                        int x; cin >> x;
                        g.removeSubstation(x-1);
                        cout << "\nWARNING: SUBSTATION " << gridNames[x-1] << " HAS BEEN TAKEN OFFLINE.\n";
                    }
                    else if(num == 3) {
                        g.display_Substations();
                        cout << "ENTER FIRST SUBSTATION NUMBER: ";
                        ll x; cin >> x;
                        cout << "ENTER SECOND SUBSTATION NUMBER: ";
                        ll y; cin >> y;
                        cout << "ENTER DISTANCE BETWEEN THEM (KM): ";
                        ll z; cin >> z;
                        cout << "ENTER ESTIMATED POWER LOSS (%): ";
                        ll w; cin >> w;
                        g.addTransmissionLine(x-1, y-1, z, w);
                        cout << "Transmission line successfully connected.\n";
                    }
                    else if(num == 4) {
                        g.display_Substations();
                        cout << "ENTER FIRST SUBSTATION NUMBER: ";
                        ll x; cin >> x;
                        cout << "ENTER SECOND SUBSTATION NUMBER: ";
                        ll y; cin >> y;
                        g.removeTransmissionLine(x-1, y-1);
                        cout << "Line connection severed.\n";
                    }
                    else break;
                }
            }
            else {
                cout << "\n!! ACCESS DENIED. INCORRECT CLEARANCE KEY !!\n\n";
            }
        }
    }
    return 0;
}
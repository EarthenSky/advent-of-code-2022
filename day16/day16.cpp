#include <stdio.h>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <unordered_map>
#include <string>

#include <algorithm>

// to run (on wsl):
// g++ -std=c++20 -Wall day16.cpp -o day16 -O2; time ./day16
 
void panic() {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d\n", EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

// NOTE: trivially visualizing all the intricacies of this algorithm & doing off by one proofs visually, etc., 
// will be a good testcase for my program visualization tools. However, they will need to be compared with actual visual debuggers, 
// because those are typically really good.

// ---------------------------------- //
// parts

std::vector<std::string> split(std::string s, std::string delim) {
    std::vector<std::string> output;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        output.push_back(token);
        s.erase(0, pos + delim.length());
    }

    output.push_back(s);

    return output;
}

int distance(
    std::unordered_map<std::string, std::vector<std::string>>& map,
    std::vector<std::string>& visited, 
    std::string& curLoc, std::string& end, int curDist
) {
    if (curLoc == end) {
        return curDist;
    }

    visited.push_back(curLoc);

    //for (auto x : visited)
    //    std::cout << x << ", ";
    //std::cout << std::endl;

    int shortest_distance = 3000;
    for (std::string adj : map[curLoc]) {
        // adj in visited
        if (std::find(visited.begin(), visited.end(), adj) != visited.end()) {
            continue;
        } else {
            int dist = distance(map, visited, adj, end, curDist + 1);
            if (dist < shortest_distance) {
                shortest_distance = dist;
            }
        }
    }

    visited.pop_back();

    return shortest_distance;
}

// returns max releasable pressure
int evaluate_paths(
    std::unordered_map<std::string, std::vector<std::string>>& map,
    std::unordered_map<std::string, int>& flowrate,
    std::vector<std::string>& visited, 
    std::vector<std::string>& opened, // NOTE: opened == visited
    std::vector<std::string>& openableLocations,
    std::unordered_map<std::string, int>& distance_map,
    std::string& curLoc, int totalPressure, int releasingPressure, int minute
) {
    if (minute == 30) {
        // early exit
        return totalPressure;
    } else if (minute > 30) {
        // release any extra minutes
        return totalPressure - releasingPressure * (minute - 30);
    } else if (opened.size() == openableLocations.size()) {
        return totalPressure + releasingPressure * (30 - minute); 
    }

    totalPressure += releasingPressure; // release before moving
    
    // note: this is a worst case est, assuming we sit here for the remaining time
    int max_pressure = totalPressure + releasingPressure * (30 - (minute+1)); 

    if (flowrate[curLoc] != 0 && std::find(opened.begin(), opened.end(), curLoc) == opened.end()) {
        // case: current location has not been opened
        opened.push_back(curLoc);

        int addend = flowrate[curLoc];
        releasingPressure += addend;

        int pressure = evaluate_paths(
            map, flowrate, visited, opened, 
            openableLocations, distance_map, curLoc, 
            totalPressure, releasingPressure, minute+1);

        if (pressure > max_pressure) {
            max_pressure = pressure;
        }

        releasingPressure -= addend;

        opened.pop_back();
    } else {
        visited.push_back(curLoc);

        // try moving to any openable locations
        for (std::string next : openableLocations) {
            if (std::count(visited.begin(), visited.end(), next) != 0 || std::count(opened.begin(), opened.end(), next) != 0) {
                // no moving to duplicate openable locations
                continue;
            } else {
                // adj in visited
                int addend = releasingPressure * (distance_map[curLoc+next] - 1);
                totalPressure += addend; // release before moving
                
                int pressure = evaluate_paths(
                    map, flowrate, visited, opened, 
                    openableLocations, distance_map, next, 
                    totalPressure, releasingPressure, minute+distance_map[curLoc+next]);

                if (pressure > max_pressure) {
                    max_pressure = pressure;
                }

                totalPressure -= addend;
            }
        }

        visited.pop_back();
    }
    
    return max_pressure;
}

/*
int evaluate_paths_w_elephant_a(
    std::unordered_map<std::string, std::vector<std::string>>& map,
    std::unordered_map<std::string, int>& flowrate,
    std::vector<std::string>& visited, 
    std::vector<std::string>& opened, 
    std::vector<std::string>& openableLocations,
    std::unordered_map<std::string, int>& distance_map,
    std::string& curLoc, 
    std::string& elephantLoc, 
    int totalPressure, int releasingPressure, 
    int minute,
    int elephantMinute
) {
    if (minute == 26) {
        // early exit
        return totalPressure;
    } else if (minute > 26) {
        // release any extra minutes
        return totalPressure - releasingPressure * (minute - 26);
    } else if (opened.size() == openableLocations.size()) {
        return totalPressure + releasingPressure * (26 - minute); 
    }

    totalPressure += releasingPressure; // release before moving
    
    // note: this is a worst case est, assuming we both sit here for the remaining time
    int max_pressure = totalPressure + releasingPressure * (26 - (minute+1)); 

    if (flowrate[curLoc] != 0 && std::find(opened.begin(), opened.end(), curLoc) == opened.end()) {
        // case: current location has not been opened
        opened.push_back(curLoc);

        int addend = flowrate[curLoc];
        releasingPressure += addend;

        int pressure = evaluate_paths_w_elephant_b(
            map, flowrate, visited, opened, 
            openableLocations, distance_map, curLoc, elephantLoc, 
            totalPressure, releasingPressure, minute+1, elephantMinute);

        if (pressure > max_pressure) {
            max_pressure = pressure;
        }

        releasingPressure -= addend;

        opened.pop_back();
    } else {
        visited.push_back(curLoc);

        // try moving to any openable locations
        for (std::string next : openableLocations) {
            if (std::count(visited.begin(), visited.end(), next) != 0 || std::count(opened.begin(), opened.end(), next) != 0) {
                // no moving to duplicate openable locations
                continue;
            } else {
                // adj in visited
                int addend = releasingPressure * (distance_map[curLoc+next] - 1);
                totalPressure += addend; // release before moving
                
                int pressure = evaluate_paths_w_elephant_b(
                    map, flowrate, visited, opened, 
                    openableLocations, distance_map, next, elephantLoc, 
                    totalPressure, releasingPressure, minute+distance_map[curLoc+next], elephantMinute);

                if (pressure > max_pressure) {
                    max_pressure = pressure;
                }

                totalPressure -= addend;
            }
        }

        visited.pop_back();
    }
    
    return max_pressure;
}

// NOTE: elephants are strings w/ "e" at the end
int evaluate_paths_w_elephant_b(
    std::unordered_map<std::string, std::vector<std::string>>& map,
    std::unordered_map<std::string, int>& flowrate,
    std::vector<std::string>& visited, 
    std::vector<std::string>& opened, 
    std::vector<std::string>& openableLocations,
    std::unordered_map<std::string, int>& distance_map,
    std::string& curLoc, 
    std::string& elephantLoc, 
    int totalPressure, int releasingPressure, 
    int minute, 
    int elephantMinute
) {
    if (elephantMinute == 26) {
        // early exit
        return totalPressure;
    } else if (elephantMinute > 26) {
        // release any extra minutes
        return totalPressure - releasingPressure * (minute - 26);
    } else if (opened.size() == openableLocations.size()) {
        return totalPressure + releasingPressure * (26 - minute); 
    }

    // don't release any pressure !!
    //totalPressure += releasingPressure; // release before moving
    
    // note: this is a worst case est, assuming we both sit here for the remaining time
    int max_pressure = totalPressure + releasingPressure * (26 - (minute+1)); 

    if (flowrate[elephantLoc] != 0 && std::find(opened.begin(), opened.end(), elephantLoc) == opened.end()) {
        // case: current location has not been opened
        opened.push_back(elephantLoc);

        int addend = flowrate[elephantLoc];
        releasingPressure += addend;

        int pressure = evaluate_paths_w_elephant_a(
            map, flowrate, visited, opened, 
            openableLocations, distance_map, curLoc, elephantLoc, 
            totalPressure, releasingPressure, minute, elephantMinute+1);

        if (pressure > max_pressure) {
            max_pressure = pressure;
        }

        releasingPressure -= addend;

        opened.pop_back();
    } else {
        visited.push_back(elephantLoc + "e");

        // try moving to any openable locations
        for (std::string next : openableLocations) {
            if (std::count(visited.begin(), visited.end(), next) != 0 || std::count(opened.begin(), opened.end(), next) != 0) {
                // no moving to duplicate openable locations
                continue;
            } else {
                // adj in visited
                int addend = releasingPressure * (distance_map[elephantLoc+next] - 1);
                totalPressure += addend; // release before moving
                
                int pressure = evaluate_paths_w_elephant_a(
                    map, flowrate, visited, opened, 
                    openableLocations, distance_map, curLoc, next, 
                    totalPressure, releasingPressure, minute, elephantMinute+distance_map[elephantLoc+next]);

                if (pressure > max_pressure) {
                    max_pressure = pressure;
                }

                totalPressure -= addend;
            }
        }

        visited.pop_back();
    }
    
    return max_pressure;
}
*/

/*
// a function to simulate a single order of opening
int evaluate_path_pressure(
    std::vector<std::string>& open_order,
    std::unordered_map<std::string, int>& flowrate
) { }
*/

struct node_t {
    std::vector<std::string> pastTargeted; // e at the end of the string represents elephant opened
    std::string selfLast;
    std::string elephantLast;
    std::string selfTarget;
    std::string elephantTarget;
    int minute;
    int selfDist; // represents distance from target
    int elephantDist;
    int pressure;
    int release;
    bool selfDone;
    bool elephantDone;
};

int evaluate_paths_w_elephant(
    std::unordered_map<std::string, std::vector<std::string>>& map,
    std::unordered_map<std::string, int>& flowrate,
    std::vector<std::string>& openableLocations,
    std::unordered_map<std::string, int>& distance_map,
    std::string& startLoc
) {
    int bestPressure = 0;

    std::vector<node_t> frontier;
    frontier.reserve(10000);

    for (int j = 0; j < openableLocations.size(); j++) {
        for (int k = j+1; k < openableLocations.size(); k++) {
            // assert j != k
            std::vector<std::string> targetedLocations;
            targetedLocations.push_back("AA");
            targetedLocations.push_back(openableLocations[j]);
            targetedLocations.push_back(openableLocations[k]);
            node_t starting = node_t{
                std::move(targetedLocations),
                "AA", "AA",
                openableLocations[j], openableLocations[k],
                1,
                distance_map["AA"+openableLocations[j]], distance_map["AA"+openableLocations[k]],
                0, 0,
                false, false
            };

            frontier.push_back(std::move(starting));
        }
    }

    int smallest_frontier_size = 10000;

    int nodesChecked = 0;
    while (frontier.size() != 0) {
        // load a node
        node_t curr = frontier.back();
        frontier.pop_back();

        // TODO: duplicate state caching

        // evaluate a node until completion
        for (; curr.minute <= 26; curr.minute++) {
            curr.pressure += curr.release;

            if (curr.selfDist == 0 && curr.elephantDist == 0 && curr.selfDone == false && curr.elephantDone == false) {
                // open both valves
                curr.release += flowrate[curr.selfTarget];
                curr.release += flowrate[curr.elephantTarget];

                bool didOne = false;
                for (int j = 0; j < openableLocations.size(); j++) {
                    auto newSelfLoc = openableLocations[j];
                    if (std::count(curr.pastTargeted.begin(), curr.pastTargeted.end(), newSelfLoc) == 0) {
                        for (int k = 0; k < openableLocations.size(); k++) {
                            // assert j != k
                            if (j == k) 
                                continue; 

                            auto newElephantLoc = openableLocations[k];
                            if (std::count(curr.pastTargeted.begin(), curr.pastTargeted.end(), newElephantLoc) == 0) {
                                std::vector<std::string> targeted = curr.pastTargeted;
                                targeted.push_back(newSelfLoc);
                                targeted.push_back(newElephantLoc);
                                node_t starting = node_t{
                                    std::move(targeted),
                                    curr.selfTarget, curr.elephantTarget,
                                    newSelfLoc, newElephantLoc,
                                    curr.minute+1,
                                    distance_map[curr.selfTarget+newSelfLoc], distance_map[curr.elephantTarget+newElephantLoc],
                                    curr.pressure, curr.release,
                                    curr.selfDone, curr.elephantDone
                                };

                                frontier.push_back(std::move(starting));
                                didOne = true;
                            }
                        }
                    }
                }

                // TODO: if not did one, send just one of the people there & not the other

                if (didOne == false) {
                    // NOTE: this is just the same as the move self section  
                    for (auto newSelfLoc : openableLocations) {
                        if (std::count(curr.pastTargeted.begin(), curr.pastTargeted.end(), newSelfLoc) > 0) {
                            continue;
                        }
                        // TODO: it would be more efficient to share a vec instance (if possible?)
                        std::vector<std::string> targeted = curr.pastTargeted; 
                        targeted.push_back(newSelfLoc);
                        node_t starting = node_t{
                            std::move(targeted),
                            curr.selfTarget, curr.elephantLast,
                            newSelfLoc, curr.elephantTarget,
                            curr.minute+1,
                            distance_map[curr.selfTarget+newSelfLoc], curr.elephantDist-1,
                            curr.pressure, curr.release,
                            curr.selfDone, curr.elephantDone
                        };
                        frontier.push_back(std::move(starting));
                        didOne = true;
                    }
                }

                if (didOne == false) {
                    curr.selfDone = true;
                    curr.elephantDone = true;
                    curr.selfDist -= 1;
                    curr.elephantDist -= 1;
                } else {
                    break;
                }
            } else if (curr.selfDist == 0 && curr.selfDone == false) {
                // open valve
                curr.release += flowrate[curr.selfTarget];

                bool didOne = false;
                for (auto newSelfLoc : openableLocations) {
                    if (std::count(curr.pastTargeted.begin(), curr.pastTargeted.end(), newSelfLoc) > 0) {
                        continue;
                    }
                    // TODO: it would be more efficient to share a vec instance (if possible?)
                    std::vector<std::string> targeted = curr.pastTargeted; 
                    targeted.push_back(newSelfLoc);
                    node_t starting = node_t{
                        std::move(targeted),
                        curr.selfTarget, curr.elephantLast,
                        newSelfLoc, curr.elephantTarget,
                        curr.minute+1,
                        distance_map[curr.selfTarget+newSelfLoc], curr.elephantDist-1,
                        curr.pressure, curr.release,
                        curr.selfDone, curr.elephantDone
                    };
                    frontier.push_back(std::move(starting));
                    didOne = true;
                }

                if (didOne == false) {
                    curr.selfDone = true;
                    curr.selfDist -= 1;
                    curr.elephantDist -= 1;
                } else {
                    break;
                }
            
            } else if (curr.elephantDist == 0 && curr.elephantDone == false) {
                // open valve
                curr.release += flowrate[curr.elephantTarget];

                bool didOne = false;
                for (auto newElephantLoc : openableLocations) {
                    if (std::count(curr.pastTargeted.begin(), curr.pastTargeted.end(), newElephantLoc) > 0) {
                        continue;
                    }
                    // TODO: it would be more efficient to share a vec instance if possible
                    std::vector<std::string> targeted = curr.pastTargeted; 
                    targeted.push_back(newElephantLoc);
                    node_t starting = node_t{
                        std::move(targeted),
                        curr.selfLast, curr.elephantTarget,
                        curr.selfTarget, newElephantLoc,
                        curr.minute+1,
                        curr.selfDist-1, distance_map[curr.elephantTarget+newElephantLoc],
                        curr.pressure, curr.release,
                        curr.selfDone, curr.elephantDone
                    };
                    frontier.push_back(std::move(starting));
                    didOne = true;
                }

                if (didOne == false) {
                    curr.elephantDone = true;
                    curr.selfDist -= 1;
                    curr.elephantDist -= 1;
                } else {
                    break;
                }

            } else {
                // TODO: step forwards n steps if selfDist and elephantDist are both bigger than 1 
                // (this will probably have a small impact, but then the branches will succeed more maybe...)
                
                // take steps forwards
                curr.selfDist -= 1;
                curr.elephantDist -= 1;
            }
        }

        if (curr.pressure > bestPressure) {
            bestPressure = curr.pressure;
        }

        nodesChecked += 1;
        if (nodesChecked % 1000000 == 0) {
            std::cout << "checked: " << nodesChecked << std::endl;
            std::cout << "frontier.size: " << frontier.size() << std::endl;
            std::cout << "smallest_size: " << smallest_frontier_size << std::endl;
            smallest_frontier_size = 10000;

            std::cout << "targeted: ";
            for (auto s : curr.pastTargeted) {
                std::cout << s << " ";
            }
            std::cout << "pressure: " << curr.pressure;
            std::cout << " rel: " << curr.release;
            std::cout << " end of min: " << curr.minute << std::endl;
        }

        if (frontier.size() < smallest_frontier_size) {
            smallest_frontier_size = frontier.size();
        }
        
    }

    std::cout << "checked: " << nodesChecked << std::endl;

    return bestPressure;
}

void part1() {
    int part1 = 0;

    // TODO: turn std::string to int for performance (should incur huge improvements i think?)
    std::vector<std::string> openableLocations;
    std::unordered_map<std::string, std::vector<std::string>> map;
    std::unordered_map<std::string, int> flowrate_map;

    // generate graph
    std::string file_name = "input16";
    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        int flowrate;
        char name[5];
        char othervalues[128] = {0};
        sscanf(line.c_str(), "Valve %5s has flow rate=%d; tunnels lead to valves %[, A-Z]", name, &flowrate, othervalues);
        // non plural
        if (othervalues[0] == 0)
            sscanf(line.c_str(), "Valve %5s has flow rate=%d; tunnel leads to valve %[, A-Z]", name, &flowrate, othervalues); 
        
        std::vector<std::string> links = split(std::string(othervalues), std::string(", "));
        std::string name_str = std::string(name);

        //std::cout << "nam: "<< name_str << std::endl;
        //std::cout << "othervalues: "<< std::string(othervalues) << std::endl;

        map[name_str] = std::vector<std::string>();
        flowrate_map[name_str] = flowrate;

        if (flowrate != 0) {
            openableLocations.push_back(name_str);
        }

        for (std::string link : links) {
            map[name_str].push_back(link);
        }
    }

    // optimize graph better by adding a map of distance between openable locations
    std::unordered_map<std::string, int> distance_map;
    {
        for (size_t i = 0; i < openableLocations.size(); i++) {
            std::string curLoc = openableLocations[i];
            for (size_t j = 0; j < openableLocations.size(); j++) {
                if (i == j) continue;
                std::vector<std::string> visited;
                std::string targetLoc = openableLocations[j];
                distance_map[curLoc + targetLoc] = distance(map, visited, curLoc, targetLoc, 0);
            }
        }

        std::string curLoc = "AA";
        for (size_t j = 0; j < openableLocations.size(); j++) {
            std::vector<std::string> visited;
            std::string targetLoc = openableLocations[j];
            distance_map[curLoc + targetLoc] = distance(map, visited, curLoc, targetLoc, 0);
        }
        
        /*
        std::cout << "done dist map\n";
        for (auto x : openableLocations)
            for (auto y : openableLocations) {
                if (x == y) continue;
                std::cout << x << " -> " << y << " : " << distance_map[x + y] << std::endl;
            }

        for (auto y : openableLocations) {
            std::cout << "AA" << " -> " << y << " : " << distance_map["AA" + y] << std::endl;
        }*/
    }

    std::vector<std::string> visited;
    std::vector<std::string> opened;
    std::string start_loc = std::string("AA");
    int pressure = evaluate_paths(map, flowrate_map, visited, opened, openableLocations, distance_map, start_loc, 0, 0, 0);
    part1 = pressure;

    std::cout << "part1: " << part1 << std::endl; 
}

void part2() {
    int part2 = 0;
    
    // TODO: turn std::string to int for performance (should incur huge improvements i think?)
    std::vector<std::string> openableLocations;
    std::unordered_map<std::string, std::vector<std::string>> map;
    std::unordered_map<std::string, int> flowrate_map;

    // generate graph
    std::string file_name = "input16";
    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        int flowrate;
        char name[5];
        char othervalues[128] = {0};
        sscanf(line.c_str(), "Valve %5s has flow rate=%d; tunnels lead to valves %[, A-Z]", name, &flowrate, othervalues);
        // non plural
        if (othervalues[0] == 0)
            sscanf(line.c_str(), "Valve %5s has flow rate=%d; tunnel leads to valve %[, A-Z]", name, &flowrate, othervalues); 
        
        std::vector<std::string> links = split(std::string(othervalues), std::string(", "));
        std::string name_str = std::string(name);

        map[name_str] = std::vector<std::string>();
        flowrate_map[name_str] = flowrate;

        if (flowrate != 0) {
            openableLocations.push_back(name_str);
        }

        for (std::string link : links) {
            map[name_str].push_back(link);
        }
    }

    // optimize graph better by adding a map of distance between openable locations
    std::unordered_map<std::string, int> distance_map;
    {
        for (size_t i = 0; i < openableLocations.size(); i++) {
            std::string curLoc = openableLocations[i];
            for (size_t j = 0; j < openableLocations.size(); j++) {
                if (i == j) continue;
                std::vector<std::string> visited;
                std::string targetLoc = openableLocations[j];
                distance_map[curLoc + targetLoc] = distance(map, visited, curLoc, targetLoc, 0);
            }
        }

        std::string curLoc = "AA";
        for (size_t j = 0; j < openableLocations.size(); j++) {
            std::vector<std::string> visited;
            std::string targetLoc = openableLocations[j];
            distance_map[curLoc + targetLoc] = distance(map, visited, curLoc, targetLoc, 0);
        }
    }

    std::vector<std::string> visited;
    std::vector<std::string> opened;
    std::string start_loc = std::string("AA");
    int pressure = evaluate_paths_w_elephant(
        map, flowrate_map, openableLocations, distance_map, 
        start_loc);
    part2 = pressure;

    std::cout << "part2: " << part2 << std::endl; 
}

// ---------------------------------- //
// runner

int main() {
    part1();
    part2();
}
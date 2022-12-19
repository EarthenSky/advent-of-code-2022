#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>

#include <cmath>

// to run (on wsl):
// g++ -std=c++20 -Wall day14.cpp -o day14 -O2; time ./day14

void panic() {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d\n", EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

struct rect_t {
public:
    int left;
    int top;
    int right;
    int bottom;
};

struct pair_t {
public:
    int x;
    int y;
};

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

// ---------------------------------- //
// parts

rect_t find_bounds(std::string file_name) {
    int left = 300;
    int right = 700;
    int top = 0; // up is negative, down is positive
    int bottom = -10000;

    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        std::vector<std::string> pair_list = split(line, " -> ");
        for (std::string pair : pair_list) {
            std::stringstream ss(pair);
            int x, y;
            char comma;
            if (ss >> x >> comma >> y) {
                if (x < left) left = x;
                if (x > right) right = x;
                if (y < top) top = y;
                if (y > bottom) bottom = y;
            } else {
                panic();
            }        
        }
    }

    return rect_t{left, top, right, bottom};
}

void part1() {
    int part1 = 0;

    pair_t sand_start = pair_t{500, 0};

    std::string file_name = "input14";
    rect_t bounds = find_bounds(file_name);

    std::cout << "bounds:\n";
    std::cout << bounds.left << " " << bounds.right << std::endl;
    std::cout << bounds.top << " " << bounds.bottom << std::endl;

    // fill the map with empty squares
    int width = bounds.right - bounds.left + 1;
    int height = bounds.bottom - bounds.top + 1;
    std::vector<char> map;
    for (int i = 0; i < width * height; i++) {
        map.push_back('.');
    }
    
    // add walls
    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        std::vector<std::string> pair_list = split(line, " -> ");
        bool lastxy_valid = false;
        int lastx, lasty;
        for (std::string pair : pair_list) {
            std::stringstream ss(pair);
            int x, y;
            char comma;
            if (ss >> x >> comma >> y) {
                // get pair
                if (lastxy_valid) {
                    if (x == lastx) {
                        for (int they = std::min(lasty, y); they <= std::max(lasty, y); they++) {
                            map[(they-bounds.top) * width + (x-bounds.left)] = '#'; 
                        }
                    } else if (y == lasty) {
                        for (int thex = std::min(lastx, x); thex <= std::max(x, lastx); thex++) {
                            map[(y-bounds.top) * width + (thex-bounds.left)] = '#';
                        }
                    } else {
                        panic();
                    }
                } else {
                    lastxy_valid = true;
                }

                lastx = x;
                lasty = y;
            } else {
                panic();
            }        
        }
    }

    // simulate grains
    int num_settled = 0;
    bool last_grain_fell = false;
    while (!last_grain_fell) {
        int step = 0;
        pair_t grain_loc = sand_start;
        while (true) {
            if (grain_loc.y == height-1) {
                // fell off bottom
                last_grain_fell = true;
                break;
            } else {
                // check bottom 3 locations and 
                if (map[(grain_loc.y+1-bounds.top) * width + (grain_loc.x-bounds.left)] == '.') {
                    grain_loc.y += 1;
                } else {
                    if (grain_loc.x-bounds.left == 0) { 
                        // fell off left edge
                        last_grain_fell = true;
                        break;
                    } else if (map[(grain_loc.y+1-bounds.top) * width + (grain_loc.x-1-bounds.left)] == '.') {
                        grain_loc.y += 1;
                        grain_loc.x -= 1;
                    } else if (grain_loc.x-bounds.left == width-1) {
                        // fell off right edge
                        last_grain_fell = true;
                        break;
                    } else if (map[(grain_loc.y+1-bounds.top) * width + (grain_loc.x+1-bounds.left)] == '.') {
                        grain_loc.y += 1;
                        grain_loc.x += 1;
                    } else {
                        // stopped
                        map[(grain_loc.y-bounds.top) * width + (grain_loc.x-bounds.left)] = 'o';
                        break;
                    }
                }
            }
        } 
        
        //std::cout << "settling: " << num_settled << std::endl;

        if (!last_grain_fell) 
            num_settled += 1;
    }

    part1 = num_settled;
    
    {
        // add graphic
        map[(sand_start.y-bounds.top) * width + (sand_start.x-bounds.left)] = '+'; 
        std::cout << "map: " << std::endl; 
        for (int i = 0; i < width * height; i++) {
            std::cout << map[i];
            if (i % width == width-1) 
                std::cout << std::endl; 
        }
        std::cout << std::endl; 
    }

    std::cout << "part1: " << part1 << std::endl; 
}

void part2() {
    int part2 = 0;

    pair_t sand_start = pair_t{500, 0};

    std::string file_name = "input14";
    rect_t bounds = find_bounds(file_name);

    std::cout << "bounds:\n";
    std::cout << bounds.left << " " << bounds.right << std::endl;
    std::cout << bounds.top << " " << bounds.bottom << std::endl;

    // fill the map with empty squares
    int width = bounds.right - bounds.left + 1;
    int height = bounds.bottom - bounds.top + 1 + 1;
    std::vector<char> map;
    for (int i = 0; i < width * height; i++) {
        map.push_back('.');
    }
    
    // add walls
    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        std::vector<std::string> pair_list = split(line, " -> ");
        bool lastxy_valid = false;
        int lastx, lasty;
        for (std::string pair : pair_list) {
            std::stringstream ss(pair);
            int x, y;
            char comma;
            if (ss >> x >> comma >> y) {
                // get pair
                if (lastxy_valid) {
                    if (x == lastx) {
                        for (int they = std::min(lasty, y); they <= std::max(lasty, y); they++) {
                            map[(they-bounds.top) * width + (x-bounds.left)] = '#'; 
                        }
                    } else if (y == lasty) {
                        for (int thex = std::min(lastx, x); thex <= std::max(x, lastx); thex++) {
                            map[(y-bounds.top) * width + (thex-bounds.left)] = '#';
                        }
                    } else {
                        panic();
                    }
                } else {
                    lastxy_valid = true;
                }

                lastx = x;
                lasty = y;
            } else {
                panic();
            }        
        }
    }

    // simulate grains
    int num_settled = 0;
    bool last_grain_fell = false;
    while (!last_grain_fell && num_settled < 300000) {
        int step = 0;
        pair_t grain_loc = sand_start;
        while (true) {
            if (grain_loc.y == height-1) {
                // stopped at bottom
                map[(grain_loc.y-bounds.top) * width + (grain_loc.x-bounds.left)] = 'o';
                break;
            } else {
                // check bottom 3 locations and 
                if (map[(grain_loc.y+1-bounds.top) * width + (grain_loc.x-bounds.left)] == '.') {
                    grain_loc.y += 1;
                } else {
                    if (grain_loc.x-bounds.left == 0) { 
                        // fell off left edge
                        panic(); // increase left size
                        break;
                    } else if (map[(grain_loc.y+1-bounds.top) * width + (grain_loc.x-1-bounds.left)] == '.') {
                        grain_loc.y += 1;
                        grain_loc.x -= 1;
                    } else if (grain_loc.x-bounds.left == width-1) {
                        // fell off right edge
                        panic(); // increse right size
                        break;
                    } else if (map[(grain_loc.y+1-bounds.top) * width + (grain_loc.x+1-bounds.left)] == '.') {
                        grain_loc.y += 1;
                        grain_loc.x += 1;
                    } else {
                        // stopped
                        if (grain_loc.y == sand_start.y && grain_loc.x == sand_start.x) {
                            last_grain_fell = true;
                        }
                        map[(grain_loc.y-bounds.top) * width + (grain_loc.x-bounds.left)] = 'o';
                        break;
                    }
                }
            }
        } 

        // NOTE: in this case, the last grain does settle!        
        num_settled += 1;
    }

    part2 = num_settled;
    
    {
        // add graphic
        map[(sand_start.y-bounds.top) * width + (sand_start.x-bounds.left)] = '+'; 
        std::cout << "map: " << std::endl; 
        for (int i = 0; i < width * height; i++) {
            std::cout << map[i];
            if (i % width == width-1) 
                std::cout << std::endl; 
        }
        std::cout << std::endl; 
    }

    std::cout << "part2: " << part2 << std::endl; 
}

// ---------------------------------- //
// runner

int main() {
    part1();
    part2();
}
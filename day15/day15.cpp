#include <stdio.h>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>

// to run (on wsl):
// g++ -std=c++20 -Wall day15.cpp -o day15 -O2; time ./day15

// invalid answers:
// 5099316 (too big)
//
 
void panic() {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d\n", EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

// ---------------------------------- //
// parts

void part1() {
    int part1 = 0;

    std::string file_name = "input15";
    int target_y = 2000000;

    int OFFSET = 10 * 1000 * 1000 + 5 * 1000 * 1000; // 0 -> -OFFSET
    int FURTHEST_DIST = 30 * 1000 * 1000;
    std::vector<int> big_list;
    for (int i = 0; i < FURTHEST_DIST; i++) {
        big_list.push_back(0);
    }

    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        int sx, sy, bx, by;
        sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by);
        if (by == target_y) {
            big_list[bx + OFFSET] = 2; // 2 means contains a beacon!
        } 
        if (by == target_y) {
            big_list[bx + OFFSET] = 2; // 2 means contains a beacon!
        }
        int mdist = std::abs(sx - bx) + std::abs(sy - by);
        int left = sx - mdist;
        int right = sx + mdist;
        for (int curx = left-1; curx <= right+1; curx++) {
            int cur_mdist = std::abs(sx - curx) + std::abs(sy - target_y);
            if (cur_mdist <= mdist) {
                //std::cout << sx << " " << sy << "\t at " << curx << std::endl;
                big_list[curx + OFFSET] = (big_list[curx + OFFSET] == 2) ? 2 : 1;
            }
        }
    }

    for (int i = 0; i < FURTHEST_DIST; i++) {
        if (big_list[i] == 1) { 
            //std::cout << (i - OFFSET) << std::endl; 
            part1++;
        }
    }

    std::cout << "part1: " << part1 << std::endl; 
}

// NOTE: for ranges, x is always smaller
struct pair_t {
public:
    int x;
    int y;
};

const int MAX_COORD = 4 * 1000 * 1000;
//const int MAX_COORD = 20;

bool inside(int p, int left, int right) {
    return p >= left && p <= right;
}

bool inside_or_adj(int p, int left, int right) {
    return p >= left-1 && p <= right+1;
}

// NOTE: we always have that x < y
pair_t unions(pair_t range1, pair_t range2) {
    if (inside_or_adj(range1.x, range2.x, range2.y) && inside_or_adj(range1.y, range2.x, range2.y)) {
        return range2;
    } else if (inside_or_adj(range2.x, range1.x, range1.y) && inside_or_adj(range2.y, range1.x, range1.y)) {
        return range1;
    } else {
        if (inside_or_adj(range1.x, range2.x, range2.y)) {
            return pair_t{range2.x, range1.y};
        } else if (inside_or_adj(range2.x, range1.x, range1.y)) {
            return pair_t{range1.x, range2.y};
        } else {
            return pair_t{0,0};
        }
    }
}

bool look_in_row(int target_y, std::vector<int>& input_values, int* x) {
    std::vector<pair_t> ranges;

    for (int index = 0; index < input_values.size(); index += 4) {
        int sx = input_values[index + 0];
        int sy = input_values[index + 1];
        int bx = input_values[index + 2]; 
        int by = input_values[index + 3];

        int mdist = std::abs(sx - bx) + std::abs(sy - by);
        int vdist = std::abs(sy - target_y);

        // see if we can combine this range with any of the other ranges
        int newoff = (mdist - vdist);
        if (newoff < 0) 
            continue;
        
        pair_t myrange = pair_t{sx - newoff, sx + newoff};
        for (int i = 0; i < ranges.size(); i++) {
            pair_t union_pair = unions(myrange, ranges[i]);
            if (union_pair.x != union_pair.y) {
                myrange = union_pair;
                ranges[i] = ranges[ranges.size()-1];
                ranges.pop_back();
                i -= 1;
            }
        }
        ranges.push_back(myrange);
    }
    
    for (pair_t range : ranges) {
        if (inside(0, range.x, range.y) && inside(MAX_COORD, range.x, range.y)) {
            return false;
        }
    }

    for (int i = 0; i < MAX_COORD; i++) {
        bool found = false;
        for (pair_t range : ranges) {
            if (inside(i, range.x, range.y)) {
                found = true;
            }
        }

        if (!found) {
            *x = i;
            break;
        }
    }
    
    return true;
}

void part2() {
    long long int part2 = 0;
    
    std::string file_name = "input15";
    
    std::vector<int> input_values;
    std::ifstream infile(file_name);
    std::string line;
    while (std::getline(infile, line)) {
        int sx, sy, bx, by;
        sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy, &bx, &by);
        input_values.push_back(sx);
        input_values.push_back(sy);
        input_values.push_back(bx);
        input_values.push_back(by);
    }

    int stored_y;
    for (int y = 0; y < MAX_COORD; y++) {
        int x = 0;
        if (look_in_row(y, input_values, &x)) {
            part2 = x;
            stored_y = y;
            //std::cout << "found! @ " << y << std::endl;
            break;
        }
    }

    part2 *= MAX_COORD;
    part2 += stored_y;
    std::cout << "part2: " << part2 << std::endl; 
}

// ---------------------------------- //
// runner

int main() {
    part1();
    part2();
}
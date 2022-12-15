#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <stdbool.h>
#include <string.h>

// to run (on wsl):
// gcc -std=c11 day12.c -o day12 -O2; time ./day12

void panic() {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d\n", EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

int SELF = -14;
int END_SPACE = -28;

struct node_t {
    int index;
    int from_index;
    int cost;
};

void frontier_add(struct node_t* frontier, int* frontier_len, struct node_t to_add) {
    frontier[*frontier_len] = to_add;
    *frontier_len += 1;
}

struct node_t min_frontier_pop(struct node_t* frontier, int* frontier_len) {
    int len = *frontier_len;
    int min_cost = INT_MAX;
    
    // pq could do this log(n) time
    struct node_t min_node;
    int min_i;
    for (int i = 0; i < len; i++) {
        if (frontier[i].cost < min_cost) {
            min_node = frontier[i];
            min_cost = frontier[i].cost;
            min_i = i;
        }
    }

    // overwrite our node & reduce length of array
    frontier[min_i] = frontier[len-1];
    *frontier_len -= 1;

    return min_node;
}

void adjacent_nodes(int* matrix, int width, int height, struct node_t cur_node, struct node_t* out, int* out_len) {
    int x = cur_node.index % width;
    int y = cur_node.index / width;
    int cur_val = matrix[cur_node.index];
    
    if (cur_val == SELF)
        cur_val = 0;

    int out_i = 0;

    if (x > 0) {
        int left_i = y*width + x - 1;
        int left = matrix[left_i];
        if ((left <= cur_val+1 && left != SELF)) {
            out[out_i] = (struct node_t){ left_i, cur_node.index, cur_node.cost + 1 };
            out_i += 1;
        }
    }
    
    if (x < width-1) {
        int right_i = y*width + x + 1;
        int right = matrix[right_i];
        if ((right <= cur_val+1 && right != SELF)) {
            out[out_i] = (struct node_t){ right_i, cur_node.index, cur_node.cost + 1 };
            out_i += 1;
        }
    }

    if (y < height-1) {
        int up_i = (y+1)*width + x;
        int up = matrix[up_i];
        if ((up <= cur_val+1 && up != SELF)) {
            out[out_i] = (struct node_t){ up_i, cur_node.index, cur_node.cost + 1 };
            out_i += 1;
        }
    }

    if (y > 0) {
        int down_i = (y-1)*width + x;
        int down = matrix[down_i];
        if ((down <= cur_val+1 && down != SELF)) {
            out[out_i] = (struct node_t){ down_i, cur_node.index, cur_node.cost + 1 };
            out_i += 1;
        }
    }

    *out_len = out_i;

}

void part1() {
    size_t width, height;

    size_t self_i, end_i;

    // NOTE: i put an extra space at the end of the input files so that the reading works properly
    FILE* fp = fopen("input12", "r");
    if (fp == NULL) panic();
    
    size_t matsize = 0;
    int matrix[9200];

    char linebuf0[1024]; // is a null term string
    for(;;) {
        char* succ = fgets(linebuf0, 1024, fp);
        if (!succ) break;
        
        width = strlen(linebuf0)-1; //.ignore \n at end, or empty space

        for (int i = matsize; i < matsize+width; i++) {
            if (succ[i-matsize] == 'S') {
                self_i = i;
                matrix[i] = succ[i - matsize] - 'a';
            } else if (succ[i-matsize] == 'E') {
                end_i = i;
                matrix[i] = 'z' - 'a';
            } else
                matrix[i] = succ[i - matsize] - 'a'; // 'a' shuld be 0 now
        }
        matsize += width;
    }

    height = matsize / width;

    // this stores the minimum cost node to get here
    struct node_t min_visited_matrix[9200];
    for (int i = 0; i < matsize; i++)
        min_visited_matrix[i] = (struct node_t){ i, -1, INT_MAX };

    // this stores nodes we wish to evaluate soon
    int frontier_len = 0;
    struct node_t frontier[9200];
    frontier_add(frontier, &frontier_len, (struct node_t){ self_i, -1, 0 });
    
    int path_len = -1;

    // this is the body of BFS
    int adj_nodes_len;
    struct node_t adj_nodes[4];
    while (frontier_len > 0) {
        struct node_t cur_node = min_frontier_pop(frontier, &frontier_len);
        //printf("cost: %d, index: %d, frontier_len: %d\n", cur_node.cost, cur_node.index, frontier_len);
        if (cur_node.cost < min_visited_matrix[cur_node.index].cost) {
            min_visited_matrix[cur_node.index] = cur_node;
        } else {
            // we can prune this subtree because it is suboptimal compared to the node that's already there
            continue; 
        }
        
        if (cur_node.index == end_i) {
            printf("done!\n");
            path_len = cur_node.cost;
            break;
        } else {
            adj_nodes_len = 0;
            adjacent_nodes(matrix, width, height, cur_node, adj_nodes, &adj_nodes_len);

            //printf("num adj: %d\n", adj_nodes_len);

            // add adjacent nodes to the frontier
            for (int i = 0; i < adj_nodes_len; i++) {
                frontier_add(frontier, &frontier_len, adj_nodes[i]);
            }
        }
    }

    // NOTE: we could reconstruct the path using our min_visited_matrix if we wanted

    printf("part1: %d\n", path_len);
}

void part2() {
    size_t width, height;

    size_t self_i, end_i;

    // NOTE: i put an extra space at the end of the input files so that the reading works properly
    FILE* fp = fopen("input12", "r");
    if (fp == NULL) panic();
    
    size_t matsize = 0;
    int matrix[9200];

    char linebuf0[1024]; // is a null term string
    for(;;) {
        char* succ = fgets(linebuf0, 1024, fp);
        if (!succ) break;
        
        width = strlen(linebuf0)-1; //.ignore \n at end, or empty space

        for (int i = matsize; i < matsize+width; i++) {
            if (succ[i-matsize] == 'S') {
                self_i = i;
                matrix[i] = succ[i - matsize] - 'a';
            } else if (succ[i-matsize] == 'E') {
                end_i = i;
                matrix[i] = 'z' - 'a';
            } else
                matrix[i] = succ[i - matsize] - 'a'; // 'a' shuld be 0 now
        }
        matsize += width;
    }

    height = matsize / width;

    // this stores the minimum cost node to get here
    struct node_t min_visited_matrix[9200];
    for (int i = 0; i < matsize; i++)
        min_visited_matrix[i] = (struct node_t){ i, -1, INT_MAX };

    // this stores nodes we wish to evaluate soon
    int frontier_len = 0;
    struct node_t frontier[9200];
    frontier_add(frontier, &frontier_len, (struct node_t){ self_i, -1, 0 });
    
    for (int i = 0; i < matsize; i++) {
        if (matrix[i] == 0) {
            frontier_add(frontier, &frontier_len, (struct node_t){ i, -1, 0 });
        }
    }

    int path_len = -1;

    // this is the body of BFS
    int adj_nodes_len;
    struct node_t adj_nodes[4];
    while (frontier_len > 0) {
        struct node_t cur_node = min_frontier_pop(frontier, &frontier_len);
        //printf("cost: %d, index: %d, frontier_len: %d\n", cur_node.cost, cur_node.index, frontier_len);
        if (cur_node.cost < min_visited_matrix[cur_node.index].cost) {
            min_visited_matrix[cur_node.index] = cur_node;
        } else {
            // we can prune this subtree because it is suboptimal compared to the node that's already there
            continue; 
        }
        
        if (cur_node.index == end_i) {
            printf("done!\n");
            path_len = cur_node.cost;
            break;
        } else {
            adj_nodes_len = 0;
            adjacent_nodes(matrix, width, height, cur_node, adj_nodes, &adj_nodes_len);

            //printf("num adj: %d\n", adj_nodes_len);

            // add adjacent nodes to the frontier
            for (int i = 0; i < adj_nodes_len; i++) {
                frontier_add(frontier, &frontier_len, adj_nodes[i]);
            }
        }
    }

    printf("part2: %d\n", path_len);
}

int main() {
    part1();
    part2();

    return 0;
}
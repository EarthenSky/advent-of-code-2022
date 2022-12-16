#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <stdbool.h>
#include <string.h>

// to run (on wsl):
// gcc -std=c11 day13.c -o day13 -O2; time ./day13

// ---------------------------------- //
// globals here


// ---------------------------------- //
// functions here

void panic() {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d\n", EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

// WARNING: this assumes that s1 won't overflow it's buffer (& that tmp has a buffer of 256)
// inserts s2 into s1 at loc
void strinsert(char* s1, char* s2, int loc) {
    char tmp[256];
    strncpy(tmp, s1, loc);
    tmp[loc] = '\0';
    strcat(tmp, s2);
    strcat(tmp, s1+loc);
    strcpy(s1, tmp);
}

// 3 rules: smaller int, smaller list, upgrade int to list of that int.
bool compare_packets(const char* p1_in, const char* p2_in) {
    int STATUS_LIST = 0;
    int STATUS_CLOSE_LIST = 1;
    int STATUS_NUMBER = 2;
    
    int left_size = 0, right_size = 0;
    int left_stack[512], right_stack[512];
    
    int left_status, right_status;

    // make mutable copies of p1 and p2
    char p1[512], p2[512];
    strcpy(p1, p1_in);
    strcpy(p2, p2_in);

    int len1 = strlen(p1), len2 = strlen(p2);
    int li = 0, ri = 0;
    while (li < len1 && ri < len2) {
        char c1 = p1[li], c2 = p2[ri];

        //printf("\tli %d, ri %d\n", li, ri);

        int left_chars_consumed, left_num_read;
        if (c1 == '[' || (c1 == ',' && p1[li+1] == '[')) {
            left_stack[left_size] = c1;
            left_size += 1;
            if (c1 == ',') li += 1;
            left_status = STATUS_LIST;
        } else if (c1 == ']') {
            // pop from stack until we hit a left bracket
            while (left_size != 0 && left_stack[left_size-1] != '[') {
                left_size -= 1;
            }
            left_size -= 1; // NOTE: this would fail if we had two lists on the bottom level [][]
            left_status = STATUS_CLOSE_LIST;
        } else {
            int num_vars_read = sscanf(&p1[li], ",%d%n", &left_num_read, &left_chars_consumed);
            if (num_vars_read == 0) { 
                sscanf(&p1[li], "%d%n", &left_num_read, &left_chars_consumed);
            }
            li += left_chars_consumed-1;
            left_stack[left_size] = -left_num_read; // stored as -ve to avoid collision with chars
            left_size += 1;
            left_status = STATUS_NUMBER;
        }

        int right_chars_consumed, right_num_read;
        if (c2 == '[' || (c2 == ',' && p2[ri+1] == '[')) {
            right_stack[right_size] = c2;
            right_size += 1;
            if (c2 == ',') ri += 1;
            right_status = STATUS_LIST;
        } else if (c2 == ']') {
            // pop from stack until we hit a left bracket
            while (right_size != 0 && right_stack[right_size-1] != '[') {
                right_size -= 1;
            }
            right_size -= 1; // NOTE: this would fail if we had two lists on the bottom level [][]
            right_status = STATUS_CLOSE_LIST;
        } else {
            int num_vars_read = sscanf(&p2[ri], ",%d%n", &right_num_read, &right_chars_consumed);
            if (num_vars_read == 0) { 
                sscanf(&p2[ri], "%d%n", &right_num_read, &right_chars_consumed);
            }
            ri += right_chars_consumed-1;
            right_stack[right_size] = -right_num_read; // stored as -ve to avoid collision with chars
            right_size += 1;
            right_status = STATUS_NUMBER;
        }

        if (left_status == STATUS_NUMBER && right_status == STATUS_NUMBER) {
            if (-left_stack[left_size-1] > -right_stack[right_size-1]) {
                //printf("false: %d, %d\n", left_stack[left_size-1], right_stack[right_size-1]);
                return false;
            } else if (-left_stack[left_size-1] < -right_stack[right_size-1]) {
                //printf("true: %d, %d\n", left_stack[left_size-1], right_stack[right_size-1]);
                return true;
            } else {
                // pass
            }
        } else if (left_status == STATUS_LIST && right_status == STATUS_LIST) {
            // pass
            //printf("list list\n");
        } else if (left_status == STATUS_LIST && right_status == STATUS_NUMBER) {
            //printf("right transform\n");
            // turn the number into a list!
            right_stack[right_size-1] = '[';
            strinsert(p2, "]", ri+1);
            //printf("after insert p1: %s", p1);
            //printf("after insert p2: %s", p2);
            ri -= right_chars_consumed;
        } else if (left_status == STATUS_NUMBER && right_status == STATUS_LIST) {
            //printf("left transform\n");
            // turn the number into a list!
            left_stack[left_size-1] = '[';
            strinsert(p1, "]", li+1);
            //printf("after insert p1: %s", p1);
            //printf("after insert p2: %s", p2);
            //printf("t chars consumed: %d\n", left_chars_consumed);
            //printf("li: %d\n", li);
            li -= left_chars_consumed;
        } else if (left_status == STATUS_NUMBER && right_status == STATUS_CLOSE_LIST) {
            //printf("number vs list close\n");
            return false;
        } else if (left_status == STATUS_LIST && right_status == STATUS_CLOSE_LIST) {
            //printf("list vs list close\n");
            return false;
        } else if (left_status == STATUS_CLOSE_LIST && right_status == STATUS_NUMBER) {
            //printf("list close vs number\n");
            return true;
        } else if (left_status == STATUS_CLOSE_LIST && right_status == STATUS_LIST) {
            //printf("list close vs list\n");
            return true;
        } else if (left_status == STATUS_CLOSE_LIST && right_status == STATUS_CLOSE_LIST) {
            // pass
            //printf("close close\n");
        }
        
        li++;
        ri++;
    }

    return true;
}

void sort(char* line_list, int line_len, int list_len) {
    char tmp[512];

    // this is an O(1) memory n^2 sort
    for (int i = 1; i < list_len; i++) {
        for (int j = i; j > 0; j--) {
            char* cur_line = line_list + line_len*j;
            char* bef_line = line_list + line_len*(j-1);
            if (compare_packets(cur_line, bef_line)) {
                // do a swap
                strcpy(tmp, bef_line);
                strcpy(bef_line, cur_line);
                strcpy(cur_line, tmp);
            }
        }
    }
}

// ---------------------------------- //
// parts

void part1() {
    int score = 0;

    // NOTE: i put an extra space at the end of the input files so that the reading works properly
    FILE* fp = fopen("input13", "r");
    if (fp == NULL) panic();
    
    // data structs here

    int index = 1;
    char linebuf0[512]; // is a null term string
    char linebuf1[512];
    for(;;) {
        fgets(linebuf0, 512, fp);
        fgets(linebuf1, 512, fp);

        //printf("s1: %s", linebuf0);
        //printf("s2: %s", linebuf1);

        score += compare_packets(linebuf0, linebuf1) ? index : 0;
        
        //printf("i %d| score: %d\n", index, score);

        // grab empty line if it exists, or exit
        char* succ = fgets(linebuf0, 512, fp);
        if (!succ) break;
        
        index += 1;
    }

    printf("part1: %d\n", score);
}

void part2() {
    int score = 1;

    // NOTE: i put an extra space at the end of the input files so that the reading works properly
    FILE* fp = fopen("input13", "r");
    if (fp == NULL) panic();
    
    // data structs here
    char linebuf_list[512*512]; // is a null term string

    int index = 1;
    char linebuf0[512]; // tmp storage
    for(;;) {
        fgets(linebuf_list + index*512, 512, fp);
        index += 1;
        fgets(linebuf_list + index*512, 512, fp);
        index += 1;

        // grab empty line if it exists, or exit
        char* succ = fgets(linebuf0, 512, fp);
        if (!succ) break;
    }

    strcpy(linebuf_list + index*512, "[[2]]\n");
    index += 1;

    strcpy(linebuf_list + index*512, "[[6]]\n");
    index += 1;

    sort(linebuf_list, 512, index);

    for (int i = 0; i < index; i++) {
        char* s = linebuf_list + 512*i;
        if (strcmp(s, "[[2]]\n") == 0 || strcmp(s, "[[6]]\n") == 0) {
            score *= i+1;
        }
    }

    /*for (int i = 0; i < index; i++) {
        printf("%s", linebuf_list + 512*i);
    }*/

    printf("part2: %d\n", score);
}


// ---------------------------------- //
// runner

int main() {
    part1();
    part2();

    return 0;
}
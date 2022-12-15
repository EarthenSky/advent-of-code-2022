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
bool compare_packets(char* p1, char* p2) {
    int STATUS_LIST = 0;
    int STATUS_CLOSE_LIST = 1;
    int STATUS_NUMBER = 2;
    
    int left_size = 0, right_size = 0;
    int left_stack[512], right_stack[512];
    
    int left_status, right_status;

    int len1 = strlen(p1), len2 = strlen(p2);
    int li = 0, ri = 0;
    while (li < len1 && ri < len2) {
        char c1 = p1[li], c2 = p2[ri];

        //printf("\tli %d, ri %d\n", li, ri);
        //printf("\tli %d\n", li);
        //printf("\tri %d\n", ri);
        //printf("\td\n");

        int left_chars_consumed, left_num_read;
        if (c1 == '[' || (c1 == ',' && p1[li+1] == '[')) {
            left_stack[left_size] = c1;
            left_size += 1;
            if (c1 == ',') li += 1;
            left_status = STATUS_LIST;
        } else if (c1 == ']') {
            //printf("\tclose list -> li: %d\n", li);
            // pop from stack until we hit a left bracket
            while (left_size != 0 || left_stack[left_size-1] != '[') {
                left_size -= 1;
            }
            left_size -= 1; // NOTE: this would fail if we had two lists on the bottom level [][]
            left_status = STATUS_CLOSE_LIST;
        } else {
            int num_vars_read = sscanf(&p1[li], ",%d%n", &left_num_read, &left_chars_consumed);
            if (num_vars_read == 0) { 
                sscanf(&p1[li], "%d%n", &left_num_read, &left_chars_consumed);
            }
            //printf("\tleft chars consumed: %d\n", left_chars_consumed);
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
            while (right_size != 0 || right_stack[right_size-1] != '[') {
                right_size -= 1;
            }
            right_size -= 1; // NOTE: this would fail if we had two lists on the bottom level [][]
            right_status = STATUS_CLOSE_LIST;
        } else {
            int num_vars_read = sscanf(&p2[ri], ",%d%n", &right_num_read, &right_chars_consumed);
            if (num_vars_read == 0) { 
                sscanf(&p2[ri], "%d%n", &right_num_read, &right_chars_consumed);
            }
            //printf("\tright chars consumed: %d\n", right_chars_consumed);
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
                //printf("tie: %d vs %d\n", left_stack[left_size-1], right_stack[right_size-1]);
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

    printf("part2: %d\n", 0);
}

// ---------------------------------- //
// runner

int main() {
    part1();
    part2();

    return 0;
}
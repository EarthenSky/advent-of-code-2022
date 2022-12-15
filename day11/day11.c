#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>

// to run: 
// gcc -std=c11 day11.c -o day11 -O2; time ./day11

void panic() {
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAA %d\n", EXIT_FAILURE);
    exit(EXIT_FAILURE);
}

struct monkey_t {
    int items_top;
    int items[100];

    char operation;
    int right_op_int;

    int test;
    int iftrue;
    int iffalse;

    int num_inspected;
};

struct moddable_number_t {
    int modby[25];
};

struct moddable_number_t from_number(int x) {
    struct moddable_number_t mn;
    for (int i = 1; i < 25; i++) {
        mn.modby[i] = x % i;
    }
    return mn;
}

struct moddable_number_t add(struct moddable_number_t mn1, int x) {
    struct moddable_number_t mn;
    for (int i = 1; i < 25; i++) {
        mn.modby[i] = (mn1.modby[i] + x) % i;
    }
    return mn;
}

struct moddable_number_t mult(struct moddable_number_t mn1, int x) {
    struct moddable_number_t mn;
    for (int i = 1; i < 25; i++) {
        mn.modby[i] = (mn1.modby[i] * x) % i;
    }
    return mn;
}

struct moddable_number_t mult2(struct moddable_number_t mn1, struct moddable_number_t mn2) {
    struct moddable_number_t mn;
    for (int i = 1; i < 25; i++) {
        mn.modby[i] = (mn1.modby[i] * mn2.modby[i]) % i;
    }
    return mn;
}

bool divisibleby(struct moddable_number_t mn1, int x) {
    return mn1.modby[x] == 0;
}


struct big_monkey_t {
    int items_top;
    struct moddable_number_t items[100];

    char operation;
    int right_op_int;

    int test;
    int iftrue;
    int iffalse;

    unsigned long long int num_inspected;
};

void part1() {
    int LINE1_HEAD = strlen("  Starting items: ");

    FILE* fp = fopen("input11", "r");
    if (fp == NULL) panic();

    size_t num_monkeys = 0;
    struct monkey_t monkey_list[20];

    char linebuf0[1024]; // is a null term string
    char linebuf1[1024];
    char linebuf2[1024];
    char linebuf3[1024];
    char linebuf4[1024]; 
    char linebuf5[1024]; 
    for(;;) {
        char* succ = fgets(linebuf0, 1024, fp);
        if (!succ) break;
        fgets(linebuf1, 1024, fp);
        fgets(linebuf2, 1024, fp);
        fgets(linebuf3, 1024, fp);
        fgets(linebuf4, 1024, fp);
        fgets(linebuf5, 1024, fp);

        monkey_list[num_monkeys] = (struct monkey_t){ 0,{[0] = 0},0,0,0,0,0,0 };

        // read the list on line 2
        char* token;
        token = strtok(&linebuf1[LINE1_HEAD], ", ");
        while (token != NULL && strcmp(token, "\n")) {
            int index = monkey_list[num_monkeys].items_top;
            
            sscanf(token, "%d", &monkey_list[num_monkeys].items[index]);
            monkey_list[num_monkeys].items_top++;
            token = strtok(NULL, ", ");
        }

        char operation[128], right_op[128];
        sscanf(linebuf2, "  Operation: new = old %s %s\n", operation, right_op);
        
        monkey_list[num_monkeys].operation = operation[0];
        monkey_list[num_monkeys].right_op_int = atoi(right_op);

        sscanf(linebuf3, "  Test: divisible by %d", &monkey_list[num_monkeys].test);
        sscanf(linebuf4, "    If true: throw to monkey %d", &monkey_list[num_monkeys].iftrue);
        sscanf(linebuf5, "    If false: throw to monkey %d", &monkey_list[num_monkeys].iffalse);
        
        // test validity
        //printf("%c %d ", monkey_list[num_monkeys].operation, monkey_list[num_monkeys].right_op_int);
        //printf("%d %d %d\n", monkey_list[num_monkeys].test, monkey_list[num_monkeys].iftrue, monkey_list[num_monkeys].iffalse);

        num_monkeys++;

        fgets(linebuf0, 1024, fp); // parse empty line
    }

    for (int round = 0; round < 20; round++) {
        for (int turn = 0; turn < num_monkeys; turn++) {
            int num_items = monkey_list[turn].items_top;
            for (int item = 0; item < num_items; item++) {
                // inspect
                monkey_list[turn].num_inspected += 1;

                // operation
                if (monkey_list[turn].operation == '*') {
                    if (monkey_list[turn].right_op_int == 0) {
                        monkey_list[turn].items[item] *= monkey_list[turn].items[item];
                    } else {
                        monkey_list[turn].items[item] *= monkey_list[turn].right_op_int;
                    }
                } else {
                    if (monkey_list[turn].right_op_int == 0) {
                        monkey_list[turn].items[item] += monkey_list[turn].items[item];
                    } else {
                        monkey_list[turn].items[item] += monkey_list[turn].right_op_int;
                    }
                }

                // worry div
                monkey_list[turn].items[item] /= 3;
                
                // test
                int worry_amount = monkey_list[turn].items[item];

                if (worry_amount % monkey_list[turn].test == 0) {
                    monkey_list[monkey_list[turn].iftrue].items[monkey_list[monkey_list[turn].iftrue].items_top] = worry_amount;
                    monkey_list[monkey_list[turn].iftrue].items_top += 1;
                } else {
                    monkey_list[monkey_list[turn].iffalse].items[monkey_list[monkey_list[turn].iffalse].items_top] = worry_amount;
                    monkey_list[monkey_list[turn].iffalse].items_top += 1;
                }
            }

            // we empty our stack each iteration
            monkey_list[turn].items_top = 0;

            //printf("%d -> num inspected: %d\n", turn, monkey_list[turn].num_inspected);
        } 
    }

    int max1 = 0;
    int max2 = 0;
    for (int turn = 0; turn < num_monkeys; turn++) {
        int num = monkey_list[turn].num_inspected;
        if (num > max2 && num > max1) {
            max2 = max1;
            max1 = num;
        } else if (num > max2) {
            max2 = num;
        }
    }

    printf("part1: %d\n", max1 * max2);
}

void part2() {
    int LINE1_HEAD = strlen("  Starting items: ");

    FILE* fp = fopen("input11", "r");
    if (fp == NULL) panic();

    size_t num_monkeys = 0;
    struct big_monkey_t monkey_list[20];

    char linebuf0[1024]; // is a null term string
    char linebuf1[1024];
    char linebuf2[1024];
    char linebuf3[1024];
    char linebuf4[1024]; 
    char linebuf5[1024]; 
    for(;;) {
        char* succ = fgets(linebuf0, 1024, fp);
        if (!succ) break;
        fgets(linebuf1, 1024, fp);
        fgets(linebuf2, 1024, fp);
        fgets(linebuf3, 1024, fp);
        fgets(linebuf4, 1024, fp);
        fgets(linebuf5, 1024, fp);

        monkey_list[num_monkeys] = (struct big_monkey_t){ 0,{[0] = 0},0,0,0,0,0,0 };

        // read the list on line 2
        char* token;
        token = strtok(&linebuf1[LINE1_HEAD], ", ");
        while (token != NULL && strcmp(token, "\n")) {
            int index = monkey_list[num_monkeys].items_top;
            
            int tmp;
            sscanf(token, "%d", &tmp);
            monkey_list[num_monkeys].items[index] = from_number(tmp);
            monkey_list[num_monkeys].items_top++;
            token = strtok(NULL, ", ");
        }

        char operation[128], right_op[128];
        sscanf(linebuf2, "  Operation: new = old %s %s\n", operation, right_op);
        
        monkey_list[num_monkeys].operation = operation[0];
        monkey_list[num_monkeys].right_op_int = atoi(right_op);

        sscanf(linebuf3, "  Test: divisible by %d", &monkey_list[num_monkeys].test);
        sscanf(linebuf4, "    If true: throw to monkey %d", &monkey_list[num_monkeys].iftrue);
        sscanf(linebuf5, "    If false: throw to monkey %d", &monkey_list[num_monkeys].iffalse);

        num_monkeys++;

        fgets(linebuf0, 1024, fp); // parse empty line
    }

    for (int round = 0; round < 10000; round++) {
        for (int turn = 0; turn < num_monkeys; turn++) {
            int num_items = monkey_list[turn].items_top;
            for (int item = 0; item < num_items; item++) {
                // inspect
                monkey_list[turn].num_inspected += 1;

                // operation
                if (monkey_list[turn].operation == '*') {
                    if (monkey_list[turn].right_op_int == 0) {
                        //printf("mult2\n");
                        monkey_list[turn].items[item] = mult2(monkey_list[turn].items[item], monkey_list[turn].items[item]);
                    } else {
                        //printf("mult\n");
                        monkey_list[turn].items[item] = mult(monkey_list[turn].items[item], monkey_list[turn].right_op_int);
                    }
                } else {
                    if (monkey_list[turn].right_op_int == 0) {
                        panic();
                    } else {
                        //printf("add\n");
                        monkey_list[turn].items[item] = add(monkey_list[turn].items[item], monkey_list[turn].right_op_int);
                    }
                }

                // worry div
                //monkey_list[turn].items[item] /= 3;

                // test
                struct moddable_number_t worry_amount = monkey_list[turn].items[item];
                if (divisibleby(worry_amount, monkey_list[turn].test)) {
                    //printf("true\n");
                    monkey_list[monkey_list[turn].iftrue].items[monkey_list[monkey_list[turn].iftrue].items_top] = worry_amount;
                    monkey_list[monkey_list[turn].iftrue].items_top += 1;
                } else {
                    //printf("false\n");
                    monkey_list[monkey_list[turn].iffalse].items[monkey_list[monkey_list[turn].iffalse].items_top] = worry_amount;
                    monkey_list[monkey_list[turn].iffalse].items_top += 1;
                }
            }

            // we empty our stack each iteration
            monkey_list[turn].items_top = 0;

            if (round % 1000 == 0 || round == 20)
                printf("%d -> num inspected: %d\n", turn, monkey_list[turn].num_inspected);
        } 
    }

    unsigned long long int max1 = 0;
    unsigned long long int max2 = 0;
    for (int turn = 0; turn < num_monkeys; turn++) {
        unsigned long long int num = monkey_list[turn].num_inspected;
        if (num > max2 && num > max1) {
            max2 = max1;
            max1 = num;
        } else if (num > max2) {
            max2 = num;
        }
    }

    printf("part2: %llu\n", max1 * max2);
}

int main() {
    part1();
    part2();

    return 0;
}
#import itertools

# -------------------------
# part 1:

def check_cycle(current_cycle):
    if current_cycle == 20 or current_cycle == 60 or current_cycle == 100 or \
       current_cycle == 140 or current_cycle == 180 or current_cycle == 220:
        return X_reg * current_cycle
    else: 
        return 0

score = 0
with open('input10') as f:
    current_cycle = 0
    X_reg = 1
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "").split(" ")
        if line[0] == "addx":
            current_cycle += 1
            score += check_cycle(current_cycle)
            
            current_cycle += 1
            score += check_cycle(current_cycle)
            X_reg += int(line[1])

        elif line[0] == "noop":
            current_cycle += 1
            score += check_cycle(current_cycle)
            
# -------------------------
# part 2:

score2 = 0
with open('input10') as f:
    HEIGHT, WIDTH = 6, 40
    screen = ""           #("." * WIDTH + "\n") * HEIGHT
    sprite_size = 3

    current_cycle = 0
    X_reg = 1             # sprite position
    lines = f.readlines() # any subsequent calls to readlines will return []
    
    for line in lines:
        line = line.replace("\n", "").split(" ")
        if line[0] == "addx":
            current_cycle += 1
            x = current_cycle % WIDTH
            if X_reg == x or X_reg+2 == x or X_reg+1 == x:
                screen += "#"
            else:
                screen += "."
            
            if x == 0:
                screen += "\n"
                
            current_cycle += 1
            x = current_cycle % WIDTH
            if X_reg == x or X_reg+2 == x or X_reg+1 == x:
                screen += "#"
            else:
                screen += "."
            
            if x == 0:
                screen += "\n"
            X_reg += int(line[1])

        elif line[0] == "noop":
            current_cycle += 1
            x = current_cycle % WIDTH
            if X_reg == x or X_reg+2 == x or X_reg+1 == x:
                screen += "#"
            else:
                screen += "."

            if x == 0:
                screen += "\n"

    score2 = "\n" + screen

# -------------------------
# output:

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

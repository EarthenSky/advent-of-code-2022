# to run: 
# python day2.py
# pypy day2.py

score = 0

def as_abc(str):
    if str == "X":
        return "A"
    elif str == "Y":
        return "B"
    else:
        return "C"

def beats(str):
    if str == "A":
        return 2
    elif str == "B":
        return 3
    else:
        return 1

def loses(str):
    if str == "A":
        return 3
    elif str == "B":
        return 1
    else:
        return 2

def ties(str):
    if str == "A":
        return 1
    elif str == "B":
        return 2
    else:
        return 3

with open('input2') as f:
    for line in f.readlines():
        pair = line.split(" ")
        l = pair[0].strip()
        r = as_abc(pair[1].strip())
        
        if r == "A":
            score_mod = 1
        elif r == "B":
            score_mod = 2
        elif r == "C":
            score_mod = 3

        if l == r:
            score += score_mod + 3
        elif l == "A" and r == "B":
            score += score_mod + 6
        elif l == "A" and r == "C":
            score += score_mod + 0
        elif l == "B" and r == "A":
            score += score_mod + 0
        elif l == "B" and r == "C":
            score += score_mod + 6
        elif l == "C" and r == "A":
            score += score_mod + 6
        elif l == "C" and r == "B":
            score += score_mod + 0
        else:
            print("error")

score2 = 0
with open('input2') as f:
    for line in f.readlines():
        pair = line.split(" ")
        l = pair[0].strip()
        r = pair[1].strip()
        
        if r == "X":
            score2 += loses(l) + 0
        elif r == "Y":
            score2 += ties(l) + 3
        else:
            score2 += beats(l) + 6


# set these to the vars:

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

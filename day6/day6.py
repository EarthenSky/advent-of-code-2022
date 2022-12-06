#import itertools

# to run: 
# python day6.py
# pypy day6.py

score = 0

with open('input6') as f:
    lines = f.readlines() # any subsequent calls to readlines will return []
    line = lines[0].replace("\n", "")
    for i in range(0, len(line)):
        exists = []
        for j in range(0, 4):
            if line[i+j] in exists:
                break
            else: 
                exists += [line[i+j]]
        
        if len(exists) == 4:
            score = i + 4
            break

# -------------------------
# part 2:

score2 = 0

with open('input6') as f:
    lines = f.readlines() # any subsequent calls to readlines will return []
    line = lines[0].replace("\n", "")
    for i in range(0, len(line)):
        exists = []
        for j in range(0, 14):
            if line[i+j] in exists:
                break
            else: 
                exists += [line[i+j]]
        
        if len(exists) == 14:
            score2 = i + 14
            break

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

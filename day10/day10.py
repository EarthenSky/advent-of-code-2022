#import itertools

# -------------------------
# part 1:

score = 0
with open('input10') as f:
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "")
       

# -------------------------
# part 2:

score2 = 0
with open('input10') as f:
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "")
            
# -------------------------
# output:

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

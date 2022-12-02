# to run: 
# python day1.py
# pypy day1.py

everything = []
themax = 0

with open('input1') as f:
    current = 0
    for line in f.readlines():
        if line == "\n":
            everything += [current]
            current = 0
        else:
            current += int(line)

print("part1: {}\t".format(sorted(everything)[-1]))
print("part2: {}\t".format(sum(sorted(everything)[-3:])))

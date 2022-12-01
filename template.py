# to run: 
# python template.py
# pypy template.py

everything = []

with open('input') as f:
    for line in f.readlines():
        for item in line.split(" "):
            everything += [int(item)]

print(sum(everything))
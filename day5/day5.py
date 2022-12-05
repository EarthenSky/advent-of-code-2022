# to run: 
# python day5.py
# pypy day5.py

score = ""
model = []
numlines = 0

with open('input5') as f:
    for line in f.readlines():
        numlines += 1
        thelen = int(len(line) / 4)
        
        line = line.replace("\n", "")
        
        if line == "":
            break

        if len(model) == 0:
            for i in range(0, thelen):
                model.append([])

        do_continue = False
        for i in range(0, thelen):
            if line[1 + i*4] == "1":
                do_continue = True
                break
            elif line[1 + i*4] != " ":
                model[i] += [line[1 + i*4]]

        if do_continue:
            continue 

    for ls in model:
        ls.reverse()

    print(model)

with open('input5') as f:
    i = 0
    for line in f.readlines():
        i += 1
        if i <= numlines:
            continue

        line = line.replace("\n", "")
        words = line.split(" ")
        
        numof = int(words[1])
        start = int(words[3])-1
        end = int(words[5])-1

        for i2 in range(0, numof):
            val = model[start].pop()
            model[end].append(val)

        print(model)

    print(model)

    for ls in model:
        score += ls[-1]

# put part 2 here
score2 = ""
model = []
numlines = 0

with open('input5') as f:
    for line in f.readlines():
        numlines += 1
        thelen = int(len(line) / 4)
        
        line = line.replace("\n", "")
        
        if line == "":
            break

        if len(model) == 0:
            for i in range(0, thelen):
                model.append([])

        do_continue = False
        for i in range(0, thelen):
            if line[1 + i*4] == "1":
                do_continue = True
                break
            elif line[1 + i*4] != " ":
                model[i] += [line[1 + i*4]]

        if do_continue:
            continue 

    for ls in model:
        ls.reverse()

    print(model)

with open('input5') as f:
    i = 0
    for line in f.readlines():
        i += 1
        if i <= numlines:
            continue

        line = line.replace("\n", "")
        words = line.split(" ")
        
        numof = int(words[1])
        start = int(words[3])-1
        end = int(words[5])-1

        tmp_stack = []
        for i2 in range(0, numof):
            val = model[start].pop()
            tmp_stack.append(val)
            
        tmp_stack.reverse()

        for val in tmp_stack:
            model[end].append(val)

        print(model)

    print(model)

    for ls in model:
        score2 += ls[-1]

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

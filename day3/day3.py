# to run: 
# python day3.py
# pypy day3.py

score = 0
with open('input3') as f:
    for line in f.readlines():
        line = line.replace("\n", "")

        loc = int(len(line)/2)
        compartment1 = line[:loc]
        compartment2 = line[loc:]

        for ch in compartment1:
            if ch in compartment2: 
                if ch.isupper():
                    score += ord(ch) - ord('A') + 27
                else:
                    score += ord(ch) - ord('a') + 1
                    
                break

score2 = 0
with open('input3') as f:
    line1 = None
    line2 = None
    line3 = None
    for line in f.readlines():
        line = line.replace("\n", "")

        if line1 == None:
            line1 = line
        elif line2 == None:
            line2 = line
        elif line3 == None:
            line3 = line
            
            for ch in line1:
                if ch in line2 and ch in line3:
                    if ch.isupper():
                        score2 += ord(ch) - ord('A') + 27
                    else:
                        score2 += ord(ch) - ord('a') + 1
                    break 
                    
            line1 = None
            line2 = None
            line3 = None

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

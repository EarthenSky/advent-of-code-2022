# to run: 
# python day4.py
# pypy day4.py

score = 0
with open('input4') as f:
    for line in f.readlines():
        pair = line.replace("\n", "")
        pair1 = pair.split(",")[0]
        pair2 = pair.split(",")[1]

        left1 = int(pair1.split("-")[0])
        right1 = int(pair1.split("-")[1])

        left2 = int(pair2.split("-")[0])
        right2 = int(pair2.split("-")[1])

        if (left1 <= left2 and right1 >= right2) or (left1 >= left2 and right1 <= right2):
            score += 1

score2 = 0
# put part 2 here
with open('input4') as f:
    for line in f.readlines():
        pair = line.replace("\n", "")
        pair1 = pair.split(",")[0]
        pair2 = pair.split(",")[1]

        left1 = int(pair1.split("-")[0])
        right1 = int(pair1.split("-")[1])

        left2 = int(pair2.split("-")[0])
        right2 = int(pair2.split("-")[1])

        if (left1 >= left2 and left1 <= right2) or (right1 >= left2 and right1 <= right2) or (left1 <= left2 and right1 >= right2):
            score2 += 1

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

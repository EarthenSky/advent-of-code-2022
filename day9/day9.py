#import itertools

# -------------------------
# part 1:

score = 0
with open('input9') as f:
    head_pos = [0,0]
    tail_pos = [0,0]
    visited = { "0#0": True }
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "").split(" ")
        
        dir = line[0]
        num = int(line[1])
        
        step = [0,0]
        if dir == "R":
            step[0] = 1
        elif dir == "L":
            step[0] = -1
        elif dir == "U":
            step[1] = 1
        elif dir == "D":
            step[1] = -1

        for i in range(0,num):
            head_pos[0] += step[0]
            head_pos[1] += step[1]
            diff = [head_pos[0]-tail_pos[0],head_pos[1]-tail_pos[1]]

            if abs(diff[0]) <= 1 and abs(diff[1]) <= 1:
                pass
            elif diff[0] == 0 and abs(diff[1]) >= 2:
                tail_pos = [tail_pos[0], tail_pos[1] + int(diff[1] / abs(diff[1]))]
            elif diff[1] == 0 and abs(diff[0]) >= 2:
                tail_pos = [tail_pos[0] + int(diff[0] / abs(diff[0])), tail_pos[1]]
            else:
                tail_pos = [tail_pos[0] + int(diff[0] / abs(diff[0])), tail_pos[1] + int(diff[1] / abs(diff[1]))]

            visited["{}#{}".format(tail_pos[0], tail_pos[1])] = True

    score = len(visited)
# -------------------------
# part 2:

score2 = 0
with open('input9') as f:
    segment_pos_list = [[0,0] for _ in range(10)]
    visited = { "0#0": True }
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "").split(" ")
        
        dir = line[0]
        num = int(line[1])
        
        step = [0,0]
        if dir == "R":
            step[0] = 1
        elif dir == "L":
            step[0] = -1
        elif dir == "U":
            step[1] = 1
        elif dir == "D":
            step[1] = -1
        
        for i in range(0,num):
            segment_pos_list[0][0] += step[0]
            segment_pos_list[0][1] += step[1]
            for i in range(len(segment_pos_list)):
                if i == 9: break

                diff = [segment_pos_list[i][0]-segment_pos_list[i+1][0],segment_pos_list[i][1]-segment_pos_list[i+1][1]]

                if abs(diff[0]) <= 1 and abs(diff[1]) <= 1:
                    pass
                elif diff[0] == 0 and abs(diff[1]) >= 2:
                    segment_pos_list[i+1] = [segment_pos_list[i+1][0], segment_pos_list[i+1][1] + int(diff[1] / abs(diff[1]))]
                elif diff[1] == 0 and abs(diff[0]) >= 2:
                    segment_pos_list[i+1] = [segment_pos_list[i+1][0] + int(diff[0] / abs(diff[0])), segment_pos_list[i+1][1]]
                else:
                    segment_pos_list[i+1] = [segment_pos_list[i+1][0] + int(diff[0] / abs(diff[0])), segment_pos_list[i+1][1] + int(diff[1] / abs(diff[1]))]

            visited["{}#{}".format(segment_pos_list[9][0], segment_pos_list[9][1])] = True
    
    score2 = len(visited)

# -------------------------
# output:

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

#import itertools

# to run: 
# python day7.py
# pypy day7.py

# -------------------------
# part 1:

score = 0
with open('input7') as f:
    directory_store = {}
    current_path = []

    # parse commands & build the directory
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "")
        if line[0] == "$":
            if line[2:].startswith("ls"):
                pass
            elif line == "$ cd /":
                current_path = ["/"]
            elif line == "$ cd ..":
                current_path.pop()
            elif line[2:].startswith("cd"):
                thedir = line[4:].strip()
                current_path += [thedir + "/"]
        else:
            path_str = "".join(current_path)
            if not path_str in directory_store:
                directory_store[path_str] = []
                
            if line.startswith("dir"):
                child_dir = line.split(" ")[1]
                directory_store[path_str] += [child_dir]
            else:
                file_size = int(line.split(" ")[0])
                directory_store[path_str] += [file_size]

    completed_dirs = {}

    # recursively figure out sizes for all dirs
    while len(directory_store) != 0:
        for path, dir in directory_store.items():
            thesum = 0
            
            found_sum = True
            for item in dir:
                if isinstance(item, str):
                    child_path = path + item + "/"
                    if child_path in completed_dirs:
                        thesum += int(completed_dirs[child_path])
                    else:
                        found_sum = False
                        break
                else:
                    thesum += item

            if found_sum:
                completed_dirs[path] = thesum   

        for key,val in completed_dirs.items():
            if key in directory_store:
                del directory_store[key]

    # compute score
    for key, val in completed_dirs.items():
        if key == "/":
            continue

        if val <= 100000:
            score += val

# -------------------------
# part 2:

total_space = 70000000
needed_space = 30000000

score2 = 0
with open('input7') as f:
    directory_store = {}
    current_path = []

    # parse commands & build the directory
    lines = f.readlines() # any subsequent calls to readlines will return []
    for line in lines:
        line = line.replace("\n", "")
        if line[0] == "$":
            if line[2:].startswith("ls"):
                pass
            elif line == "$ cd /":
                current_path = ["/"]
            elif line == "$ cd ..":
                current_path.pop()
            elif line[2:].startswith("cd"):
                thedir = line[4:].strip()
                current_path += [thedir + "/"]
        else:
            path_str = "".join(current_path)
            if not path_str in directory_store:
                directory_store[path_str] = []
                
            if line.startswith("dir"):
                child_dir = line.split(" ")[1]
                directory_store[path_str] += [child_dir]
            else:
                file_size = int(line.split(" ")[0])
                directory_store[path_str] += [file_size]

    completed_dirs = {}

    # recursively figure out sizes for all dirs
    while len(directory_store) != 0:
        for path, dir in directory_store.items():
            thesum = 0
            
            found_sum = True
            for item in dir:
                if isinstance(item, str):
                    child_path = path + item + "/"
                    if child_path in completed_dirs:
                        thesum += int(completed_dirs[child_path])
                    else:
                        found_sum = False
                        break
                else:
                    thesum += item

            if found_sum:
                completed_dirs[path] = thesum   

        for key,val in completed_dirs.items():
            if key in directory_store:
                del directory_store[key]

    used_space = completed_dirs["/"]
    to_free = needed_space - (total_space - used_space)
    score2 = total_space

    for key, val in completed_dirs.items():
        if val >= to_free:
            score2 = min(score2, val)
            
# -------------------------
# output:

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

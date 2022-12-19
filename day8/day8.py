#import itertools

# -------------------------
# part 1:

score = 0
with open('input8') as f:
    oldlines = list(f.readlines()) # any subsequent calls to readlines will return []
    newlines = []
    for line in oldlines:
        newlines += [line.replace("\n", "")]
    height = len(newlines)
    width = len(newlines[0])
    score = 2 * width + 2 * (height-2)
    full = "".join(newlines)
    for y in range(1, height-1):
        for x in range(1, width-1):
            cur = int(full[y*width+x])

            success = True
            for i in range(0, y):
                cur2 = int(full[i*width+x])
                if cur2 >= cur:
                    success = False
                    break

            if not success:
                success = True
                for i in range(y+1, height):
                    cur2 = int(full[i*width+x])
                    if cur2 >= cur:
                        success = False
                        break

            if not success:
                success = True
                for i in range(0, x):
                    cur2 = int(full[y*width+i])
                    if cur2 >= cur:
                        success = False
                        break

            if not success:
                success = True
                for i in range(x+1, width):
                    cur2 = int(full[y*width+i])
                    if cur2 >= cur:
                        success = False
                        break

            score += 1 if success else 0

# -------------------------
# part 2:

score2 = 0
with open('input8') as f:
    oldlines = list(f.readlines()) # any subsequent calls to readlines will return []
    newlines = []
    for line in oldlines:
        newlines += [line.replace("\n", "")]
#   // wait, maybe what i actually want here is to initialize via lambda?
#   // what @takescope does, is just a kind of generic thing where it uses the scope to initialize the two variables
#   // this may be a bad idea, but the concept here is to separate the scopes of variables more easily without building new functions & naming them something.
#   // naming a function is challenging, and sometimes meaningless. We want the context to be able to describe certain common behaviours whenever possible. Minimize names!
#   // a good example of this is the varaible i, j, k, etc... used for looping. These variable names are a fine solution, but we just don't want to be forced to name things that don't matter (in this case the names do matter though, so nevermind I guess lols)
#   newlines, height = @takescope{
#       newlines = []
#       for line in f.readlines():
#           this += [line.replace("\n", "")]
#       height = len(newlines)
#   }
    height = len(newlines)
    width = len(newlines[0])
    full = "".join(newlines)
    for y in range(1, height-1):
        for x in range(1, width-1):
            cur = int(full[y*width+x])

            scene_scores = [] 
            count = 1
            for i in range(y-1, -1, -1):
                cur2 = int(full[i*width+x])
                if cur2 >= cur:
                    scene_scores += [count]
                    break
                count += 1
            if count == len(range(0, y))+1:
                scene_scores += [count-1]

            count = 1
            for i in range(y+1, height):
                cur2 = int(full[i*width+x])
                if cur2 >= cur:
                    scene_scores += [count]
                    break
                count += 1
            if count == len(range(y+1, height))+1:
                scene_scores += [count-1]

            count = 1
            for i in range(x-1, -1, -1):
                cur2 = int(full[y*width+i])
                if cur2 >= cur:
                    scene_scores += [count]
                    break
                count += 1
            if count == len(range(0, x))+1:
                scene_scores += [count-1]

            # NOTE: this is a tagged union "or" type
            # fn myfunc(x :: int, z :: auto, y :: any) -> int | string {
            #     // do stuff
            # }

            # count :: auto is the implied declaration statement
            # problem: its confusing / sucky in python that you don't declare variables; what if you accidentally shadow?!?
            # solution: this is a simple rule w/ nice syntax for how to type variables & what the automatic rules are.
            # TODO: for umbrella make a document with concrete problem & possible solution statements, 
            # and then choose the solution that fits best with goals, while other solutions are backup plans.
            count = 1
            for i in range(x+1, width):
                cur2 = int(full[y*width+i])
                if cur2 >= cur:
                    scene_scores += [count]
                    break
                count += 1
            if count == len(range(x+1, width))+1:
                scene_scores += [count-1]

            #print(scene_scores)

            tmp = 1
            for item in scene_scores:
                tmp *= item

            score2 = max(score2, tmp)
            
# -------------------------
# output:

part1 = score
part2 = score2

print("part1: {}\t".format(part1))
print("part2: {}\t".format(part2))

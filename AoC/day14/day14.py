"""AoC day 14."""


contents = []
with open('./AoC/day14/input.txt', 'r') as f:
    [contents.append([[int(z) for z in y.split(',')] for y in x.strip().split(' -> ')]) for x in f.readlines()]

minx = 500
maxx = 500
miny = 0
maxy = 0
resevoir = {}
sand = {}
for line in contents:
    pos = line[0]
    resevoir[str(pos[0]) + ',' + str(pos[1])] = '#'
    for n in line[1:]:
        resevoir[str(pos[0]) + ',' + str(pos[1])] = '#'
        while pos != n:
            if pos[0] > maxx:
                maxx = pos[0]
            if pos[0] < minx:
                minx = pos[0]
            if pos[1] > maxy:
                maxy = pos[1]
            if pos[1] < miny:
                miny = pos[1]

            if pos[0] > n[0]:
                pos[0] -= 1
            elif pos[0] < n[0]:
                pos[0] += 1
            elif pos[1] > n[1]:
                pos[1] -= 1
            elif pos[1] < n[1]:
                pos[1] += 1
            resevoir[str(pos[0]) + ',' + str(pos[1])] = '#'

while True:
    s_pos = (500, 0)
    while True:
        loc = str(s_pos[0]) + ',' + str(s_pos[1] + 1)
        loc2 = str(s_pos[0] - 1) + ',' + str(s_pos[1] + 1)
        loc3 = str(s_pos[0] + 1) + ',' + str(s_pos[1] + 1)
        if not (loc in sand.keys() or loc in resevoir.keys()):
            s_pos = (s_pos[0], s_pos[1] + 1)
        elif not (loc2 in sand.keys() or loc2 in resevoir.keys()):
            s_pos = (s_pos[0] - 1, s_pos[1] + 1)
        elif not (loc3 in sand.keys() or loc3 in resevoir.keys()):
            s_pos = (s_pos[0] + 1, s_pos[1] + 1)
        else:
            sand[str(s_pos[0]) + ',' + str(s_pos[1])] = 'o'
            break
        if s_pos[1] > maxy:
            break
    if s_pos[1] > maxy:
        break


print(f'Part 1: {len(sand)}')

sand = {}
maxy += 2
for i in range(minx - 300, maxx * 2):
    resevoir[str(i) + ',' + str(maxy)] = '#'


while True:
    s_pos = (500, 0)
    while True:
        loc = str(s_pos[0]) + ',' + str(s_pos[1] + 1)
        loc2 = str(s_pos[0] - 1) + ',' + str(s_pos[1] + 1)
        loc3 = str(s_pos[0] + 1) + ',' + str(s_pos[1] + 1)
        if not (loc in sand.keys() or loc in resevoir.keys()):
            s_pos = (s_pos[0], s_pos[1] + 1)
        elif not (loc2 in sand.keys() or loc2 in resevoir.keys()):
            s_pos = (s_pos[0] - 1, s_pos[1] + 1)
        elif not (loc3 in sand.keys() or loc3 in resevoir.keys()):
            s_pos = (s_pos[0] + 1, s_pos[1] + 1)
        else:
            sand[str(s_pos[0]) + ',' + str(s_pos[1])] = 'o'
            break
        if s_pos == (500, 0):
            break
    if '500,0' in sand.keys():
        break


print(f'Part 2: {len(sand)}')

"""AoC day 12."""


looked = []


def can_travel(coord, hmap):
    """Where can we move."""
    global looked
    possibilities = []
    height = hmap[coord[1]][coord[0]]
    if coord[1] + 1 < len(hmap) and hmap[coord[1] + 1][coord[0]] >= height - 1:
        possibilities.append((coord[0], coord[1] + 1))
    if coord[1] - 1 >= 0 and hmap[coord[1] - 1][coord[0]] >= height - 1:
        possibilities.append((coord[0], coord[1] - 1))
    if (coord[0] + 1 < len(hmap[0])
            and hmap[coord[1]][coord[0] + 1] >= height - 1):
        possibilities.append((coord[0] + 1, coord[1]))
    if coord[0] - 1 >= 0 and hmap[coord[1]][coord[0] - 1] >= height - 1:
        possibilities.append((coord[0] - 1, coord[1]))
    return possibilities


hmap = []
start = (0, 0)
end = (0, 0)
with open('./AoC/day12/input.txt', 'r') as f:
    contents = []
    [contents.append(x.strip()) for x in f.readlines()]
    y = 0
    for line in contents:
        row = []
        x = 0
        for c in line:
            if c == 'S':
                row.append(0)
                start = (x, y)
            elif c == 'E':
                row.append(ord('z') - ord('a'))
                end = (x, y)
            else:
                row.append(ord(c) - ord('a'))
            x += 1
        hmap.append(row)
        y += 1


height = hmap[end[1]][end[0]]
to_look = [(0, end)]
i = 0
not_found_start = True
mini = 9999
part1 = 0
while not_found_start:
    n = []
    if to_look == []:
        break
    for k, v in to_look:
        ps = can_travel(v, hmap)
        for p in ps:
            seen = False
            for h in looked:
                if p == h[1]:
                    seen = True
            for h in n:
                if p == h[1]:
                    seen = True
            if not seen:
                n.append((k + 1, p))
            if p == start:
                part1 = k + 1
                not_found_start = False
            if hmap[p[1]][p[0]] == 0:
                if k + 1 < mini:
                    mini = k + 1

    looked.extend(to_look)
    to_look = n


print(f'Part 1: {part1}')
print(f'Part 2: {mini}')

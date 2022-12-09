"""AoC day 3."""


contents = []
with open('./AoC/day3/input.txt', 'r') as f:
    [contents.append(x.strip()) for x in f.readlines()]


def compare(contents):
    """Compare."""
    pocket1 = contents[0:int(len(contents) / 2)]
    pocket2 = contents[int(len(contents) / 2):]
    p1 = set([x for x in pocket1])
    duplicates = []
    for c in pocket2:
        if c in p1 and c not in duplicates:
            duplicates.append(c)
    if duplicates[0].islower():
        return ord(duplicates[0]) - 96
    return ord(duplicates[0]) - 38


def part1(contents):
    """Part 1."""
    total = 0
    for content in contents:
        total += compare(content)

    print(f'Part 1: {total}')


def part2(contents):
    """Part 2."""
    badges = []
    for _ in range(int(len(contents) / 3)):
        item_map = {}
        for _ in range(3):
            pack_set = set(contents.pop(0))
            for c in pack_set:
                if c in item_map:
                    item_map[c] += 1
                else:
                    item_map[c] = 1
        for k, v in item_map.items():
            if v >= 3:
                badges.append(k)
    total = 0
    for c in badges:
        if c.islower():
            total += ord(c) - 96
        else:
            total += ord(c) - 38
    print(f'Part 2: {total}')


part1(contents[:])
part2(contents[:])

"""AoC day4."""


contents = []
with open('./AoC/day4/input.txt', 'r') as f:
    [
        contents.append(
            [int(x) for x in x.strip().replace(',', '-').split('-')]
        )
        for x in f.readlines()
    ]


def part1(ranges):
    """Part 1."""
    e1 = list(range(ranges[0], ranges[1] + 1))
    e2 = list(range(ranges[2], ranges[3] + 1))
    if any([all([x in e2 for x in e1]), all([x in e1 for x in e2])]):
        return 1
    return 0


def part2(ranges):
    """Part 2."""
    e1 = list(range(ranges[0], ranges[1] + 1))
    e2 = list(range(ranges[2], ranges[3] + 1))
    if any([x in e2 for x in e1]):
        return 1
    return 0


print(f'Part 1: {sum([part1(x) for x in contents])}')
print(f'Part 2: {sum([part2(x) for x in contents])}')

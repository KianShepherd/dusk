"""AoC day 9."""


contents = []
with open('./AoC/day9/input.txt', 'r') as f:
    [contents.append(x.strip().split(' ')) for x in f.readlines()]
    for i in range(len(contents)):
        contents[i][1] = int(contents[i][1])

head = (0, 0)
tail = (0, 0)
tail_locs = {}


def move(direction, head, tail):
    """Move head."""
    if direction == 'U':
        head = (head[0], head[1] + 1)
    elif direction == 'D':
        head = (head[0], head[1] - 1)
    elif direction == 'R':
        head = (head[0] + 1, head[1])
    elif direction == 'L':
        head = (head[0] - 1, head[1])
    if abs(head[0] - tail[0]) > 1 or abs(head[1] - tail[1]) > 1:
        diff = (
            min(max(-1, head[0] - tail[0]), 1),
            min(max(-1, head[1] - tail[1]), 1)
        )
        tail = (tail[0] + diff[0], tail[1] + diff[1])

    return head, tail


def move_chain(direction, chain):
    """Move chain."""
    head = chain.pop(0)
    tail = chain[0]
    head, tail = move(direction, head, tail)
    new_chain = [head]
    chain[0] = tail
    for _ in range(len(chain) - 1):
        head = chain.pop(0)
        tail = chain[0]
        if abs(head[0] - tail[0]) > 1 or abs(head[1] - tail[1]) > 1:
            diff = (
                min(max(-1, head[0] - tail[0]), 1),
                min(max(-1, head[1] - tail[1]), 1)
            )
            tail = (tail[0] + diff[0], tail[1] + diff[1])
            chain[0] = tail
        new_chain.append(head)
    new_chain.append(tail)

    return new_chain


for moves in contents:
    direction = moves[0]
    count = moves[1]
    while count > 0:
        head, tail = move(direction, head, tail)
        count -= 1
        tail_locs[tail] = 1

print(f'Part 1: {len(tail_locs)}')

chain = [(0, 0) for _ in range(10)]
tail_locs = {}

for moves in contents:
    direction = moves[0]
    count = moves[1]
    while count > 0:
        chain = move_chain(direction, chain)
        count -= 1
        tail_locs[chain[-1]] = 1

print(f'Part 2: {len(tail_locs)}')

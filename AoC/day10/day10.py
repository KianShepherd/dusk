"""AoC day 10."""


contents = []
with open('./AoC/day10/input.txt', 'r') as f:
    [contents.append(x.strip()) for x in f.readlines()]

print(contents)

reg = 1
cycles = []

for op in contents:
    if op == 'noop':
        cycles.append(reg)
    else:
        cycles.append(reg)
        cycles.append(reg)
        reg += int(op.split(' ')[1])


idxs = [20]
[idxs.append(i) for i in range(60, len(cycles), 40)]

print(f'Part 1: {sum([cycles[i - 1] * i for i in idxs])}')


print('Part 2:')
for i in range(int(len(cycles) / 40)):
    s = ''
    for j in range(40):
        if abs(cycles[(40 * i) + j] - j) < 2:
            s += '#'
        else:
            s += ' '
    print(s)

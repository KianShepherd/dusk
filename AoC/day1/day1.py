"""AoC day1."""
cals = []
with open('./AoC/day1/input.txt', 'r') as f:
    [cals.append(int(x) if x != '' else -1) for x in f.read().split('\n')]

elves = []
i = 0
total = 0
while True:
    if i >= len(cals):
        break
    if cals[i] == -1:
        elves.append(total)
        total = 0
    else:
        total += cals[i]
    i += 1
elves = sorted(elves)[::-1]

print(f'Part 1: {elves[0]}')
print(f'Part 2: {sum(elves[:3])}')

"""AoC day 13."""


from functools import cmp_to_key


pairs = []
with open('./AoC/day13/input.txt', 'r') as f:
    contents = []
    [contents.append(x.strip()) for x in f.readlines() if x.strip() != '']
    for i in range(int(len(contents) / 2)):
        pairs.append((eval(contents[2 * i]), eval(contents[2 * i + 1])))


def _in_order(lhs, rhs):
    lhs = lhs if isinstance(lhs, list) else [lhs]
    rhs = rhs if isinstance(rhs, list) else [rhs]
    for i in range(min(len(lhs), len(rhs))):
        if isinstance(lhs[i], list) or isinstance(rhs[i], list):
            res = _in_order(lhs[i], rhs[i])
        else:
            res = rhs[i] - lhs[i]
        if res != 0:
            return res
    return len(rhs) - len(lhs)


def in_order(lhs, rhs):
    """Part 1."""
    return _in_order(lhs, rhs) > 0


total = 0
i = 1
for p in pairs:
    i = i + 1
    if in_order(p[0], p[1]):
        total += i + 1

print(f'Part 1: {total}')

pairs.append([[[2]], [[6]]])
pairs = [y for x in pairs for y in x]
pairs = sorted(pairs, key=cmp_to_key(_in_order), reverse=True)
p1 = 0
p2 = 0
i = 0
for p in pairs:
    i = i + 1
    if p in ([[2]], [[6]]):
        if p1 == 0:
            p1 = i
        else:
            p2 = i
            break
print(f'Part 2: {p1 * p2}')

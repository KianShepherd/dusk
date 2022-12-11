"""AoC day 11."""


from math import floor


class Monkey:
    """Monkey class helper."""

    def __init__(self, lines):
        """Init."""
        self.id = lines[0].split(' ')[1][:-1]
        self.items = [int(x) for x in lines[1][16:].split(', ')]
        self.op = ' '.join(lines[2].split(' ')[3:])
        self.num = lines[2].split(' ')[5]
        self.test = int(lines[3].split(' ')[3])
        self.t = int(lines[4].split(' ')[5])
        self.f = int(lines[5].split(' ')[5])
        self.monkeys = []
        self.inspected = 0

    def all_monkeys(self, monkeys):
        """Set all monkeys."""
        self.monkeys = monkeys

    def turn(self, *, part2=False):
        """Do turn."""
        for i in self.items:
            self.inspected += 1
            old = eval(self.op.replace('old', str(i)))
            if not part2:
                old = floor(old / 3)
            if eval(f'{old} % {self.test}') == 0:
                old = old % 9699690
                self.monkeys[self.t].items.append(old)
            else:
                old = old % 9699690
                self.monkeys[self.f].items.append(old)
        self.items = []


monkeys = []

with open('./AoC/day11/input.txt', 'r') as f:
    contents = []
    _monkeys = []
    [contents.append(x.strip()) for x in f.readlines()]
    cur_monkey = []
    for line in contents:
        if line == '':
            _monkeys.append(cur_monkey)
            cur_monkey = []
        else:
            cur_monkey.append(line)
    _monkeys.append(cur_monkey)
    [monkeys.append(Monkey(x)) for x in _monkeys]
    for m in monkeys:
        m.all_monkeys(monkeys)

for _ in range(20):
    for m in monkeys:
        m.turn()

counts = []
for m in monkeys:
    counts.append(m.inspected)
counts.sort()

print(f'Part 1: {counts[-1] * counts[-2]}')

monkeys = []

with open('./AoC/day11/input.txt', 'r') as f:
    contents = []
    _monkeys = []
    [contents.append(x.strip()) for x in f.readlines()]
    cur_monkey = []
    for line in contents:
        if line == '':
            _monkeys.append(cur_monkey)
            cur_monkey = []
        else:
            cur_monkey.append(line)
    _monkeys.append(cur_monkey)
    [monkeys.append(Monkey(x)) for x in _monkeys]
    for m in monkeys:
        m.all_monkeys(monkeys)

for _ in range(10000):
    for m in monkeys:
        m.turn(part2=True)

counts = []
for m in monkeys:
    counts.append(m.inspected)
counts.sort()

print(f'Part 2: {counts[-1] * counts[-2]}')

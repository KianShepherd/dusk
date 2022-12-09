"""AoC day 5."""


import re


class PuzzleState:
    """Puzzle State Handler."""

    def __init__(self, lines):
        """Init."""
        cols = [
            int(x)
            for x in
            re.split(
                r'\s+',
                lines[-1].strip('\n').strip()
            )
        ]
        self.cols = [[] for x in range(len(cols))]
        lines = lines[:-1][::-1]
        idxs = [1 + (4 * x) for x in range(len(self.cols))]
        for line in lines:
            for i in range(len(idxs)):
                if idxs[i] >= len(line):
                    break
                if line[idxs[i]] != ' ':
                    self.cols[i].append(line[idxs[i]])

    def move(self, count, fro, to):
        """Move part 1."""
        if len(self.cols[fro]) == 0:
            return
        self.cols[to].append(self.cols[fro].pop())
        count -= 1
        if count > 0:
            self.move(count, fro, to)

    def move2(self, count, fro, to):
        """Move part 2."""
        if len(self.cols[fro]) == 0:
            return
        to_add = []
        while len(self.cols[fro]) > 0 and count > 0:
            to_add.append(self.cols[fro].pop())
            count -= 1

        self.cols[to].extend(to_add[::-1])
        count -= 1
        if count > 0:
            self.move(count, fro, to)


contents = []
puzzle_state_input = []
puzzle_moves_input = []
with open('./AoC/day5/input.txt', 'r') as f:
    [contents.append(x) for x in f.readlines()]
    reading_state = True
    idx = 0
    for idx in range(len(contents)):
        if 'move' not in contents[idx]:
            puzzle_state_input.append(contents[idx])
        else:
            moves = []
            line = contents[idx].split(' ')
            puzzle_moves_input.append(
                [
                    int(line[1]),
                    int(line[3]),
                    int(line[5])
                ]
            )


puzzle_state_input = puzzle_state_input[:-1]
puzzle = PuzzleState(puzzle_state_input[:])

for move in puzzle_moves_input:
    puzzle.move(move[0], move[1] - 1, move[2] - 1)

part1 = ''.join([x[-1] for x in puzzle.cols])

puzzle = PuzzleState(puzzle_state_input[:])

for move in puzzle_moves_input:
    puzzle.move2(move[0], move[1] - 1, move[2] - 1)

part2 = ''.join([x[-1] for x in puzzle.cols])

print(f'Part 1: {part1}')
print(f'Part 2: {part2}')

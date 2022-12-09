"""AoC day 6."""


contents = None
with open('./AoC/day6/input.txt', 'r') as f:
    contents = f.read().strip()


def get_packet_start(stream):
    """Get packet start."""
    for i in range(len(stream) - 3):
        letters = {}
        unique = True
        for j in range(4):
            if stream[i + j] in letters.keys():
                unique = False
                break
            letters[stream[i + j]] = 1
        if unique:
            return i + 4


def get_message_start(stream):
    """Get message start."""
    for i in range(len(stream) - 13):
        letters = {}
        unique = True
        for j in range(14):
            if stream[i + j] in letters.keys():
                unique = False
                break
            letters[stream[i + j]] = 1
        if unique:
            return i + 14


print(f'Part 1: {get_packet_start(contents)}')
print(f'Part 2: {get_message_start(contents)}')

"""AoC day2."""
games = []
with open('./AoC/day2/input.txt', 'r') as f:
    [games.append(x.strip().split(' ')) for x in f.readlines()]


# A - Rock
# B - Paper
# C - Scissors

# X - Rock     - 1
# Y - Paper    - 2
# Z - Scissors - 3

# Win  - 6
# Draw - 3
# Loss - 0

# X - Lose
# Y - Draw
# Z - Win


def get_winner_score(oponenet, me):
    """AoC."""
    if oponenet == 'A':
        if me == 'X':
            return 3
        elif me == 'Y':
            return 6
        else:
            return 0
    elif oponenet == 'B':
        if me == 'X':
            return 0
        elif me == 'Y':
            return 3
        else:
            return 6

    else:
        if me == 'X':
            return 6
        elif me == 'Y':
            return 0
        else:
            return 3


def get_choice_score(me):
    """AoC."""
    if me == 'X':
        return 1
    elif me == 'Y':
        return 2
    else:
        return 3


def get_score(oponenet, me):
    """AoC."""
    return get_winner_score(oponenet, me) + get_choice_score(me)


def score_part1(games):
    """AoC."""
    score = 0
    for game in games:
        score += get_score(game[0], game[1])
    return score


def get_my_choice(oponenet, me):
    """AoC."""
    if oponenet == 'A':
        if me == 'X':
            return 'Z'
        elif me == 'Y':
            return 'X'
        else:
            return 'Y'
    elif oponenet == 'B':
        if me == 'X':
            return 'X'
        elif me == 'Y':
            return 'Y'
        else:
            return 'Z'

    else:
        if me == 'X':
            return 'Y'
        elif me == 'Y':
            return 'Z'
        else:
            return 'X'


def score_part2(games):
    """AoC."""
    score = 0
    for game in games:
        score += get_score(game[0], get_my_choice(game[0], game[1]))
    return score


print(score_part1(games))
print(score_part2(games))

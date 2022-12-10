"""AoC day 8."""


contents = []
with open('./AoC/day8/input.txt', 'r') as f:
    [contents.append([int(y) for y in x.strip()]) for x in f.readlines()]


def count_visible(trees):
    """Count visible trees."""
    height = len(trees)
    width = len(trees[0])
    visible_trees = (2 * height) + (2 * (width - 2))
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            tree_height = trees[i][j]
            all_lower = True
            for k in range(0, i):
                if trees[k][j] >= tree_height:
                    all_lower = False
            if all_lower:
                visible_trees += 1
                continue
            all_lower = True
            for k in range(i + 1, height):
                if trees[k][j] >= tree_height:
                    all_lower = False
            if all_lower:
                visible_trees += 1
                continue
            all_lower = True
            for k in range(0, j):
                if trees[i][k] >= tree_height:
                    all_lower = False
            if all_lower:
                visible_trees += 1
                continue
            all_lower = True
            for k in range(j + 1, width):
                if trees[i][k] >= tree_height:
                    all_lower = False
            if all_lower:
                visible_trees += 1
                continue
            all_lower = True
    return visible_trees


def get_greatest_scenic_score(trees):
    """Get trees scenic score."""
    height = len(trees)
    width = len(trees[0])
    best = 0
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            tree_height = trees[i][j]
            above = 0
            below = 0
            left = 0
            right = 0
            k = 1
            while True:
                above += 1
                if trees[i - k][j] >= tree_height or i - k == 0:
                    break
                k += 1
            k = 1
            while True:
                below += 1
                if trees[i + k][j] >= tree_height or i + k == height - 1:
                    break
                k += 1
            k = 1
            while True:
                left += 1
                if trees[i][j - k] >= tree_height or j - k == 0:
                    break
                k += 1
            k = 1
            while True:
                right += 1
                if trees[i][j + k] >= tree_height or j + k == width - 1:
                    break
                k += 1
            score = above * below * left * right
            if score > best:
                best = score
    return best


print(f'Part 1: {count_visible(contents)}')
print(f'Part 2: {get_greatest_scenic_score(contents)}')

"""AoC day 7."""

commands = []


class Directory:
    """Dir helper class."""

    def __init__(self, path, parent):
        """Init."""
        self.path = path
        name = '/'
        if path != '/':
            name = path[:-1].split('/')[-1]
        self.name = name
        self.dirs = []
        self.files = []
        self.parent = parent
        self.lsd = False

    def ls(self, contents):
        """LS command."""
        if self.lsd:
            return
        for content in contents:
            if 'dir' in content:
                self.dirs.append(
                    Directory(
                        self.path + content.split(' ')[1] + '/',
                        self
                    )
                )
            else:
                spl_ls = content.split(' ')
                spl_ls[0] = int(spl_ls[0])
                self.files.append(spl_ls)
        self.lsd = True

    def get_dir(self, dir_name):
        """Get subdir."""
        for d in self.dirs:
            if d.name == dir_name:
                return d
        self.dirs.append(Directory(self.path + dir_name + '/', self))
        return self.dirs[-1]

    def debug(self):
        """Debug."""
        self._debug(0)

    def _debug(self, depth):
        """Debug internal."""
        print(
            f'{4 * depth * " "}Dir:\n{4 * (depth + 1) * " "}name: '
            f'{self.name}\n{4 * (depth + 1) * " "}path: {self.path}\n'
            f'{4 * (depth + 1) * " "}size: {self.size}'
        )
        for file in self.files:
            print(f'{4 * (depth + 1) * " "}File: {file[1]} - size: {file[0]}')
        for d in self.dirs:
            d._debug(depth + 1)

    def calc_sizes(self):
        """Calc dir sizes."""
        size = 0
        for f in self.files:
            size += f[0]
        for d in self.dirs:
            size += d.calc_sizes()
        self.size = size
        return size

    def get_dirs(self):
        """Get all dirs."""
        dirs = [self]
        for d in self.dirs:
            dirs.extend(d.get_dirs())
        return dirs


with open('./AoC/day7/input.txt', 'r') as f:
    contents = []
    [contents.append(x.strip()) for x in f.readlines()]
    cur_command = []
    while True:
        if '$' in contents[0]:
            if cur_command != []:
                commands.append(cur_command)
            cur_command = [contents[0]]
        else:
            cur_command.append(contents[0])
        contents.pop(0)
        if len(contents) == 0:
            commands.append(cur_command)
            break


location = ''
cur_dir = None

for command in commands:
    if 'cd' in command[0]:
        spl_cmd = command[0].split(' ')
        if location == '':
            location = spl_cmd[2]
            cur_dir = Directory('/', None)
        else:
            if spl_cmd[2] == '..':
                location = '/'.join(location.split('/')[:-2]) + '/'
                cur_dir = cur_dir.parent
            else:
                location += spl_cmd[2] + '/'
                cur_dir = cur_dir.get_dir(spl_cmd[2])
    else:
        cur_dir.ls(command[1:])

while True:
    if cur_dir.parent is None:
        break
    cur_dir = cur_dir.parent

cur_dir.calc_sizes()
dirs = cur_dir.get_dirs()

print(f'Part 1: {sum([d.size for d in dirs if d.size < 100000])}')
total_size = 70000000
space_needed = 30000000
space_used = cur_dir.size
delete_size = space_needed - (total_size - space_used)
diff = total_size
deleting = 0
for d in dirs:
    if d.size > delete_size and d.size - delete_size < diff:
        diff = d.size - delete_size
        deleting = d.size

print(f'Part 2: {deleting}')

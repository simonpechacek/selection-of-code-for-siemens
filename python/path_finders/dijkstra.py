class Dijkstra(object):
    def __init__(self, wall_char, start_char, end_char):
        self.wall_char = wall_char
        self.start_char = start_char
        self.end_char = end_char
        self.width = 0
        self.height = 0
        self.que = []
        self.que_no_step = []
        self.closed = []
        self.closed_no_step = []

    def find_path(self, maze: list):
        start_node, end_node = self.find_nodes(maze)
        self.que.append(end_node)
        solved = False
        path = []
        while len(self.que) > 0:
            current = self.que.pop(0)

            if (current[0], current[1]) == start_node:
                self.closed.append(current)
                solved = True
                break

            self.eval(current, maze)

            self.closed.append(current)
            self.closed_no_step.append((current[0], current[1]))
        if solved:
            path = self.backtrack(end_node)
        self.refresh()
        return path

    def eval(self, current, maze):
        step = current[2]
        neighbours = self.neighbours(current[0], current[1])
        for neighbour in neighbours:
            if (neighbour not in self.closed_no_step) and maze[neighbour[1]][neighbour[0]] != self.wall_char and neighbour not in self.que_no_step:
                self.que.append((neighbour[0], neighbour[1], step + 1))
                self.que_no_step.append(neighbour)

    def neighbours(self, x, y):
        coords = [(x, y - 1), (x + 1, y), (x, y + 1), (x - 1, y)]
        to_remove = []
        for coord in coords:
            if -1 < coord[0] < self.width and -1 < coord[1] < self.height:
                continue
            else:
                to_remove.append(coord)
        for rem in to_remove:
            coords.remove(rem)
        return coords

    def refresh(self):
        self.que.clear()
        self.que_no_step.clear()
        self.closed.clear()
        self.closed_no_step.clear()

    def backtrack(self, end_node):
        current = self.closed.pop()
        i = current[2]
        path = []
        while (current[0], current[1]) != (end_node[0], end_node[1]):
            neighbours = self.neighbours(current[0], current[1])

            for neighbour in neighbours:
                if (neighbour[0], neighbour[1], i - 1) in self.closed:
                    path.append(neighbour)
                    i -= 1
                    current = neighbour
                    break

        path.pop()
        return path

    def find_nodes(self, maze: list):
        self.height = len(maze)
        self.width = len(maze[0])
        start_node = tuple()
        end_node = tuple()
        for y, row in enumerate(maze):
            if len(row) != self.width:
                return False
            for x, char in enumerate(row):
                if char == self.start_char:
                    start_node = (x, y)
                elif char == self.end_char:
                    end_node = (x, y, 0)
        return start_node, end_node


"""
        TEST CODE
maze = [
    ["S", "", ""],
    ["", "#", ""],
    ["", "#", "E"]
]

path_finder = Dijkstra("#", "S", "E")
print(path_finder.find_path(maze))"""

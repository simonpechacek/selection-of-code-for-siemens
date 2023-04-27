class A_star_solver(object):
    def __init__(self, start_node_char, end_node_char, wall_char):
        self.came_from = {}
        self.start_node_char = start_node_char
        self.end_node_char = end_node_char
        self.wall_char = wall_char
        self.open_list = []
        self.width = 0
        self.height = 0
        self.maze = []
        self.f_vals = []
        self.g_vals = []
        self.run = True
        self.end = False
        self.path = []

    def init(self, maze):
        # all rows have to have the same width
        self.width = len(maze[0])
        self.height = len(maze)
        self.maze = maze
        self.f_vals = [[float("inf") for _ in range(self.width)] for _ in range(self.height)]
        self.g_vals = [[float("inf") for _ in range(self.width)] for _ in range(self.height)]

    def solve(self, maze, start_node, end_node):
        self.init(maze)
        self.open_list.append(start_node)
        self.g_vals[start_node[1]][start_node[0]] = 0
        self.f_vals[start_node[1]][start_node[0]] = self.shortest_path(start_node, end_node)

        while len(self.open_list) > 0 and not self.end:
            while not self.run:
                continue

            current = self.get_lowest_f()
            """
            for index in range(len(self.g_vals)):
                print(self.g_vals[index], end="        ")
                print(self.f_vals[index])
            print()
            print()"""
            if current == end_node:
                # return function to backtrack
                print("found")
                self.path = self.backtrack(tuple(current), self.came_from, tuple(start_node))
                return
            self.open_list.remove(current)
            neighbours = self.get_neighbours(current)

            for neighbour in neighbours:
                temp_gVal = self.g_vals[current[1]][current[0]] + self.disatnce(current, neighbour)

                if temp_gVal < self.g_vals[neighbour[1]][neighbour[0]]:
                    self.came_from.update({tuple(neighbour): tuple(current)})
                    self.g_vals[neighbour[1]][neighbour[0]] = temp_gVal
                    self.f_vals[neighbour[1]][neighbour[0]] = temp_gVal + self.shortest_path(neighbour, end_node)
                    if neighbour not in self.open_list:
                        self.open_list.append(neighbour)
            self.run = False
        print("not found")

    def disatnce(self, current, neighbour):
        if current[0] != neighbour[0] and current[1] != neighbour[1]:
            return 14
        else:
            return 10

    def backtrack(self, curr: tuple, came_from: dict, start_node: tuple):
        path = []
        current = came_from[curr]
        while current != start_node:
            path.append(current)
            current = came_from[current]
        return path

    def shortest_path(self, start_node, end_node):
        path_cost = 0

        dx = abs(end_node[0] - start_node[0])
        dy = abs(end_node[1] - start_node[1])

        diagonal = min(dx, dy)
        horizontal = max(dx, dy) - diagonal
        path_cost += 14 * diagonal
        path_cost += 10 * horizontal
        """if start_node == [1, 0]:
            print(start_node, "    ", end_node, end="  ")
            print("dx:", dx, "dy:", dy, "diag:", diagonal, "horizontal:",horizontal)
        """
        return path_cost

    def get_lowest_f(self):
        lowest = float("inf"), []
        for to_eval in self.open_list:
            if self.f_vals[to_eval[1]][to_eval[0]] <= lowest[0]:
                lowest = self.f_vals[to_eval[1]][to_eval[0]], to_eval
        return lowest[1]

    def get_neighbours(self, node):
        neighbours = []
        for i in range(-1, 2):
            for j in range(-1, 2):
                if (j == 0) and (i == 0):
                    continue
                x = node[0] + i
                y = node[1] + j
                if (-1 < x < self.width) and (-1 < y < self.height) and self.maze[y][x] != self.wall_char:
                    neighbours.append([x, y])
        return neighbours

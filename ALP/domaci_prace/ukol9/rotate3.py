"""
Solves for 3 rings
RED - 0 (index, dots colors)
GREEN - 1
BLUE - 2
"""
import copy
import queue


common_points = {
    0: {
        1: [[0, 0], [2, 2]],
        2: [[1, 1], [3, 5]]
    },
    1: {
        0: [[0, 0], [2, 2]],
        2: [[1, 0], [3, 2]]
    },
    2: {
        0: [[1, 1], [5, 3]],
        1: [[0, 1], [2, 3]]
    }
}

skip = {
    0: [0, 1],
    1: [2, 3],
    2: [0, 5]
}


class Rings(object):
    def __init__(self, red, green, blue, created):
        """
        :param red: list of dots on red ring
        :param green: list of dots on green ring
        :param blue: list of dots on blue ring
        :param created: tuple of how this state was created (rotated ring index, "direction of rotation")
        """
        self.state = [red, green, blue]
        self.created = created

    def nextRot(self):
        """
        Rotates each ring by 1 position to the left and to the right
        P -> 0 => 1  - pop() -> insert(0)
        M -> 0 => 5  - pop(0) -> append()
        :return:
        """
        # RED 0 = GREEN 0, RED 2 = GREEN 2
        # RED 1 = BLUE 1, RED 3 = BLUE 5
        # GREEN 1 = BLUE 0, GREEN 3 = BLUE 2
        nextStates = []
        for i in range(len(self.state)):
            for rot in ["p", "m"]:
                rotations = copy.deepcopy(self.state)
                if rot == "p":
                    rotations[i].insert(0, rotations[i].pop())

                elif rot == "m":
                    rotations[i].append(rotations[i].pop(0))

                for common_point in common_points[i]:
                    for point in common_points[i][common_point]:
                        rotations[common_point][point[1]] = rotations[i][point[0]]
                nextStates.append(Rings(rotations[0], rotations[1], rotations[2], (i, rot)))
        return nextStates

    def isSolution(self):
        """
        determines if state is final solution
        state[0] - all 0 except 0 and 1
        state[1] - all 1 except 2 and 3
        state[2] - all 2 except 0 and 5
        :return: True/False
        """
        for i in range(len(self.state)):
            for j in range(len(self.state[i])):
                if j not in skip[i]:
                    if self.state[i][j] != i:
                        return False
        return True

    def __hash__(self):
        return hash(tuple(map(tuple, self.state)))

    def __eq__(self, other):
        return tuple(map(tuple, self.state)) == tuple(map(tuple, other.state))

    def __str__(self):
        return "".join(f"({self.created[0]}, {self.created[1]})") if self.created else "None"


def load_input():
    rings = []
    for _ in range(3):
        rings.append(list(map(int, input().split())))
    return rings


def constructPath(start, visited):
    path = []
    curr = start
    while curr.created is not None:
        path.append(curr)
        curr = visited[curr]

    for move in path[::-1]:
        print(str(move), end="")


def main():
    init_ring = load_input()
    ring = Rings(init_ring[0], init_ring[1], init_ring[2], None)
    q = queue.Queue()
    q.put([ring, None])
    #stack = [[ring, None]]

    visited = {}
    while not q.empty() > 0:
        curr, prev = q.get()
        if curr not in visited:
            visited[curr] = prev
            #print(len(stack))
            if curr.isSolution():
                #print("SOLUTION")
                constructPath(curr, visited)
                return
            else:
                for next_state in curr.nextRot():
                    q.put([next_state, curr])


if __name__ == "__main__":
    """a = Rings([2, 0, 1, 2, 1, 1], [2, 2, 1, 0, 0, 1], [2, 0, 0, 0, 2, 2], None)
    b = Rings([2, 0, 1, 2, 1, 1], [2, 2, 1, 0, 0, 1], [2, 0, 0, 0, 2, 2], (1, "m"))
    print(a.__hash__() == b.__hash__())
    quit()"""
    main()

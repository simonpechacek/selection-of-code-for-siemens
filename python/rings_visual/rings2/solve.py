"""
This program finds shortest "path" - of how to rotate two rings
in such a way that each ring consist only of dots of same color as the ring itself

red dots, red ring - 0
green dots, green ring - 1

input - first red ring, second green ring

"""


class Rings(object):
    def __init__(self, redRing, greenRing, rotation):
        """
        :param redRing: current state of red ring
        :param greenRing: current state of green ring
        :param rotation: rotation from which this state was created (index, "dir")
        """
        self.state = [redRing, greenRing]
        self.rotation = rotation

    def nextRot(self):
        """
        rotates both rings in both directions
        :return: 4 objects of type Rings - rotated
        """

        rotations = []

        # RED P - direction (UP) =>
        # print("before RED P - rotation: ", self.state[0], self.state[1])
        tmp1 = self.state[0][:]
        tmp1.insert(0, tmp1.pop())
        tmp2 = self.state[1][:]
        tmp2[0], tmp2[2] = tmp1[0], tmp1[2]
        rotations.append(Rings(tmp1, tmp2, (0, "p")))
        # print("after RED P - rotation: ", tmp1, tmp2)

        # RED M - direction (DOWN) <=
        tmp1 = self.state[0][:]
        tmp1.append(tmp1.pop(0))
        tmp2 = self.state[1][:]
        tmp2[0], tmp2[2] = tmp1[0], tmp1[2]
        rotations.append(Rings(tmp1, tmp2, (0, "m")))

        # GREEN P - direction (UP) =>
        tmp1 = self.state[1][:]
        tmp1.insert(0, tmp1.pop())
        tmp2 = self.state[0][:]
        tmp2[0], tmp2[2] = tmp1[0], tmp1[2]
        rotations.append(Rings(tmp2, tmp1, (1, "p")))

        # GREEN M - direction (DOWN) <=
        tmp1 = self.state[1][:]
        tmp1.append(tmp1.pop(0))
        tmp2 = self.state[0][:]
        tmp2[0], tmp2[2] = tmp1[0], tmp1[2]
        rotations.append(Rings(tmp2, tmp1, (1, "m")))

        return rotations

    def is_solution(self):
        """
        determines if ring configuration is solution
        :return: True/False
        """
        if self.state[0][0] != 1 or self.state[1][2] != 0:
            return False

        for i in range(1, len(self.state[0])):
            if i == 2:
                continue
            if self.state[0][i] != 0 or self.state[1][i] != 1:
                return False
        return True

    def __eq__(self, a):
        return self.state == a.state

    def __str__(self):
        return "".join(f"({self.rotation[0]},{self.rotation[1]})")

    def __hash__(self):
        return hash((frozenset(self.state[0]), frozenset(self.state[1])))


class Solve(object):
    def __init__(self):
        self.get_start()

    def get_start(self):
        rings = []
        with open("ring.txt", "r") as f:
            for line in f.readlines():
                rings.append((list(map(int, line.split()))))

        return rings[0], rings[1]

    def construct_path(self, visited, final):
        curr = final
        path = []
        path_f = []
        while curr.rotation is not None:
            path.append(curr)
            curr = visited[curr]
        for p in path[::-1]:
            path_f.append(p.rotation)
        return path_f

    def main(self):
        stack = []
        visited = {}
        red, green = self.get_start()
        stack.append([Rings(red, green, None), None])

        while len(stack) > 0:
            state, prev = stack.pop(0)
            """if state.rotation is not None:
                print(str(state))"""
            if state not in visited:
                visited[state] = prev

                if state.is_solution():
                    # print("SOLUTION")
                    return self.construct_path(visited, state)

                for rot in state.nextRot():
                    stack.append([rot, state])


"""
ring = Rings([1, 0, 0, 0, 0, 0], [1, 1, 0, 1, 1, 1], (2, "c"))
ring2 = Rings([1, 0, 1, 1, 0, 0], [1, 1, 1, 0, 1, 1], (0, "m"))
# ring.nextRot()
print(ring.is_solution())
"""
"""states = {ring: ring2}
print(ring2 in states.values())
print(states)"""

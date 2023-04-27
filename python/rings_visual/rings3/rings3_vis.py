from rotate3 import Solve
import pygame

solver = Solve()

# print(solver.main())

# init pygame
pygame.init()

# WINDOW SIZE
WIN_SIZE = (550, 530)

center_red = [200, 200]
center_green = [350, 200]
center_blue = [275, 330]
diameter = 150
# creates window surface
win = pygame.display.set_mode(WIN_SIZE)

cos2 = 0.9998477
sin2 = 0.01745241

cos60 = 0.5
sin60 = 0.8660254

colors = {
    0: "#ff6b6b",
    1: "#10ac84",
    2: "#54a0ff"
}
rotations = {
    0: {
        "m": -1,
        "p": 1
    },
    1: {
        "m": 1,
        "p": -1
    },
    2: {
        "m": -1,
        "p": 1
    }
}

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


class Point(object):
    def __init__(self, x, y, center, color):
        self.x = x
        self.y = y
        self.center = center
        self.color = color

    def rotate(self, dir):
        """
        rotates point by 2 degrees around center[x, y]
        :return: None
        """
        x = self.x - self.center[0]
        y = self.y - self.center[1]
        self.x = self.center[0] + (x * cos2 - y * (sin2 * dir))
        self.y = self.center[1] + (x * (sin2 * dir) + y * cos2)

    def draw(self, win):
        pygame.draw.circle(win, self.color, [self.x, self.y], 5)
        pygame.draw.circle(win, "black", [self.x, self.y], 7, width=2)


class Ring(object):
    def __init__(self, points, center, index):
        self.points = points
        self.center = center
        self.index = index
        self.rot_progress = 0
        self.done = True
        self.dir = None

    def turn(self, ring1, ring2):

        if self.dir == "m":
            self.points.append(self.points.pop(0))
        elif self.dir == "p":
            self.points.insert(0, self.points.pop())

        common_point1, common_point2 = common_points[self.index][ring1.index]
        ring1.points[common_point1[1]] = self.points[common_point1[0]]
        ring1.points[common_point2[1]] = self.points[common_point2[0]]
        common_point1, common_point2 = common_points[self.index][ring2.index]
        ring2.points[common_point1[1]] = self.points[common_point1[0]]
        ring2.points[common_point2[1]] = self.points[common_point2[0]]

    def update(self, ring1, ring2):
        for point in self.points:
            point.rotate(rotations[self.index][self.dir])

        self.rot_progress += 1

        if self.rot_progress > 59:
            self.rot_progress = 0
            self.done = True
            self.turn(ring1, ring2)

    def first_call(self, dir):
        for point in self.points:
            point.center = self.center

        self.dir = dir


def draw_base_shapes():
    pygame.draw.circle(win, colors[0], center_red, diameter + 10, width=20)
    pygame.draw.circle(win, colors[1], center_green, diameter + 10, width=20)
    pygame.draw.circle(win, colors[2], center_blue, diameter + 10, width=20)


def draw(r, g, b):
    win.fill("#48dbfb")
    draw_base_shapes()
    for point in r:
        point.draw(win)
    for point in g:
        point.draw(win)
    for point in b:
        point.draw(win)
    pygame.display.flip()


def rotate_by_60(x, y, center, dir):
    """ dir = 1 - clockwise, -1 counterclockwise"""
    tmp_x = x - center[0]
    tmp_y = y - center[1]
    x = center[0] + (tmp_x * cos60 - tmp_y * (sin60 * dir))
    y = center[1] + (tmp_x * (sin60 * dir) + tmp_y * cos60)
    return x, y


def load_input():
    rings = []
    with open("ring.txt", "r") as f:
        for line in f.readlines():
            ring = list(map(int, line.split()))
            rings.append(ring)
    return rings


def construct_points(points):
    redRing = points[0]
    greenRing = points[1]
    blueRing = points[2]
    x, y = rotate_by_60(350, 200, center_red, -1)
    r_ring = []
    for point in redRing:
        r_ring.append(Point(x, y, center_red, colors[point]))
        x, y = rotate_by_60(x, y, center_red, 1)

    g_ring = [0] * len(greenRing)
    x, y = rotate_by_60(200, 200, center_green, 1)
    for i, point in enumerate(greenRing):
        if i in (0, 2):
            g_ring[i] = r_ring[i]
        else:
            g_ring[i] = Point(x, y, center_green, colors[point])
        x, y = rotate_by_60(x, y, center_green, -1)

    b_ring = [0] * len(greenRing)
    b_ring[0] = g_ring[1]
    x, y = b_ring[0].x, b_ring[0].y
    for i in range(1, len(blueRing)):
        x, y = rotate_by_60(x, y, center_blue, 1)
        if i == 1:
            b_ring[1] = r_ring[1]
        elif i == 2:
            b_ring[2] = g_ring[3]
        elif i == 5:
            b_ring[5] = r_ring[3]
        else:
            b_ring[i] = Point(x, y, center_blue, colors[blueRing[i]])
    return r_ring, g_ring, b_ring


def main():
    run = True
    clock = pygame.time.Clock()
    points = load_input()
    r_ring, g_ring, b_ring = construct_points(points)
    draw(r_ring, g_ring, b_ring)
    solution = solver.main()
    redRing = Ring(r_ring, center_red, 0)
    greenRing = Ring(g_ring, center_green, 1)
    blueRing = Ring(b_ring, center_blue, 2)
    current = None
    otherPoints = None
    while run:
        clock.tick(30)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

        draw(r_ring, g_ring, b_ring)
        if redRing.done and greenRing.done and blueRing.done:
            if len(solution) > 0:
                sol = solution.pop(0)
                if sol[0] == 1:
                    greenRing.first_call(sol[1])
                    current = greenRing
                    otherPoints = [redRing, blueRing]
                    greenRing.done = False
                elif sol[0] == 0:
                    redRing.first_call(sol[1])
                    current = redRing
                    otherPoints = [greenRing, blueRing]
                    redRing.done = False
                else:
                    blueRing.first_call(sol[1])
                    current = blueRing
                    otherPoints = [redRing, greenRing]
                    blueRing.done = False
        else:
            current.update(otherPoints[0], otherPoints[1])


main()

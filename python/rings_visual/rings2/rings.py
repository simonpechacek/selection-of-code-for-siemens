import pygame
import solve

# init pygame
pygame.init()

# WINDOW SIZE
WIN_SIZE = (700, 600)

center_red = [250, 300]
center_green = [450, 300]
diameter = 200
# creates window surface
win = pygame.display.set_mode(WIN_SIZE)

cos2 = 0.9998477
sin2 = 0.01745241

cos60 = 0.5
sin60 = 0.8660254


colors = {
    0: "#ff6b6b",
    1: "#10ac84"
}
rotations = {
    0: {
        "m": -1,
        "p": 1
    },
    1: {
        "m": 1,
        "p": -1
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
        pygame.draw.circle(win, self.color, [self.x, self.y], 10)
        pygame.draw.circle(win, "black", [self.x, self.y], 13, width=3)


class Ring(object):
    def __init__(self, points, center, index):
        self.points = points
        self.center = center
        self.index = index
        self.rot_progress = 0
        self.done = True
        self.dir = None

    def turn(self, other_points):
        """
        :param dir:
        :return:
        """

        if self.dir == "m":
            first = self.points.pop(0)
            self.points.append(first)
            other_points[0] = self.points[0]
            other_points[2] = self.points[2]
        elif self.dir == "p":
            last = self.points.pop()
            self.points.insert(0, last)
            other_points[0] = self.points[0]
            other_points[2] = self.points[2]

    def update(self, other_points):
        for point in self.points:
            point.rotate(rotations[self.index][self.dir])
        self.rot_progress += 1

        if self.rot_progress > 59:
            self.rot_progress = 0
            self.done = True
            self.turn(other_points)

    def first_call(self, dir):

        for point in self.points:
            point.center = self.center

        self.dir = dir


# draw function - fills screen with color and updates it
def draw(r_ring, g_ring):
    win.fill("#48dbfb")
    draw_base_shapes()
    for point in r_ring:
        point.draw(win)
    for point in g_ring:
        point.draw(win)
    pygame.display.flip()


def draw_base_shapes():
    """pygame.draw.line(win, "black", (0, 300), (600, 300), 5)
    pygame.draw.line(win, "black", (200, 290), (200, 310))
    pygame.draw.line(win, "black", (300, 290), (300, 310))
    pygame.draw.line(win, "black", (400, 290), (400, 310))"""
    pygame.draw.circle(win, "black", center_green, diameter + 17, width=2)
    pygame.draw.circle(win, "black", center_green, diameter + -15, width=2)
    pygame.draw.circle(win, "black", center_red, diameter + 17, width=2)
    pygame.draw.circle(win, "black", center_red, diameter + -15, width=2)
    pygame.draw.circle(win, colors[0], center_red, diameter + 15, width=30)
    pygame.draw.circle(win, colors[1], center_green, diameter + 15, width=30)


def construct_points(input):
    """
    0 and 2 - same for red and green
    """
    redRing = input[0]
    greenRing = input[1]
    x, y = rotate_by_60(450, 300, center_red, -1)
    r_ring = []
    for point in redRing:
        r_ring.append(Point(x, y, center_red, colors[point]))
        x, y = rotate_by_60(x, y, center_red, 1)
    g_ring = [0] * len(greenRing)
    x, y = rotate_by_60(250, 300, center_green, 1)
    for i, point in enumerate(greenRing):
        if i in (0, 2):
            g_ring[i] = r_ring[i]
        else:
            g_ring[i] = Point(x, y, center_green, colors[point])
        x, y = rotate_by_60(x, y, center_green, -1)
    return r_ring, g_ring


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


# mian loop
def main(solution):
    run = True
    clock = pygame.time.Clock()
    points = load_input()
    r_ring, g_ring = construct_points(points)
    redRing = Ring(r_ring, center_red, 0)
    greenRing = Ring(g_ring, center_green, 1)
    current = None
    otherPoints = None
    while run:
        clock.tick(30)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
        draw(r_ring, g_ring)

        if redRing.done and greenRing.done:
            if len(solution) > 0:
                sol = solution.pop(0)
                if sol[0] == 1:
                    greenRing.first_call(sol[1])
                    current = greenRing
                    otherPoints = redRing.points
                    greenRing.done = False
                else:
                    redRing.first_call(sol[1])
                    current = redRing
                    otherPoints = greenRing.points
                    redRing.done = False
        else:
            current.update(otherPoints)


if __name__ == "__main__":
    slv = solve.Solve()
    main(slv.main())

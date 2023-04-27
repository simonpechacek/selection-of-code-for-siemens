import random
import pygame
from snake_AI import Snake
from dijkstra import Dijkstra

# import sys

# init pygame
pygame.init()

# WINDOW SIZE
WIN_SIZE = (600, 600)
tile_size = 20
win_len = WIN_SIZE[0] // tile_size
# creates window surface
win = pygame.display.set_mode(WIN_SIZE)

bg_colors = ["#576574", "#8395a7"]

snake = Snake(tile_size, win_len)
_SNAKE_CHAR = "~"
_APPLE_CHAR = "@"
_HEAD_CHAR = "*"
path_finder = Dijkstra(_SNAKE_CHAR, _HEAD_CHAR, _APPLE_CHAR)


class Apple(object):
    def __init__(self, apple_char):
        self.char = apple_char
        self.pos = []
        self.update()

    def update(self, head=(0, 0), body=()):
        while True:
            x, y = random.randint(0, win_len - 1), random.randint(0, win_len - 1)
            if (x, y) != head and [x, y] not in body:
                break
        self.pos = [x, y]

    def draw(self):
        pygame.draw.rect(win, "#ee5253", (self.pos[0] * tile_size, self.pos[1] * tile_size, tile_size, tile_size))

    def collided(self, snake_head):
        if self.pos == snake_head:
            return True
        else:
            return False


class Path(object):
    def __init__(self):
        self.path = []


# draw function - fills screen with color and updates it
def draw():
    """for i in range(win_len):
        for j in range(win_len):
            pygame.draw.rect(win, bg_colors[(i + j) % 2], (j * tile_size, i * tile_size, tile_size, tile_size))
    """
    win.fill("#48dbfb")
    apple.draw()
    snake.draw(win)
    pygame.display.flip()


def get_dir(head):
    first = path_obj.path.pop(0)
    if head[0] < first[0]:
        return "RIGHT"
    elif head[0] > first[0]:
        return "LEFT"
    else:
        if head[1] < first[1]:
            return "DOWN"
        else:
            return "UP"


path_obj = Path()


def update():
    maze = [["" for _ in range(win_len)] for _ in range(win_len)]
    maze[apple.pos[1]][apple.pos[0]] = apple.char
    head, body = snake.get_pos()

    maze[head[1]][head[0]] = _HEAD_CHAR

    for bod in snake.body:
        maze[bod[1]][bod[0]] = _SNAKE_CHAR

    if apple.collided(head):
        snake.get_longer()
        apple.update(head, snake.body)
        maze[apple.pos[1]][apple.pos[0]] = apple.char
        path_obj.path = path_finder.find_path(maze)

    if not path_obj.path:
        path_obj.path = path_finder.find_path(maze)

    try:
        coll = snake.update(get_dir(head))
    except IndexError:
        coll = snake.update(random.choice(["UP", "DOWN", "LEFT", "RIGHT"]))
        # print("No viable path exists")
        # sys.exit()
    return coll


apple = Apple(_APPLE_CHAR)


# main loop
def main():
    run = True
    clock = pygame.time.Clock()
    # update()
    while run:
        clock.tick(20)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
        if update():
            run = False
        draw()


if __name__ == "__main__":
    main()

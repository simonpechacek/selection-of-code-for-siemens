import pygame
from dijkstra import Dijkstra
from animator import AnimatePath
import random
# init pygame
pygame.init()

# WINDOW SIZE
WIN_SIZE = (1440, 800)

# creates window surface
win = pygame.display.set_mode(WIN_SIZE)

_START = "S"
_END = "E"
_WALL = "#"
weights = ["" for _ in range(10)]
weights[8], weights[9] = _WALL, _WALL
_TILE_SIZE = 80
count_w = 1440 // _TILE_SIZE
count_h = 800 // _TILE_SIZE
maze = [[random.choice(weights) for _ in range(count_w)] for _ in range(count_h)]
maze[0][0], maze[count_h - 1][count_w - 1] = _START, _END
"""
maze = [
    ["S", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""],
    ["#", "#", "#", "#", "#", "#", "#", "#", "#", "", "", "", "", "", "", "", "", ""],
    ["", "", "", "", "", "", "", "", "", "",  "", "", "", "", "", "", "", ""],
    ["", "#", "#", "#", "#", "#", "#", "#", "#", "#",  "", "", "", "", "", "", "", ""],
    ["", "", "", "", "", "", "", "", "", "",  "", "", "", "", "", "", "", ""],
    ["#", "#", "#", "#", "#", "#", "#", "#", "#", "",  "", "", "", "", "", "", "", ""],
    ["", "", "", "", "", "", "", "", "", "",  "", "", "", "", "", "", "", ""],
    ["", "#", "#", "#", "#", "#", "#", "#", "#", "#",  "", "", "", "", "", "", "", ""],
    ["", "", "", "", "", "", "", "", "", "",  "", "", "", "", "", "", "", ""],
    ["#", "#", "#", "#", "#", "#", "#", "#", "#", "",  "", "", "", "", "", "", "", "E"],
]
"""
colors = {
    "": "#48dbfb",
    _START: "#1dd1a1",
    _END: "#ff6b6b",
    _WALL: "#576574"
}

path_finder = Dijkstra(_WALL, _START, _END)


# draw function - fills screen with color and updates it
def draw(animator):
    win.fill((255, 255, 255))
    for y, row in enumerate(maze):
        for x, char in enumerate(row):
            pygame.draw.rect(win, colors[char], (x * _TILE_SIZE, y * _TILE_SIZE, _TILE_SIZE, _TILE_SIZE))
    """for p in path:
        pygame.draw.rect(win, "#feca57", (p[0] * _TILE_SIZE, p[1] * _TILE_SIZE, _TILE_SIZE, _TILE_SIZE))
    """
    animator.draw(win)
    pygame.display.flip()


def update(x, y, tile):
    if tile == "":
        if maze[y][x] == _WALL:
            maze[y][x] = ""
        elif maze[y][x] == "":
            maze[y][x] = _WALL
    else:
        maze[y][x] = tile
    path = path_finder.find_path(maze)
    return path


# main loop
def main():
    run = True
    clock = pygame.time.Clock()
    path = path_finder.find_path(maze)
    animator = AnimatePath(path, _TILE_SIZE)
    draw(animator)
    tile = ""
    while run:
        clock.tick(60)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

            mouse = pygame.mouse.get_pos()
            x, y = mouse[0] // _TILE_SIZE, mouse[1] // _TILE_SIZE

            if event.type == pygame.MOUSEBUTTONDOWN:
                if maze[y][x] == _START:
                    maze[y][x] = ""
                    tile = _START
                elif maze[y][x] == _END:
                    maze[y][x] = ""
                    tile = _END
                else:
                    tile = ""

            if event.type == pygame.MOUSEBUTTONUP:

                path = update(x, y, tile)
                animator.update(path)

        draw(animator)


if __name__ == "__main__":
    main()

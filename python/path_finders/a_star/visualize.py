import pygame
from a_star import A_star_solver
import threading

# init pygame
pygame.init()

# WINDOW SIZE
WIN_SIZE = (1000, 700)
TILE_SIZE = 20
MAZE_WIDTH = 40  # WIN_SIZE[0] // TILE_SIZE
MAZE_HEIGHT = 30  # WIN_SIZE[1] // TILE_SIZE

# creates window surface
win = pygame.display.set_mode(WIN_SIZE)

maze = [["" for _ in range(MAZE_WIDTH)] for _ in range(MAZE_HEIGHT)]
maze[0][0] = "s"
maze[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = "e"

finder = A_star_solver("s", "e", "#")

colors = {
    "s": "#63cdda",
    "e": "#cf6a87",
    "#": "#786fa6"
}

for i in range(1, MAZE_HEIGHT):
    maze[i][10] = "#"



# draw function - fills screen with color and updates it
def draw():
    win.fill("#f3a683")
    for y, row in enumerate(maze):
        for x, tile in enumerate(row):
            if tile == "":
                continue
            pygame.draw.rect(win, colors[tile], (x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE))

    for evaled in list(finder.came_from.keys()):
        pygame.draw.rect(win, "#546de5", (evaled[0] * TILE_SIZE, evaled[1] * TILE_SIZE, TILE_SIZE, TILE_SIZE))

    for path_block in finder.path:
        pygame.draw.rect(win, "#c44569", (path_block[0] * TILE_SIZE, path_block[1] * TILE_SIZE, TILE_SIZE, TILE_SIZE))

    pygame.display.flip()


# main loop
def main():
    run = True
    clock = pygame.time.Clock()
    threading.Thread(target=finder.solve, args=(maze, [0, 0], [MAZE_WIDTH - 1, MAZE_HEIGHT - 1])).start()
    while run:
        clock.tick(60)
        finder.run = True
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
                finder.end = True
                finder.run = True

        draw()


if __name__ == "__main__":
    main()

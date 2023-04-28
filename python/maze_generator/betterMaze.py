import pygame
import time
import random

pygame.init()

width = 880
height = 880

win = pygame.display.set_mode((width, height))
pygame.display.set_caption("Maze generator")
colors = {
   "grey": (48, 48, 48),
   "white": (180, 180, 180),
   "red": (255, 46, 89),
   "light": (80, 80, 80)
}
win.fill(colors["grey"])
pygame.display.update()

x = 0
y = 0
cell_size = 40
grid = []
stack_list = []
count_list = []
solution = []

def create_grid(x, y, cell_size = cell_size):
    for i in range(20):
        y += cell_size
        x = cell_size
        for j in range(20):
            pygame.draw.line(win, colors["white"], [x, y], [x + cell_size, y], 4) #horni
            pygame.draw.line(win, colors["white"], [x + cell_size, y], [x + cell_size, y + cell_size], 4) #prava
            pygame.draw.line(win, colors["white"], [x, y], [x, y + cell_size], 4) #leva
            pygame.draw.line(win, colors["white"], [x, y + cell_size], [x + cell_size, y+cell_size], 4) #dolni
            pygame.display.update()
            grid.append((x, y))
            x += cell_size

def draw_head(x, y):
    pygame.draw.rect(win, colors["red"], (x+2, y+2, 36, 36), 0)
    pygame.display.update()


def draw_back_head(x, y):
    pygame.draw.rect(win, colors["light"], (x+2, y+2, 36, 36), 0)


def horni_stena(x, y):
    pygame.draw.rect(win, colors["light"], (x + 2, y - cell_size + 2, 37, 76))
    pygame.display.update()


def dolni_stena(x, y):
    pygame.draw.rect(win, colors["light"], (x+2, y+2, 37, 77))
    pygame.display.update()


def leva_stena(x, y):
    pygame.draw.rect(win, colors["light"], (x - cell_size +2, y+2, 77, 37))
    pygame.display.update()


def prava_stena(x, y):
    pygame.draw.rect(win, colors["light"], (x+2, y+2, 77, 37))
    pygame.display.update()


def draw_solution():
    for move in solution:
        pygame.draw.rect(win, (0, 0, 255), (move[0]+10, move[1] + 10, 15, 15))
        pygame.display.update()


def create_maze(x, y, cell_size = cell_size):
    count_list.append((x, y))
    stack_list.append((x, y))
    draw_head(x, y)
    run = True
    while run and len(stack_list) > 0:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
        volba = []
        if (x + cell_size, y) not in count_list and (x + cell_size, y) in grid:
            volba.append("prava")
        if (x - cell_size, y) not in count_list and (x - cell_size, y) in grid:
            volba.append("leva")
        if (x, y + cell_size) not in count_list and (x, y + cell_size) in grid:
            volba.append("dolu")
        if (x, y - cell_size) not in count_list and (x, y - cell_size) in grid:
            volba.append("nahoru")
        if len(volba) > 0:
            smer = random.choice(volba)
            if smer == "prava":
                prava_stena(x, y)
                x += cell_size
                count_list.append((x, y))
                stack_list.append((x, y))
                draw_head(x, y)
            elif smer == "leva":
                leva_stena(x, y)
                x -= cell_size
                count_list.append((x, y))
                stack_list.append((x, y))
                draw_head(x, y)
            elif smer == "dolu":
                dolni_stena(x, y)
                y += cell_size
                count_list.append((x, y))
                stack_list.append((x, y))
                draw_head(x, y)
            elif smer == "nahoru":
                horni_stena(x, y)
                y -= cell_size
                count_list.append((x, y))
                stack_list.append((x, y))
                draw_head(x, y)
            solution.clear()
            time.sleep(0.01)
        else:
            time.sleep(0.01)
            x, y = stack_list.pop()
            draw_head(x, y)
            draw_back_head(x ,y)
            solution.append((x, y))


create_grid(x, y, cell_size)
x = 40
y = 40
create_maze(x, y, cell_size)
#draw_solution()
run = True


class game(object):
    def __init__(self):
        self.started = False
        self.text_drawn = False
        self.game_won = False
        self.time_start = None
        self.time_end = None
        self.prev_time = 0
        self.win_time = 0


hra = game()

font = pygame.font.SysFont("Arial", 15, True)
text = font.render("Place mouse on red square and click to start", True, (255, 255, 255))
text2 = font.render("You lost, place mouse on red square and click", True, (255, 255, 255))
text3 = font.render("You WON!", True, (255, 255, 255))


def draw():
    if not hra.text_drawn:
        pygame.draw.rect(win, colors["grey"], (0, 0, 880, 39))
        hra.text_drawn = True
    pygame.draw.rect(win, colors["red"], (40 + 1, 40 + 1, 38, 38))
    pygame.draw.rect(win, (0, 0, 255), (800 + 1, 800 + 1, 38, 38))
    if not hra.started:
        win.blit(text2, (width/2 - text2.get_width()/2, 10))
        hra.text_drawn = False
        hra.time_end = None
        hra.time_start = None
    if hra.game_won:
        win.blit(text3, (width/2 - text3.get_width()/2, 10))
        hra.prev_time = hra.win_time
        hra.win_time = round(hra.time_end - hra.time_start, 3)
        text4 = font.render(str(hra.win_time), True, (255, 255, 255))
        win.blit(text4, (width - text2.get_width()/2 - 10, 10))
        text5 = font.render(str(hra.prev_time), True, (255, 255, 255))
        win.blit(text5, (10, 10))
        hra.time_end = None
        hra.time_start = None
        hra.started = False
        hra.text_drawn = False
    pygame.display.update()

win.blit(text, (width/2 - text.get_width()/2, 10))
pygame.draw.rect(win, colors["red"], (40 + 1, 40 + 1, 38, 38))
pygame.draw.rect(win, (0, 0, 255), (800 + 1, 800 + 1, 38, 38))
pygame.display.update()

clock = pygame.time.Clock()


def check_pos(col_x, col_y):
    color = win.get_at((col_x, col_y))
    if color == colors["white"]:
        hra.started = False
    if 800 < col_x < 840 and 800 < col_y < 880:
        hra.game_won = True
        hra.time_end = time.time()


while run:
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.MOUSEBUTTONUP:
            if 40 < mouse[0] < 80 and 40 < mouse[1] < 80 and not hra.started:
                hra.started = True
                hra.game_won = False
                hra.time_start = time.time()
    mouse = pygame.mouse.get_pos()
    if hra.started:
        check_pos(mouse[0], mouse[1])
        draw()

pygame.quit()

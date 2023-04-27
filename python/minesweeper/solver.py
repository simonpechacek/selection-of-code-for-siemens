import pygame
import random
from mine_sweeper import mineSweeper
import time
import sys

tileSize = 40
mapSize = 10
numBombs = 10
button_size = 80
win_size = mapSize*tileSize

pygame.init()
win = pygame.display.set_mode((win_size, win_size))
pygame.display.set_caption("minesweeper")

notSucces = 0

game = mineSweeper(mapSize, tileSize, numBombs)

run = True


availableClicks = []
availableMarks = []
markedTiles = []

tries = 0

def botClick(x, y):
    if len(game.game) == 0:
        forbiden_tiles = game.get_neighbours_pos(x, y)
        forbiden_tiles.append([x, y])
        game.make_map(forbiden_tiles)
                    
    end = game.click(x, y)
    if end:
        print("You Lost")
        
    game.draw(win)

def botMark(x, y):
    game.mark_bomb(x, y)            
    game.draw(win)
    
def botMove():
    global tries
    global notSucces
    if len(availableMarks)>0:
        mark = availableMarks.pop()
        markedTiles.append(mark)
        botMark(mark[0], mark[1])
        
    elif len(availableClicks)>0:
        click = availableClicks.pop()
        botClick(click[0], click[1])
        
    else:
        if tries % 2 == 0:
            #save_marks(get_marks())
            finds = save_clicks(get_clicks())
            if not finds:
                notSucces +=1
            else:
                notSucces=0
            
        else:
            finds = save_marks(get_marks())   
            if not finds:
                notSucces+=1
            else:
                notSucces=0
        
        if notSucces > 6:
            botClick(random.randint(0, mapSize-1), random.randint(0, mapSize-1))
        
        tries+=1    

def get_marks():
    marks = []
    for y in range(mapSize):
        for x in range(mapSize):
            if game.game[y][x][1] == 0:
                continue
            elif game.game[y][x][1] == 1:
                neighbours = game.get_neighbours_pos(x, y)
                to_remove = []
                marked = 0
                for neigh in neighbours:
                    if game.game[neigh[1]][neigh[0]][1] == 1:
                        to_remove.append(neigh)
                    elif neigh in game.marked_bombs:
                        to_remove.append(neigh)
                        marked+=1
                #print(len(neighbours), end=" ")
                for rem in to_remove:
                    neighbours.remove(rem)
                #print(len(neighbours))
                if len(neighbours) == game.game[y][x][0] - marked:
                    for mark in neighbours:
                        marks.append(mark)
    return marks

def save_marks(marks):
    for mark in marks:
        if mark not in availableMarks and mark not in markedTiles:
            availableMarks.append(mark)
    return len(marks)

def get_clicks():
    clicks = []
    for y in range(mapSize):
        for x in range(mapSize):
            if game.game[y][x][1] == 0 or game.game[y][x][0] == 0 or [x, y] in game.marked_bombs:
                continue
            else:
                neigbours = game.get_neighbours_pos(x, y)
                marked = 0
                to_remove = []
                for neighb in neigbours:
                    if game.game[neighb[1]][neighb[0]][1] == 1:
                        to_remove.append(neighb)
                    elif neighb in game.marked_bombs:
                        marked +=1
                        to_remove.append(neighb)
                
                for rem in to_remove:
                    neigbours.remove(rem)
                if marked == game.game[y][x][0]:
                    for click in neigbours:
                        clicks.append(click)
    
    return clicks

buttonBot = pygame.Rect(win_size/4 - button_size/2, win_size/2 - button_size/2, button_size, button_size)   
buttonMe = pygame.Rect(3* win_size/4 - button_size/2, win_size/2 - button_size/2, button_size, button_size)
            
def save_clicks(clicks):
    for click in clicks:
        if click not in availableClicks:
            availableClicks.append(click)
    return len(clicks)

def draw_menu():
    win.fill("#2ecc71")
    
    pygame.draw.rect(win, "#00a8ff", buttonMe)
    pygame.draw.rect(win, "#8c7ae6", buttonBot)
    
    pygame.display.update()

BOT = False
    

menu = True
while menu:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            menu = False
            sys.exit()
            
        if event.type == pygame.MOUSEBUTTONUP:
            mouse = pygame.mouse.get_pos()
            if buttonBot.collidepoint(mouse):
                BOT = True
                menu = False
            elif buttonMe.collidepoint(mouse):
                BOT = False
                menu = False
    
    draw_menu()
    

game.first_draw(win)               
if BOT:
    botClick(random.randint(0, mapSize-1), random.randint(0, mapSize-1))

    save_marks(get_marks())

while run:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
        if event.type == pygame.MOUSEBUTTONUP:
            pos = pygame.mouse.get_pos()
            x = pos[0] // tileSize
            y = pos[1] // tileSize
            if event.button == 1:
                if len(game.game) == 0:
                    forbiden_tiles = game.get_neighbours_pos(x, y)
                    forbiden_tiles.append([x, y])
                    game.make_map(forbiden_tiles)
                    
                end = game.click(x, y)
                if end:
                    print("You Lost")
                    run = False
            elif event.button == 3:
                game.mark_bomb(x, y)
                
            game.draw(win)
    if BOT:
        botMove()
        time.sleep(0.5)
    
    if len(game.marked_bombs) == numBombs:
        print("YOU WON")
        run = False
    
    
    



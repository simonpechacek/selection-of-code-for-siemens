import pygame
import random


class mineSweeper(object):
    def __init__(self, map_size, tile_size, num_of_bombs):
        self.map_size = map_size
        self.tile_size = tile_size
        self.num_of_bombs = num_of_bombs
        self.game = []
        self.marked_bombs = []
        self.marked_tiles = []
        
    def make_map(self, forbiden_tiles):
        for i in range(self.map_size):
            self.game.append([])
            for j in range(self.map_size):
                self.game[i].append("")
        bombs = self.generate_bombs(self.num_of_bombs, forbiden_tiles)

        for bomb in bombs:
            # [num of neigbours, state (2 == bomb)]
            self.game[bomb[1]][bomb[0]] = [0, 2]
            
        self.count_bombs()
            
    def generate_bombs(self, num_of_bombs, forbiden_tiles):
        bombs = []

        while len(bombs) < num_of_bombs:
            x, y = random.randint(0, self.map_size-1), random.randint(0, self.map_size-1)

            if [x, y] not in bombs and [x, y] not in forbiden_tiles:
                bombs.append([x, y])
        return bombs
            
    def get_neighbours(self, x, y):
        # "x, y" of cell we want to know neigbours of
        # size of self.game - for borders
        NEIGBOURS = []
        for i in range(-1, 2):
            for j in range(-1, 2):  
                if j == 0 and i == 0:
                    continue
                else:
                    k, l = x + i, y + j
                    if self.map_size > k >= 0 and self.map_size > l >= 0:
                        NEIGBOURS.append(self.game[l][k]) 
        return NEIGBOURS
    
    def get_neighbours_pos(self, x, y):
        NEIGBOURS = []
        for i in range(-1, 2):
            for j in range(-1, 2):
                if j == 0 and i == 0:
                    continue
                else:
                    k, l = x + i, y + j
                    if self.map_size > k >= 0 and self.map_size > l >= 0:
                        NEIGBOURS.append([k, l]) #[x, y]
        return NEIGBOURS
    
    def count_bombs(self):
        for y in range(self.map_size):
            for x in range(self.map_size):
                if self.game[y][x] == "":
                    neigbours = self.get_neighbours(x, y)
                    bombs = 0
                    for neigbour in neigbours:
                        if neigbour != "":
                            if neigbour[1] == 2:
                                bombs += 1
                    #sets number of bombs and state as (0 == covered)
                    self.game[y][x] = [bombs, 0] 
                    
    def uncover(self, x, y):
        
        TO_UNCOVER = []
        TO_UNCOVER.append([x, y])
        TO_SHOW = []
        while len(TO_UNCOVER)>0:
            pos = TO_UNCOVER.pop(0)
            neighbours = self.get_neighbours_pos(pos[0], pos[1])
            self.game[pos[1]][pos[0]] = [0, 1]

            for neighbour in neighbours:
                if self.game[neighbour[1]][neighbour[0]][0] == 0 and self.game[neighbour[1]][neighbour[0]][1] == 0:
                    TO_UNCOVER.append(neighbour)
                if self.game[neighbour[1]][neighbour[0]][1] == 0 and neighbour not in TO_UNCOVER and neighbour not in TO_SHOW:
                    TO_SHOW.append(neighbour)
        for neigh in TO_SHOW:
            self.game[neigh[1]][neigh[0]][1] = 1
    
    def click(self, x, y):
        if self.game[y][x][1] == 2:
            return True
            
        elif self.game[y][x][1] == 1:
            return False
        elif self.game[y][x][1] == 0:
            if self.game[y][x][0] == 0:
                self.uncover(x, y)
                
            else:
                self.game[y][x][1] = 1 
            return False

    def mark_bomb(self, x, y):
        if [x, y] not in self.marked_bombs and [x, y] not in self.marked_tiles:
            if len(self.marked_tiles) + len(self.marked_bombs) < self.num_of_bombs:
                if self.game[y][x][1] == 2:
                    self.marked_bombs.append([x, y])
                elif self.game[y][x][1] == 0:
                    self.marked_tiles.append([x, y])
        elif [x, y] in self.marked_tiles:
            self.marked_tiles.remove([x, y])
        elif [x, y] in self.marked_bombs:
            self.marked_bombs.remove([x, y])
    
    def draw_num(self, x, y, num, win):
        TILE_SIZE = self.tile_size
        if num == 1:
            pygame.draw.rect(win, (0, 0, 0), (x + TILE_SIZE/2 - 3, y + TILE_SIZE/2 - 3, 6, 6))
        elif num == 2:
            pygame.draw.rect(win, (0, 0, 0), (x + 10, y + 10, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x +TILE_SIZE-16, y+TILE_SIZE -16, 6, 6))
        elif num == 3:
            pygame.draw.rect(win, (0, 0, 0), (x + 10, y + 10, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x +TILE_SIZE-16, y+TILE_SIZE -16, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x + TILE_SIZE/2 - 3, y + TILE_SIZE/2 - 3, 6, 6))
        elif num == 4:
            pygame.draw.rect(win, (0, 0, 0), (x + 10, y + 10, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x + TILE_SIZE -16, y +TILE_SIZE-16, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x +TILE_SIZE- 16, y + 10, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x +10, y + TILE_SIZE-16, 6, 6))
        elif num == 5:    
            pygame.draw.rect(win, (0, 0, 0), (x + 10, y + 10, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x + TILE_SIZE -16, y +TILE_SIZE-16, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x +TILE_SIZE- 16, y + 10, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x +10, y + TILE_SIZE-16, 6, 6))
            pygame.draw.rect(win, (0, 0, 0), (x + TILE_SIZE/2 - 3, y + TILE_SIZE/2 - 3, 6, 6))
        else:
            print("bigger num")
            
    def draw(self, win):
        TILE_SIZE = self.tile_size
        win.fill("#bdc3c7")
        count = 0
        for i in range(0, self.map_size):
            for j in range(0, self.map_size):
                if [j, i] in self.marked_bombs or [j, i] in self.marked_tiles:
                    pygame.draw.rect(win, "#e74c3c", (j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE))
                    count+=1
                    continue
                if self.game[i][j][1] == 1:
                    if self.game[i][j][0] == 0:
                        if count%2 == 1:
                            pygame.draw.rect(win, "#95a5a6", (j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE))
                        count+=1
                        continue
                    else:
                        if count%2 == 1:
                            pygame.draw.rect(win, "#95a5a6", (j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE))
                        num = self.game[i][j][0]
                        self.draw_num(j * TILE_SIZE, i * TILE_SIZE, num, win)
                        count += 1
                        continue
                    
                if count%2 == 0:
                    pygame.draw.rect(win, "#2ecc71", (j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE))
                    
                else:
                    pygame.draw.rect(win, "#27ae60", (j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE))
                    
                count +=1
            count+=1
        pygame.display.update()
        
    def first_draw(self,win):
        win.fill("#bdc3c7")
        count = 0
        for i in range(self.map_size):
            for j in range(self.map_size):
                if count%2 == 0:
                    pygame.draw.rect(win, "#2ecc71", (j * self.tile_size, i * self.tile_size, self.tile_size, self.tile_size))
                    
                else:
                    pygame.draw.rect(win, "#27ae60", (j * self.tile_size, i * self.tile_size, self.tile_size, self.tile_size))
                    
                count +=1
            count+= 1
        pygame.display.update()
    
                
                

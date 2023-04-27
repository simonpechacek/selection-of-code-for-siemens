import pygame


class Snake(object):
    def __init__(self, tile_size, win_len):
        self.tile_size = tile_size
        self.win_len = win_len
        self.head = [1, 0]
        self.body = [[0, 0]]
        self.body_cols = ["#1dd1a1", "#10ac84"]

    def update(self, direction):
        self.body.insert(0, self.head.copy())
        self.body.pop()

        if direction == "LEFT":
            self.head[0] -= 1
        elif direction == "RIGHT":
            self.head[0] += 1
        elif direction == "UP":
            self.head[1] -= 1
        elif direction == "DOWN":
            self.head[1] += 1
        return self.check_coll()

    def draw(self, win):
        pygame.draw.rect(win, "#10ac84",
                         (self.head[0] * self.tile_size, self.head[1] * self.tile_size, self.tile_size, self.tile_size))

        for i, rect in enumerate(self.body):
            pygame.draw.rect(win, "#1dd1a1",
                             (rect[0] * self.tile_size, rect[1] * self.tile_size, self.tile_size, self.tile_size))

    def get_pos(self):
        return self.head, self.body

    def check_coll(self):
        if self.head in self.body:
            return True
        return False

    def get_longer(self):
        self.body.append([-1, -1])

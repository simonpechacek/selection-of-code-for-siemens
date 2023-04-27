import pygame


class AnimatePath(object):
    def __init__(self, path, _tile_size):
        self.index = 0
        self.draw_path = False
        self._tile_size = _tile_size
        self.path = None
        self.length = 0
        self.growths = []
        self.update(path)

    def update(self, path):
        if path:
            self.path = path
            self.length = len(path)
            self.growths = [20 for _ in range(self.length)]
            self.index = 0
            self.draw_path = True
        else:
            self.draw_path = False

    def draw(self, win):
        if self.draw_path:
            for i in range(self.index + 1):
                # print(f"drawing rect at index {i}")
                size = self.growths[i]
                x = (self.path[i][0] * self._tile_size + 0.5 * self._tile_size) - size * 0.5
                y = (self.path[i][1] * self._tile_size + 0.5 * self._tile_size) - size * 0.5
                pygame.draw.rect(win, "#feca57", (x, y, size, size), border_radius=(size % self._tile_size))
                if size < 80:
                    size += 10
                    self.growths[i] = size
                    if size < 50:
                        if self.index == i and self.index < self.length - 1:
                            self.index += 1


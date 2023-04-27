import pygame

#init pygame 
pygame.init()

# WINDOW SIZE
WIN_SIZE = (400, 200)

# creates window surface
win = pygame.display.set_mode(WIN_SIZE)

# draw function - fills screen with color and updates it
def draw():
    win.fill((255, 255, 255))
    
    pygame.display.flip()


# mian loop
def main():
    run = True
    clock = pygame.time.Clock()
    
    while run:
        clock.tick(60)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
                
        draw()
        
if __name__ == "__main__":
    main()
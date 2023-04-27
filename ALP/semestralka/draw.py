from PIL import Image
from PIL import ImageColor
from PIL import ImageDraw
from PIL import ImageFont

class Drawer:
    def __init__(self):
        self.res = 30 #pixels per cell
        self.colors = ["#a2faa3","#92c9b1","#4f759b","#5d5179","#571f4e","#960200","#ce6c47","#ffd046","#f3d8c7","#69385c"]
        self.colors = ["#7798ab","#c3dbc5","#e8dcb9","#f2cee6","#ffa69e","#ff686b","#bf98a0","#6b0504","#a3320b","#9B287B"]



    def drawGrid(self, pix, board, imwidth, imheight):
        lineColor = (218,165,32)
        for row in range(len(board)):
            y = row*self.res
            for px in range(imwidth):
                pix[px,y] = lineColor
        for col in range(len(board[0])):
            x = col*self.res
            for py in range(imheight):
                pix[x,py] = lineColor
                
    def drawCell(self, pix, row, col, color ):
        y1 = row*self.res
        x1 = col*self.res
        y2 = (row+1)*self.res
        x2 = (col+1)*self.res
        for px in range(x1,x2):
            for py in range(y1,y2):
                pix[px,py] = color

    def drawMarks(self, img, marks):
        draw = ImageDraw.Draw(img)
        off = self.res/5
        for row in range(len(marks)):
            for col in range(len(marks[row])):
                if marks[row][col] == 1:
                    #draw 'o'
                    draw.ellipse((col*self.res+off,row*self.res+off, (col+1)*self.res-off, (row+1)*self.res-off), outline="black")
                    
                elif marks[row][col] == -1:
                    #draw 'x'
                    draw.line((col*self.res+off,row*self.res+off, (col+1)*self.res-off, (row+1)*self.res-off), fill="black", width=1)
                    draw.line(((col+1)*self.res-off,row*self.res+off, (col)*self.res+off, (row+1)*self.res-off), fill="black", width=1)

    def draw(self, board, marks, filename):
        imwidth = len(board[0])*self.res
        imheight = len(board)*self.res
        img = Image.new('RGB', (imwidth,imheight), "white")

        pix = img.load()
        draw = ImageDraw.Draw(img)
                   
        for row in range(len(board)):
            for col in range(len(board[row])):
                if board[row][col] != 0:
                    color = ImageColor.getrgb( self.colors[board[row][col]-1] )
                    self.drawCell(pix, row, col, color)
                    draw.text((col*self.res, row*self.res),"{}".format(board[row][col]),"black")
        self.drawMarks(img, marks)
        self.drawGrid(pix, board, imwidth, imheight)

        img.save(filename)    




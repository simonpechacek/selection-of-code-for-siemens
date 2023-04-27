import PySimpleGUI as sg

# window theme
sg.theme("DarkGrey13")

# here comes the window layout
layout = [
    [sg.Text("your text goes here")],
    [sg.Button("Exit")]
]

# main window object
win = sg.Window("title", layout)

def main():
    
    # main loop 
    while True:
        event, values = win.read()
        
        # add events that close window
        if event in (sg.WIN_CLOSED,"Exit"):
            break
    
if __name__ == "__main__":
    main()


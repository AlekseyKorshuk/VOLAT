from modules.actions.controller import Client
from modules.visual.classes import Map
import matplotlib.pyplot as plt
import uuid
from tkinter import *
from tkinter import ttk

client = Client()

entry = None
win = None


def create_window():
    global win
    win = Tk('WGForge', 'WGForge')
    win.title('WGForge')
    # Set the geometry of Tkinter frame
    win.geometry("280x110")

    # Initialize a Label to display the User Input
    label = Label(win, text="Enter the name of the game", font=("Courier 10 bold"))
    label.pack()
    global entry
    # Create an Entry widget to accept User Input
    entry = Entry(win, width=40)
    entry.focus_set()
    entry.pack()

    ttk.Button(win, text="Submit", width=20, command=display_text).pack(pady=20)
    return win


# Create a Button to validate Entry Widget
def display_text():
    global win
    global entry
    string = entry.get()
    win.destroy()

    client.login(f'VisualizerByVOLAT-{uuid.uuid4().hex}', game=string, num_players=3, is_observer=True)
    map = Map(client)
    while plt.fignum_exists(1):
        try:
            map.plot_map()
        except:
            pass
        plt.pause(0.01)
    restart()


def restart():
    global win
    win = create_window()
    win.mainloop()


restart()

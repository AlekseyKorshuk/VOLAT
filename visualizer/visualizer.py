from modules.actions.controller import Client
from modules.visual.classes import Map
import matplotlib.pyplot as plt
import uuid
from tkinter import *
from tkinter import ttk
from moviepy.editor import *
from moviepy.video.io.bindings import mplfig_to_npimage
import webbrowser

from threading import Thread


client = Client()

game_name = None
password = None
num_players = None
num_turns = None
status = None
button = None


def record(string_game_name, string_password, string_num_players, string_num_turns):
    global status, button
    button["state"] = "disabled"


    images_dict = {}
    client.login(
        name=f'VisualizerByVOLAT-{string_game_name}',
        game=string_game_name,
        password=string_password,
        num_players=int(string_num_players),
        num_turns=int(string_num_turns),
        is_observer=True
    )
    map = Map(client)
    # manager = plt.get_current_fig_manager()
    # manager.window.showMaximized()

    state = map.client.game_state()
    while len(state['players']) != state['num_players']:
        state = map.client.game_state()
        status.config(text=map.get_state_table(state, border=True))

    map.plot_map(False)
    images_dict[0] = mplfig_to_npimage(map.fig)

    while True:

        # win.update_idletasks()
        try:
            state = map.plot_map()

            if state['current_turn'] not in images_dict:
                images_dict[state['current_turn']] = mplfig_to_npimage(map.fig)
                status.config(text=map.get_state_table(state, border=True))
                map.client.turn()
        except Exception as ex:
            print(ex)
            pass
        try:
            if state['finished']:
                break
        except Exception as ex:
            pass

        # plt.pause(0.01)

    images_list = {array.tostring(): array for array in images_dict.values()}
    clip = ImageSequenceClip(list(images_list.values()), fps=1)
    clip.write_videofile(f"{string_game_name}.mp4")
    webbrowser.open(f"{string_game_name}.mp4")
    button["state"] = "normal"

# Create a Button to validate Entry Widget
def display_text():
    global win
    global game_name
    global password
    global num_players
    global num_turns

    string_game_name = game_name.get()
    string_password = password.get()
    string_num_players = num_players.get()
    string_num_turns = num_turns.get()
    # win.destroy()
    thread = Thread(target=record, args=(string_game_name, string_password, string_num_players, string_num_turns, ))
    thread.start()

    # restart()
    # status['text'] = "Pending..."


def create_window():
    global win
    win = Tk('WGForge', 'WGForge')
    win.title('WGForge')
    # Set the geometry of Tkinter frame
    win.geometry("520x470")

    # Initialize a Label to display the User Input
    label = Label(win, text="Name of the game", font=("Courier 10 bold"))
    label.pack()
    global game_name
    # Create an Entry widget to accept User Input
    game_name = Entry(win, width=40)
    game_name.insert(END, 'testVOLAT')
    game_name.focus_set()
    game_name.pack()

    # Initialize a Label to display the User Input
    label = Label(win, text="Password", font=("Courier 10 bold"))
    label.pack()
    global password
    # Create an Entry widget to accept User Input
    password = Entry(win, show="*", width=40)
    password.insert(END, '')
    password.focus_set()
    password.pack()

    # Initialize a Label to display the User Input
    label = Label(win, text="Number of players", font=("Courier 10 bold"))
    label.pack()
    global num_players
    # Create an Entry widget to accept User Input
    num_players = Entry(win, width=40)
    num_players.insert(END, 1)
    num_players.focus_set()
    num_players.pack()

    # Initialize a Label to display the User Input
    label = Label(win, text="Number of turns", font=("Courier 10 bold"))
    label.pack()
    global num_turns
    # Create an Entry widget to accept User Input
    num_turns = Entry(win, width=40)
    num_turns.insert(END, 15)
    num_turns.focus_set()
    num_turns.pack()

    global button
    button = ttk.Button(win, text="Record", width=20, command=display_text)
    button.pack(pady=20)

    # Initialize a Label to display the User Input
    label = Label(win, text="Game state", font=("Courier 10 bold"))
    label.pack()

    global status
    status = Label(win, text="Pending...", font=("Courier 10"))
    status.pack()

    return win


def create_status_window(string_game_name):
    global win
    win_status = Toplevel(win)

    win_status.title(f'WGForge {string_game_name} Status')
    # Set the geometry of Tkinter frame
    win_status.geometry("280x250")

    label = Label(win_status, text=string_game_name, font=("Courier 10 bold"))
    label.pack()

    status = Label(win_status, text="Recording game...", font=("Courier 10"))
    status.pack()

    # win_status.grab_set()
    return win_status, status

def restart():
    global win
    win = create_window()
    win.mainloop()

restart()
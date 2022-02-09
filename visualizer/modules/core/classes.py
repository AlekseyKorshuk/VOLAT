from tkinter import Label
from tkinter import ttk, Tk, Entry, END, Toplevel
from PIL import Image, ImageTk
from itertools import count
from threading import Thread
import numpy as np

from modules.client.controller import Client
from modules.map.classes import Map
from modules.core import permanent


class ImageLabel(Label):
    """A label that displays images, and plays them if they are gifs"""

    delay = None
    frames = None
    loc = None

    def load(self, im: str):
        """
        Loads Pillow Image to Label
        @param im: Image path
        @return: None
        """
        if isinstance(im, str):
            im = Image.open(im)
        self.loc = 0
        self.frames = []

        try:
            for i in count(1):
                self.frames.append(ImageTk.PhotoImage(im.copy()))
                im.seek(i)
        except EOFError:
            pass

        try:
            self.delay = im.info['duration']
        except KeyError:
            self.delay = 100

        if len(self.frames) == 1:
            self.config(image=self.frames[0])
        else:
            self.next_frame()

    def unload(self):
        """
        Removes image from Label
        @return: None
        """
        self.config(image="")
        self.frames = None

    def next_frame(self):
        """
        Shows next frame and schedules itself
        @return: None
        """
        if self.frames:
            self.loc += 1
            self.loc %= len(self.frames)
            self.config(image=self.frames[self.loc])
            self.after(self.delay, self.next_frame)


class Core:
    """Core class of Visualizer"""

    def __init__(self,
                 game_name: str = permanent.DEFAULT_GAME_NAME,
                 password: str = permanent.DEFAULT_PASSWORD,
                 num_turns: int = permanent.DEFAULT_NUM_TURNS,
                 num_players: int = permanent.DEFAULT_NUM_PLAYERS,
                 show_gui: bool = permanent.DEFAULT_SHOW_GUI
                 ):
        """
        Constructor with parameters
        @param game_name: Name of the game
        @param password: Password to log in
        @param num_turns: Number of turns
        @param num_players: Number of players (max 3)
        """
        self.window = None
        self.show_gui = show_gui
        self.button = None
        self.status = None
        self.game_name = game_name
        self.password = password
        self.num_players = num_players
        self.num_turns = num_turns
        self.client = Client()
        if show_gui:
            self.create_window()
            self.window.mainloop()
        else:
            self.record()

    def record(self):
        """
        Method records the game in thread
        @return: None
        """
        if self.show_gui:
            self.button["text"] = "Recording..."
            self.button["state"] = "disabled"
            preview_window = Toplevel(self.window)
            image_lbl = ImageLabel(preview_window)
            image_lbl.pack(expand=True)
        else:
            print("Recording...")

        images_dict = {}

        self.client.login(
            name=f'VisualizerByVOLAT-{self.game_name.get() if self.show_gui else self.game_name}',
            game=self.game_name.get() if self.show_gui else self.game_name,
            password=self.password.get() if self.show_gui else self.password,
            num_players=self.num_players.get() if self.show_gui else self.num_players,
            num_turns=self.num_turns.get() if self.show_gui else self.num_turns,
            is_observer=True
        )

        game_map = Map(self.client)

        state = game_map.client.game_state()
        while len(state['players']) != state['num_players']:
            state = game_map.client.game_state()
            if self.show_gui:
                self.status.config(text=game_map.get_state_table(state, border=True))

        game_map.plot_map(False)

        game_map.fig.canvas.draw()
        data = np.frombuffer(game_map.fig.canvas.tostring_rgb(), dtype=np.uint8)
        data = data.reshape(game_map.fig.canvas.get_width_height()[::-1] + (3,))
        images_dict[0] = Image.fromarray(data, 'RGB')
        if self.show_gui:
            image_lbl.load(Image.fromarray(data, 'RGB'))

        while True:
            try:
                state = game_map.plot_map()

                if state['current_turn'] not in images_dict:
                    game_map.fig.canvas.draw()
                    data = np.frombuffer(game_map.fig.canvas.tostring_rgb(), dtype=np.uint8)
                    data = data.reshape(game_map.fig.canvas.get_width_height()[::-1] + (3,))

                    images_dict[state['current_turn']] = Image.fromarray(data, 'RGB')
                    if self.show_gui:
                        image_lbl.load(Image.fromarray(data, 'RGB'))
                        self.status.config(text=game_map.get_state_table(state, border=True))
                    game_map.client.turn()
            except KeyError:
                pass

            if state['finished']:
                break
        images_list = list(images_dict.values())
        gif_name = f'{self.game_name.get() if self.show_gui else self.game_name}.gif'
        images_list[0].save(gif_name, save_all=True,
                            append_images=images_list[1:], optimize=False,
                            duration=1000, loop=0)

        if not self.show_gui:
            preview_window = Tk()
            image_lbl = ImageLabel(preview_window)
            image_lbl.pack(expand=True)

        image_lbl.load(gif_name)

        if not self.show_gui:
            preview_window.mainloop()

        if self.show_gui:
            self.button["state"] = "normal"
            self.button["text"] = "Record"

    def start(self):
        """
        Method starts the thread
        @return:  None
        """

        thread = Thread(target=self.record, args=())
        thread.start()

    def create_window(self):
        """
        Method creates window of the GUI
        @return: None
        """
        self.window = Tk('WGForge', 'WGForge')
        self.window.title('WGForge')
        # Set the geometry of Tkinter frame
        self.window.geometry("520x470")

        # Initialize a Label to display the User Input
        label = Label(self.window, text="Name of the game", font=("Courier 10 bold"))
        label.pack()

        # Create an Entry widget to accept User Input
        self.game_name = Entry(self.window, width=40)
        self.game_name.insert(END, permanent.DEFAULT_GAME_NAME)
        self.game_name.focus_set()
        self.game_name.pack()

        # Initialize a Label to display the User Input
        label = Label(self.window, text="Password", font=("Courier 10 bold"))
        label.pack()

        # Create an Entry widget to accept User Input
        self.password = Entry(self.window, show="*", width=40)
        self.password.insert(END, permanent.DEFAULT_PASSWORD)
        self.password.focus_set()
        self.password.pack()

        # Initialize a Label to display the User Input
        label = Label(self.window, text="Number of players", font=("Courier 10 bold"))
        label.pack()

        # Create an Entry widget to accept User Input
        self.num_players = Entry(self.window, width=40)
        self.num_players.insert(END, permanent.DEFAULT_NUM_PLAYERS)
        self.num_players.focus_set()
        self.num_players.pack()

        # Initialize a Label to display the User Input
        label = Label(self.window, text="Number of turns", font=("Courier 10 bold"))
        label.pack()

        # Create an Entry widget to accept User Input
        self.num_turns = Entry(self.window, width=40)
        self.num_turns.insert(END, permanent.DEFAULT_NUM_TURNS)
        self.num_turns.focus_set()
        self.num_turns.pack()

        # Initialize a Button to display the User Input
        self.button = ttk.Button(self.window, text="Record", width=20, command=self.start)
        self.button.pack(pady=20)

        # Initialize a Label to display the User Input
        label = Label(self.window, text="Game state", font=("Courier 10 bold"))
        label.pack()

        # Initialize a Label to display the User Input
        self.status = Label(self.window, text="Pending...", font=("Courier 10"))
        self.status.pack()

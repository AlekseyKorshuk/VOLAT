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
import sys


def main(args):
    print("Recording...")
    if len(args) == 0:
        name = "VOLAT1"
        password = ""
        game = "testVOLAT"
        num_turns = 15
        num_players = 1

    else:
        name = args[0]
        password = args[1]
        game = args[2]
        num_turns = int(args[3])
        num_players = int(args[4])

    client = Client()

    images_dict = {}
    client.login(
        name=name,
        game=game,
        password=password,
        num_players=int(num_players),
        num_turns=int(num_turns),
        is_observer=True
    )
    map = Map(client)
    # manager = plt.get_current_fig_manager()
    # manager.window.showMaximized()

    state = map.client.game_state()
    while len(state['players']) != state['num_players']:
        state = map.client.game_state()

    map.plot_map(False)
    images_dict[0] = mplfig_to_npimage(map.fig)

    while True:

        # win.update_idletasks()
        try:
            state = map.plot_map()

            if state['current_turn'] not in images_dict:
                images_dict[state['current_turn']] = mplfig_to_npimage(map.fig)
                print(map.get_state_table(state, True))
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
    clip.write_videofile(f"{game}.mp4")
    webbrowser.open(f"{game}.mp4")


if __name__ == "__main__":
    main(sys.argv[1:])

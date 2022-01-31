import matplotlib.pyplot as plt
from matplotlib.patches import RegularPolygon
import numpy as np

import matplotlib as mpl
from svgpathtools import svg2paths
from svgpath2mpl import parse_path
from modules.core.permanent import VEHICLE_TYPES

from prettytable import PrettyTable
import copy

class Map:
    def __init__(self, client, radius: int = 11, plot: bool = True):
        self.client = client
        self.radius = radius
        if plot:
            # plt.ion()
            self.get_default_map = copy.deepcopy(self._get_default_map())
            self.map = self.get_default_map
            self.vehicle_to_marker = self.parse_markers()
            self.fig, self.ax = plt.subplots(1, figsize=(16, 9))
            # self.fig.canvas.set_window_title('WGForge')
            self.colors = ['#ff8c42', '#53caf2', '#e170db']
            self.colors_text = ['orange', 'blue', 'purple']
            self.plot_map()

    def parse_markers(self):
        vehicle_to_marker = {}
        for vehicle_type in VEHICLE_TYPES:
            path, attributes = svg2paths(f'assets/{vehicle_type}.svg')
            marker = parse_path(' '.join([attribute['d'] for attribute in attributes]))
            marker.vertices -= marker.vertices.mean(axis=0)
            marker = marker.transformed(mpl.transforms.Affine2D().rotate_deg(180))
            marker = marker.transformed(mpl.transforms.Affine2D().scale(-1, 1))
            vehicle_to_marker[vehicle_type] = marker
        return vehicle_to_marker

    def _get_default_map(self):
        deltas = [[1, 0, -1], [0, 1, -1], [-1, 1, 0], [-1, 0, 1], [0, -1, 1], [1, -1, 0]]
        hexes = {}
        indices = {}
        index = 0

        for r in range(self.radius):
            x = 0
            y = -r
            z = +r

            color = 'green' if r <= 1 else 'white'
            hexes[(x, y, z)] = {
                'color': color
            }
            indices[(x, y, z)] = index  # Or store objects here
            index += 1

            for j in range(6):
                if j == 5:
                    num_of_hexes_in_edge = r - 1
                else:
                    num_of_hexes_in_edge = r
                for i in range(num_of_hexes_in_edge):
                    x = x + deltas[j][0]
                    y = y + deltas[j][1]
                    z = z + deltas[j][2]

                    color = 'green' if r <= 1 else 'white'
                    hexes[(x, y, z)] = {
                        'color': color
                    }
                    indices[(x, y, z)] = index  # Or store objects here
                    index += 1
        return hexes

    def plot_map(self, current_position=True):
        position_key = 'position' if current_position else 'spawn_position'
        plt.cla()
        self.ax.spines['top'].set_visible(False)
        self.ax.spines['right'].set_visible(False)
        self.ax.spines['bottom'].set_visible(False)
        self.ax.spines['left'].set_visible(False)
        plt.gca().axes.get_yaxis().set_visible(False)
        plt.gca().axes.get_xaxis().set_visible(False)

        state = self.client.game_state()
        self.update_map(state, position_key)


        coord = self.map.keys()
        colors = [value['color'] for value in self.map.values()]
        labels = [value if 'health' in value else {} for value in
                  self.map.values()]  # [value['health'] for value in map.values()]

        # Horizontal cartesian coords
        hcoord = [c[0] for c in coord]
        # Vertical cartersian coords
        vcoord = [2. * np.sin(np.radians(60)) * (c[1] - c[2]) / 3. for c in coord]

        self.ax.set_aspect('equal')

        # Add some coloured hexagons
        for x, y, c, l in zip(hcoord, vcoord, colors, labels):
            # color = c[0].lower()  # matplotlib understands lower case words for colours
            hex = RegularPolygon((x, y), numVertices=6, radius=2. / 3.,
                                 orientation=np.radians(30),
                                 facecolor=c, alpha=0.2, edgecolor='k', linewidth=1)
            self.ax.add_patch(hex)
            # Also add a text label
            if 'health' in l:
                self.ax.text(x - 0.01, y - 0.01, l['health'], ha='center', va='center', size=8)
                self.ax.scatter([x + 0.05], [y + 0.07], s=250, c=[c], alpha=0.7,
                                marker=self.vehicle_to_marker[l['vehicle_type']])
        self.ax.scatter([-10, 10], [-10, 10], s=250, c=[None, None], alpha=0.7,
                        marker='*')


        # plt.show(block=False)
        return state

    def update_map(self, state, position_key):
        self.map = copy.deepcopy(self.get_default_map)
        text_field = self.get_state_table(state, False, replace=True)
        self.ax.text(-0.3, 0.9, text_field, bbox={'facecolor': 'red',
                                                  'alpha': 0.5, 'pad': 15},
                     transform=self.ax.transAxes, ha="center")
        players_colors = {}
        for vehicle in state['vehicles'].values():
            if vehicle['health'] > 0:
                position = vehicle[position_key].values()
                if vehicle['player_id'] not in players_colors:
                    players_colors[vehicle['player_id']] = self.colors[len(players_colors.keys())]
                self.map[
                    tuple(position)
                ] = {
                    'color': players_colors[vehicle['player_id']],
                    **vehicle
                }

    def get_state_table(self, state, border: bool = False, replace: bool = False):
        try:
            x = PrettyTable(border=border)
            x.field_names = [f"Turn {state['current_turn']}/{state['num_turns']}", "Name", "Color", "Capture", "Kill"]
            for i, player in enumerate(state['players']):
                x.add_row(['->' if state['current_player_idx'] == player['idx'] else '  ', player['name'], self.colors_text[i],
                           *state['win_points'][str(player['idx'])].values()])

            text_field = x.get_string()
            if replace:
                # text_field = text_field.replace(' ', '⠀')
                text_field = text_field.replace(' ', '  ')
            return text_field
        except:
            return None

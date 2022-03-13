import matplotlib.pyplot as plt
from matplotlib.patches import RegularPolygon
import numpy as np

import matplotlib as mpl

mpl.use('agg')
from svgpathtools import svg2paths
from svgpath2mpl import parse_path
from modules.core.permanent import VEHICLE_TYPES

from prettytable import PrettyTable
import copy
from modules.client.controller import Client


class Map:
    """Class represents current state of the game map"""

    def __init__(self, client: Client, radius: int = 11, plot: bool = True):
        """
        Constructor of Map class
        @param client:
        @param radius:
        @param plot:
        """
        self.client = client
        self.radius = radius
        if plot:
            self.get_default_map = copy.deepcopy(self._get_default_map())
            self.map = self.get_default_map
            self.vehicle_to_marker = self.parse_markers()
            self.fig, self.ax = plt.subplots(1, figsize=(16, 9))
            self.colors = ['#ff8c42', '#53caf2', '#e170db']
            self.colors_text = ['orange', 'blue', 'purple']
            self.plot_map()

    def parse_markers(self) -> dict:
        """
        Parses markers from assets
        @return: Dictionary of markers
        """
        vehicle_to_marker = {}
        for vehicle_type in VEHICLE_TYPES:
            _, attributes = svg2paths(f'assets/{vehicle_type}.svg')
            marker = parse_path(' '.join([attribute['d'] for attribute in attributes]))
            marker.vertices -= marker.vertices.mean(axis=0)
            marker = marker.transformed(mpl.transforms.Affine2D().rotate_deg(180))
            marker = marker.transformed(mpl.transforms.Affine2D().scale(-1, 1))
            vehicle_to_marker[vehicle_type] = marker
        return vehicle_to_marker

    def _get_default_map(self) -> dict:
        """
        Calculates default map
        @return: Dictionary of hexes
        """
        deltas = [[1, 0, -1], [0, 1, -1], [-1, 1, 0], [-1, 0, 1], [0, -1, 1], [1, -1, 0]]
        hexes = {}
        indices = {}
        index = 0

        for r in range(self.radius):
            x = 0
            y = -r
            z = +r

            hexes[(x, y, z)] = {
                'color': 'white'
            }
            indices[(x, y, z)] = index  # Or store objects here
            index += 1

            for j in range(6):
                if j == 5:
                    num_of_hexes_in_edge = r - 1
                else:
                    num_of_hexes_in_edge = r
                for _ in range(num_of_hexes_in_edge):
                    x = x + deltas[j][0]
                    y = y + deltas[j][1]
                    z = z + deltas[j][2]

                    hexes[(x, y, z)] = {
                        'color': 'white'
                    }
                    indices[(x, y, z)] = index  # Or store objects here
                    index += 1
        map_data = self.client.map()
        for base in map_data['content']['base']:
            hexes[(base['x'], base['y'], base['z'])] = {
                'color': 'green'
            }
        for obstacle in map_data['content']['obstacle']:
            hexes[(obstacle['x'], obstacle['y'], obstacle['z'])] = {
                'color': 'black'
            }

        for light_repair in map_data['content']['light_repair']:
            hexes[(light_repair['x'], light_repair['y'], light_repair['z'])] = {
                'color': 'yellow'
            }

        for hard_repair in map_data['content']['hard_repair']:
            hexes[(hard_repair['x'], hard_repair['y'], hard_repair['z'])] = {
                'color': 'red'
            }

        for catapult in map_data['content']['catapult']:
            hexes[(catapult['x'], catapult['y'], catapult['z'])] = {
                'color': 'aqua'
            }
        return hexes

    def plot_map(self, current_position: bool = True) -> dict:
        """
        Plots map
        @param current_position: Whenever wo plot current positions. Otherwise, plot spawn positions
        @return: Game state
        """
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
                  self.map.values()]

        # Horizontal cartesian coords
        hcoord = [c[0] for c in coord]
        # Vertical cartersian coords
        vcoord = [2. * np.sin(np.radians(60)) * (c[1] - c[2]) / 3. for c in coord]

        self.ax.set_aspect('equal')

        # Add some coloured hexagons
        for x, y, c, l in zip(hcoord, vcoord, colors, labels):
            self.ax.add_patch(RegularPolygon((x, y), numVertices=6, radius=2. / 3.,
                                             orientation=np.radians(30),
                                             facecolor=c, alpha=0.2, edgecolor='k', linewidth=1))
            # Also add a text label
            if 'health' in l:
                self.ax.text(x - 0.01, y - 0.01, l['health'], ha='center', va='center', size=8)
                self.ax.scatter([x + 0.05], [y + 0.07], s=250, c=[c], alpha=0.7,
                                marker=self.vehicle_to_marker[l['vehicle_type']])
        self.ax.scatter([-10, 10], [-10, 10], s=250, c=[None, None], alpha=0.7,
                        marker='*')

        return state

    def update_map(self, state: dict, position_key: str) -> None:
        """
        Updates map from given state
        @param state: Game state
        @param position_key: Position key: position or spawn_position
        @return: None
        """
        self.map = copy.deepcopy(self.get_default_map)
        text_field = self.get_state_table(state, False, replace=True)
        self.ax.text(-0.3, 0.9, text_field if text_field != '' else None, bbox={'facecolor': 'red',
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

    def get_state_table(self, state: dict, border: bool = False, replace: bool = False) -> str:
        """
        Generates state table
        @param state: Game state
        @param border: Whenever to show borders
        @param replace: Whenever to replace spaces
        @return: Resulting table or None
        """
        try:
            x = PrettyTable(border=border)
            x.field_names = [f"Turn {state['current_turn']}/{state['num_turns']}", "Name", "Color", "Capture", "Kill"]
            for i, player in enumerate(state['players']):
                x.add_row(['->' if state['current_player_idx'] == player['idx'] else '  ', player['name'],
                           self.colors_text[i],
                           *state['win_points'][str(player['idx'])].values()])

            text_field = x.get_string()
            if replace:
                text_field = text_field.replace(' ', '  ')
            return text_field
        except KeyError:
            return ''

from modules.client.classes import BytesConverter
from modules.client import permanent
import socket
import inspect
from modules.core.permanent import HOST, PORT


class Client:
    """Client-side message performer"""

    def __init__(self):
        """Default constructor of the class. Connects to the Game Server."""
        self.bytes_converter = BytesConverter()
        self.client = socket.socket(
            socket.AF_INET,
            socket.SOCK_STREAM
        )
        self.client.connect((HOST, PORT))

    def get_kwargs(self):
        """
        Returns kwargs for called method
        :return: kwargs dict
        """
        frame = inspect.currentframe().f_back
        keys, _, _, values = inspect.getargvalues(frame)
        kwargs = {}
        for key in keys:
            if key != 'self':
                kwargs[key] = values[key]
        return kwargs

    def _send_message(self,
                      message: bytes,
                      ) -> dict:
        """
        Sends and retrieves messages
        :param message: bytes message to send
        :return: dict response message
        """
        self.client.sendall(message)

        result_code = int.from_bytes(self.client.recv(4, socket.MSG_WAITALL), "little")
        message_length = int.from_bytes(self.client.recv(4, socket.MSG_WAITALL), "little")
        if message_length == 0:
            return {'result_code': result_code}

        msg = self.client.recv(message_length, socket.MSG_WAITALL)

        return self.bytes_converter.message_to_dict(
            result_code=result_code,
            message=msg
        )

    def login(self,
              name: str,
              password: str = "",
              game: str = None,
              num_turns: int = None,
              num_players: int = None,
              is_observer: bool = False
              ) -> dict:
        """
        This action message has to be the first in a client-server "dialog".
        Login action authenticate client and connect him to game.

        If new game is about to be created, client can control some
        of its parameters. Otherwise, game's parameters are ignored.

        :param name: player's name
        :param password: player's password used to verify the connection,
            if player with the same name tries to connect
            with another password - login will be rejected.
        :param game: game's name (use it to connect to existing game)
        :param num_turns: number of game turns to be played
        :param num_players: number of players in the game
        :param is_observer: defines if player connect to server just for watching
        :return: dict response message
        """
        action_name = 'LOGIN'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
            message=self.get_kwargs()
        )
        return self._send_message(msg)

    def logout(self):
        """
        Logout and remove player record from server storage.
        :return: dict response message
        """
        action_name = 'LOGOUT'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
        )
        return self._send_message(msg)

    def map(self):
        """
        Returns the game map. Map represents static information about game.
        :return: dict response message
        """
        action_name = 'MAP'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
        )
        return self._send_message(msg)

    def game_state(self):
        """
        Returns current state of game. Game state represents dynamic information about game.
        :return: dict response message
        """
        action_name = 'GAME_STATE'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
        )
        return self._send_message(msg)

    def game_actions(self):
        """
        Returns list of game client that happened in previous turn. Represent changes between turns.
        :return: dict response message
        """
        action_name = 'GAME_ACTIONS'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
        )
        return self._send_message(msg)

    def turn(self):
        """
        This action is needed to force next turn of the game, it allows you to not wait for game's
        time slice and play faster. The game time slice equals to 10 seconds.
        :return: dict response message
        """
        action_name = 'TURN'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
        )
        return self._send_message(msg)

    def chat(self,
             message: str,
             ) -> dict:
        """
        Do nothing. Just for testing and fun.
        :param message: string message
        :return: dict response message
        """
        action_name = 'CHAT'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
            message=self.get_kwargs()
        )
        return self._send_message(msg)

    def move(self,
             vehicle_id: int,
             target: dict
             ) -> dict:
        """
        Changes vehicle position.
        :param vehicle_id: id of vehicle.
        :param target: coordinates of hex, Ex: {"x":-1,"y":1,"z":0}
        :return: dict response message
        """
        action_name = 'MOVE'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
            message=self.get_kwargs()
        )
        return self._send_message(msg)

    def shoot(self,
              vehicle_id: int,
              target: dict
              ) -> dict:
        """
        Shoot to target position.
        :param vehicle_id: id of vehicle
        :param target: coordinates of hex
        :return: dict response message
        """
        action_name = 'SHOOT'
        msg = self.bytes_converter.generate_message(
            action=permanent.ACTIONS[action_name],
            message=self.get_kwargs()
        )
        return self._send_message(msg)

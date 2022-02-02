import argparse
from modules.core.classes import Core
from modules.core import permanent


def main():
    """
    Main function to be called
    @return: None
    """
    parser = argparse.ArgumentParser(description='WGForge Visualizer Settings')
    parser.add_argument('--game_name', dest='game_name', type=str, help='Name of the game',
                        default=permanent.DEFAULT_GAME_NAME)
    parser.add_argument('--password', dest='password', type=str, help='Password to log in',
                        default=permanent.DEFAULT_PASSWORD)
    parser.add_argument('--num_turns', dest='num_turns', type=int, help='Number of turns',
                        default=permanent.DEFAULT_NUM_TURNS)
    parser.add_argument('--num_players', dest='num_players', type=int, help='Number of players',
                        default=permanent.DEFAULT_NUM_PLAYERS)
    parser.add_argument('--hide_gui', dest='hide_gui', action='store_true',
                        help='Whenever to hide GUI',
                        default=not permanent.DEFAULT_SHOW_GUI)
    parser.set_defaults(hide_gui=not permanent.DEFAULT_SHOW_GUI)

    args = parser.parse_args()

    Core(
        game_name=args.game_name,
        password=args.password,
        num_turns=args.num_turns,
        num_players=args.num_players,
        show_gui=not args.hide_gui
    )


if __name__ == "__main__":
    main()

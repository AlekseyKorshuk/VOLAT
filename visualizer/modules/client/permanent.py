MODULE_NAME = 'client'

ACTIONS = {
    'LOGIN': 1,
    'LOGOUT': 2,
    'MAP': 3,
    'GAME_STATE': 4,
    'GAME_ACTIONS': 5,
    'TURN': 6,
    'CHAT': 100,
    'MOVE': 101,
    'SHOOT': 102
}

RESULTS = {
    'OKEY': 0,
    'BAD_COMMAND': 1,
    'ACCESS_DENIED': 2,
    'INAPPROPRIATE_GAME_STATE': 3,
    'TIMEOUT': 4,
    'INTERNAL_SERVER_ERROR': 500
}

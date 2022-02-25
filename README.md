# VOLAT

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/8945fc2b3ef74c258ff0c55ca7dd9ee8?branch=dev)](https://app.codacy.com/gh/AlekseyKorshuk/VOLAT?utm_source=github.com&utm_medium=referral&utm_content=AlekseyKorshuk/VOLAT&utm_campaign=Badge_Grade_Settings)
[![CodeFactor](https://www.codefactor.io/repository/github/alekseykorshuk/volat/badge/dev)](https://www.codefactor.io/repository/github/alekseykorshuk/volat/overview/dev)
[![CMake Build Matrix](https://github.com/AlekseyKorshuk/VOLAT/actions/workflows/c-cpp.yml/badge.svg?branch=dev)](https://github.com/AlekseyKorshuk/VOLAT/actions/workflows/c-cpp.yml)

## How to build
```
cmake -B build
cmake --build build
```
## How to use
```
cd build/debug
WGForgeProject.exe name password game num_turns num_players 1
```
name - player's name\
password  - player's password\
game - game's name\
num_turns - number of game turns to be played\
num_players - number of players in the game

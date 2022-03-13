# VOLAT

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/8945fc2b3ef74c258ff0c55ca7dd9ee8?branch=main)](https://app.codacy.com/gh/AlekseyKorshuk/VOLAT?utm_source=github.com&utm_medium=referral&utm_content=AlekseyKorshuk/VOLAT&utm_campaign=Badge_Grade_Settings)
[![CodeFactor](https://www.codefactor.io/repository/github/alekseykorshuk/volat/badge/main)](https://www.codefactor.io/repository/github/alekseykorshuk/volat/overview/main)
[![CMake Build Matrix](https://github.com/AlekseyKorshuk/VOLAT/actions/workflows/c-cpp.yml/badge.svg?branch=main)](https://github.com/AlekseyKorshuk/VOLAT/actions/workflows/c-cpp.yml)

## Description
The game is a turn-based strategy game based on the original World of tanks game. The game can be played by 3 people, under the control of which there are 5 different types of equipment with their own characteristics.

![Demo game](https://i.postimg.cc/yN7ym6Db/test-VOLAT2.gif)

## How to build
```
cmake -B build
cmake --build build
```
## How to use
```
cd build/debug
WGForgeProject.exe name password game num_turns num_players
```
name - player's name\
password  - player's password\
game - game's name\
num_turns - number of game turns to be played\
num_players - number of players in the game


## About

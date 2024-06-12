# Maze Game

## Overview

"Maze Game" is a game that uses a raycasting engine to render graphics and simulate a 3D world

This project was written in C (game engine & logic) with the help of the SDL2 rendering library

## Usage

### Installation
```sh
$ git clone https://github.com/gnxrlyqf/alx-maze_game.git
```
### Compilation:
```sh
$ make build
```
or
```sh
gcc ./src/files/* -o game -Wall -Werror -Wextra -pedantic -lSDL2 -lSDL2_image -lm -lpng
```
### Execution:
Use `$ make run` or `$ ./game` to run the game<br/>
Use `$ ./game exp` to run the game and enable minimap
### Controls
Use `W`, `A`, `S` & `D` to move `forward`, `left`, `backwards` & `right` respectively<br/>
Use `left arrow` & `right arrow` to turn the camera `left` and `right`

## Credits

[Mohamed Youssef Chetoui](https://github.com/gnxrlyqf): Lead developer and game designer<br/>
Wall textures by [wonniie](https://www.instagram.com/wonniiz/)
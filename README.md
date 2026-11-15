# Quoridor in C

The Quoridor board game in the Windows console, for 2 or 4 players: pawn moves, wall placement, saving and resuming a game, cumulative scores and game history.

Third-year programming project (ECE Paris, 2024), built as a team. The in-game text and the identifiers are in French.

![C11](https://img.shields.io/badge/C-11-00599c)
![CMake](https://img.shields.io/badge/build-CMake-064f8c)
![Windows](https://img.shields.io/badge/platform-Windows%20console-0078d6)

```
   Joueur actif : Alice  | Jeton : A  | Murs restants : 10

    A     B     C     D     E     F     G     H     I
 1  .     .     .     .     .     .     .     .     .
                      B     B
 2  .     .     .     .     A     .     .     .     .

 3  .     .     .     .     .     .     .     .     .
```

## Features

- Menu navigated with the arrow keys (new game, resume, help, scores, quit)
- 2-player games (10 walls each) or 4-player games (5 walls each), first player drawn at random
- Keyboard moves (`Z` `Q` `S` `D`), refused when a wall blocks the way
- Wall placement between two neighbouring squares, with bounds, alignment and overlap checks
- A player wins as soon as their pawn reaches the side opposite its start: 5 points, accumulated from one game to the next
- Full save (board, players, positions, current turn) and resume from the menu
- History of finished games in `historique.txt`

## Build and run

Requirements: Windows, CMake 3.16 or newer, MinGW-w64 or MSVC.

```bash
git clone https://github.com/shiva-tilhoo/quoridor-c.git
cd quoridor-c
cmake -S . -B build
cmake --build build
```

The `quoridor.exe` executable is created in `build/`. The project also opens directly in CLion or Visual Studio.

## How to play

On each turn, the player picks an action:

| Choice | Action |
|---|---|
| `1` | Move the pawn one square with `Z` (up), `S` (down), `Q` (left), `D` (right) |
| `2` | Place a wall: two neighbouring squares then a side, for example `D1 E1 B` to block the bottom of squares D1 and E1 (`H` top, `B` bottom, `G` left, `D` right) |
| `3` | Skip the turn |
| `4` | Quit, with or without saving |

## Code layout

```
├── include/
│   ├── jeu.h            Data structures (board, player, wall) and rules
│   ├── affichage.h      Console rendering
│   ├── interaction.h    Player actions and game loop
│   └── utile.h          Colours, input, random numbers
├── src/
│   ├── main.c           Main menu
│   ├── jeu.c            Initialisation, win detection, save/load, scores, history
│   ├── affichage.c      Coloured board, menu, help, score table
│   ├── interaction.c    Pawn move, wall placement, game flow
│   └── utile.c          Utility functions
└── CMakeLists.txt
```

The 9 × 9 board is stored in a 17 × 17 grid: playable squares sit on even indices, and walls go on the odd indices between two squares. Checking whether a move is blocked comes down to reading a single cell.

## History

The version handed in by the team was a single file. On my working branch I had split the code into modules; this repository combines that structure with the complete rules of the final version. The merge fixed the end-of-game flow, the mapping between the menu and its actions, and made saved games reloadable (pawn positions and current turn).

## Possible improvements

- Check that a wall always leaves a path for every player
- Forbid two pawns on the same square and handle jumping over an opponent
- Port beyond Windows (replace `conio.h` and `windows.h`)

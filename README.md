# Othello Game Simulator (C++)

This project is a **C++ implementation of the Othello (Reversi) game** with an
AI player using **Minimax with Alpha–Beta pruning**.

The program reads an initial board configuration from a file and can:
- Compute the best move at a given depth
- Simulate a full game
- Determine the winner

---

## Features

- Complete Othello rule implementation
- Minimax search with alpha–beta pruning
- Automatic handling of pass moves
- Supports configurable search depth
- Clean STL-based C++ implementation

---

## Files

- `othello.cpp` — main source code
- `input.txt` — initial board configuration

---

## Input Format (`input.txt`)

<turn> 8x8 board values ```
Where:

0 = Black

1 = White

-1 = Empty

Example:

0
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1  1  0 -1 -1 -1
-1 -1 -1  0  1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1

Compile and Run
Compile
g++ -std=c++17 othello.cpp -O2 -o othello
Run
./Othello

Output

Prints the board after each move

Displays all moves played

Declares the winner (Black / White / Draw)

Notes

Designed for academic use and learning AI game search

Board size is fixed at 8×8

Evaluation is from the initial player's perspective
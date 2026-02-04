# Othello Game Simulator (C++)

This project is a **C++ implementation of the Othello (Reversi) game** with an AI player using **Minimax with Alpha–Beta pruning**.

The program reads an initial board configuration from a file and can:
- Compute the best move at a given search depth
- Simulate a full game automatically
- Determine the winner (Black / White / Draw)

---

## Features

- Complete implementation of Othello game rules
- Minimax search with alpha–beta pruning
- Automatic handling of pass moves
- Configurable search depth
- Clean STL-based C++ implementation

---

---

## Algorithm

The AI uses the **Minimax algorithm with Alpha–Beta pruning** to explore the game tree up to a fixed depth.
Board evaluation is based on the difference in disc counts from the initial player's perspective.

## Files

- `Othello.cpp` — main source code
- `input.txt` — initial board configuration

---

## Input Format (`input.txt`)

The input file contains:
1. The current turn
2. An 8×8 board configuration

---

## Compile and Run

### Compile
```bash
g++ -std=c++17 Othello.cpp -O2 -o othello

## Output

- Prints the board after each move
- Displays all moves played
- Declares the final winner (Black / White / Draw)

### Values
- `0` → Black
- `1` → White
- `-1` → Empty

### Example
0
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 1 0 -1 -1 -1
-1 -1 -1 0 1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1
-1 -1 -1 -1 -1 -1 -1 -1

## License

This project is released for academic and educational use.



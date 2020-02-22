# Sudoku

An expressively coded, time & space efficient n-by-n Sudoku puzzle solver using recursive backtracking.

## Formatting

Usage: ./sudoku puzzle.txt > solution.txt

Puzzles should be formatted as a n x n square of digits. Non-numeric characters are ignored in the input. Use a numeric '0' for unknown entries. Given the puzzle in puzzle.txt, the solver will output a well-formatted solution (if it exists) to solution.txt.

## Algorithm Overview

1. Find the next "open" (not given by problem) square. If none are open, return SUCCESS and backtrack.
2. Get available digits for the current square. If there are none, return FAILURE and backtrack.
3. For each digit in the available collection, try that digit in the square and make a recursive call to the next open square.
4. If all digits result in recursive calls that return FAILURE, return FAILURE at this square. Otherwise, report SUCCESS on the first recursive SUCCESS. Backtrack in either case.

Once we get to the original call, we will have the overall solution for the Sudoku puzzle.

## Behavior

Note that any given Sudoku Puzzle can be under-constrained, properly constrained, or over-constrained.

- Under-constrained: The solver returns the first valid solution that it finds.
- Properly-constrained: The solver returns the unique solution to the puzzle.
- Over-constrained: No valid solution exists; the solver will report this.

Additionally, the program will ensure that the given puzzle is valid. It will report errors if a puzzle is invalid.

## Data Structures

### Square

At the most basic level, a Square represents each individual position on the Sudoku grid. It consists of:

- unsigned char to represent the current number (0 for unassigned squares). We will print unsigned chars as numbers using the prefix unary operator+.
- std::optional< std::bitset<n> > that reports the possible digits where bitset[i] represents whether i + 1 is a valid number here. We use std::optional since squares given in the problem statement do not require that we track their possible digits. They are set in stone.
- unsigned int row and column coordinates of the given Square.

### Grid

The Grid's API is an n-by-n two-dimensional array of Squares implemented as the one-dimensional std::array< Square, n * n >. The fixed squares (those given in the problem) will be skipped over while iterating over the open squares to make code more readable.

## Print Format

~~~~C++
- - - - - - - - - - - - -
| x x x | x x x | x x x |
| x x x | x x x | x x x |
| x x x | x x x | x x x |
- - - - - - - - - - - - -
| x x x | x x x | x x x |
| x x x | x x x | x x x |
| x x x | x x x | x x x |
- - - - - - - - - - - - -
| x x x | x x x | x x x |
| x x x | x x x | x x x |
| x x x | x x x | x x x |
- - - - - - - - - - - - -
~~~~

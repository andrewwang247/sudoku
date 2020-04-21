/*
Copyright 2020. Siwei Wang.
*/
#pragma once
#include <array>

#include "square.h"

using grid_t = std::array<Square, NUM_SQUARES>;

class Grid {
 private:
  // Two-dimensional 9-by-9 array of Squares
  grid_t m_grid;

  // Enumeration of possible puzzle States.
  enum class PuzzleState {
    Unsolved,
    Solution_Exists,
    No_Solution,
    Invalid_Puzzle
  };

 public:
  /**
   * Basic default constructor that sets coordinates on Squares,
   * initializes all values to 0, and sets state to Solution_Exists.
   */
  explicit Grid(std::istream& is);

  /**
   * Basic reference access by row and column index.
   */
  Square& at(unsigned row, unsigned col);
  const Square& at(unsigned row, unsigned col) const;

  /**
   * Reads a 9x9 square of digits from is into the grid.
   * Non-numeric characters are ignored in the input.
   * Use a capital 'X' for unknown entries.
   * ASSUMES: g is in just-constructed state.
   * REPLACES: Grid, state.
   * THROWS IF : grid cannot be formed.
   */
  friend std::istream& operator>>(std::istream& is, Grid& g);

  /**
   * If state == Solution_Exists, prints g to os in a nicely formatted manner.
   * If state == No_Solution, states that no solution could be found for the
   * puzzle. If state == Invalid_Puzzle, displays an error message to the
   * screen.
   */
  friend std::ostream& operator<<(std::ostream& os, Grid& g);

  /**
   * Used at the beginning to ensure that the grid is in a valid state before
   * starting. If invalid, modifies m_state to the invalid state.
   */
  void check_state() noexcept;

  /**
   * Interface function that solves the Sudoku grid.
   * Only solves if state != Invalid_Puzzle.
   * MODIFIES: grid, solution_exists.
   */
  void solve();

 private:
  // Contains the state of the puzzle. Should remain relatively unchanged after
  // >>.
  PuzzleState m_state;

  /**
   * Checks the subgrid starting at (row_start, col_start)
   * REQUIRES: bound indexing is correct.
   */
  bool check_subgrid(unsigned row, unsigned col) const;

  /**
   * Updates the bitset in the Square *it to reflect the possible values *it can
   * take.
   * @param it: A iterator reference pointing to a square in the grid.
   * REQUIRES: Grid is in a valid state.
   * MODIFIES: it->possible_values.
   */
  void find_possible(grid_t::iterator it) noexcept;

  /**
   * ! This procedure is at the heart of the backtracking algorithm.
   * Helper function for Grid::solve that allows for recursive backtracking
   * calls. RETURNS: true if puzzle is solved and false if no solution exists.
   */
  bool solve_helper(grid_t::iterator it);
};

/*
Copyright 2020. Siwei Wang.
*/
#include "grid.h"

#include <algorithm>
#include <cctype>
#include <exception>
#include <iterator>
#include <string>
#include <vector>
using std::any_of;
using std::bitset;
using std::find_if;
using std::isdigit;
using std::istream;
using std::next;
using std::ostream;
using std::runtime_error;
using std::string;
using std::strtoul;
using std::vector;

Grid::Grid(istream& is) : m_state(PuzzleState::Unsolved) {
  // Get the row and column listings into each square.
  for (unsigned i = 0; i < NUM_DIGITS; ++i) {
    for (unsigned j = 0; j < NUM_DIGITS; ++j) {
      Square& sq = m_grid[NUM_DIGITS * i + j];
      sq.m_row = i;
      sq.m_column = j;
    }
  }

  // Read 81 integer valued items from is.
  vector<int> buffer;
  buffer.reserve(NUM_SQUARES);
  unsigned num_read = 0;
  for (string str; is >> str && num_read < NUM_SQUARES;) {
    if (any_of(str.begin(), str.end(), [](char c) { return !isdigit(c); }))
      continue;
    const auto numeric = strtoul(str.c_str(), nullptr, 10);
    if (numeric > NUM_DIGITS)
      throw runtime_error("Input number is out of range.");
    if (numeric == 0)
      m_grid[num_read].m_possible_values.emplace(bitset<NUM_DIGITS>());
    m_grid[num_read++].m_number = static_cast<unsigned char>(numeric);
  }

  if (num_read != NUM_SQUARES)
    throw runtime_error("Not enough numbers to fill Sudoku grid");
}

inline Square& Grid::at(unsigned row, unsigned col) {
  return m_grid[row * NUM_DIGITS + col];
}

inline const Square& Grid::at(unsigned row, unsigned col) const {
  return m_grid[row * NUM_DIGITS + col];
}

/**
 * Returns whether ptr points to an open square or not.
 * REQUIRES: ptr is non-null. Does not check for this!
 */
inline bool is_valid_square(const Square& sq) {
  return sq.m_possible_values.has_value();
}

void Grid::solve() {
  // No work needs to be done if the puzzle is invalid or solved.
  if (m_state != PuzzleState::Unsolved) return;

  // Get the first open square.
  auto it = find_if(m_grid.begin(), m_grid.end(), is_valid_square);

  // Fancy ternary operator that sets state based on the result of solve_helper.
  m_state = solve_helper(it) ? PuzzleState::Solution_Exists
                             : PuzzleState::No_Solution;
}

bool Grid::check_subgrid(unsigned row_start, unsigned col_start) const {
  bitset<NUM_DIGITS> bs;
  for (unsigned row = row_start; row < row_start + BOX_SIDE; ++row) {
    for (unsigned col = col_start; col < col_start + BOX_SIDE; ++col) {
      const auto& sq = at(row, col);
      // Check that there are no repeats.
      if (sq.m_number != 0 && bs[sq.m_number - 1]) {
        return false;
      }
      bs[sq.m_number - 1] = true;
    }
    bs.reset();
  }
  return true;
}

void Grid::check_state() noexcept {
  bitset<NUM_DIGITS> bs;

  unsigned index = 0;
  // Check that the rows going across are valid.
  for (unsigned row = 0; row < NUM_DIGITS; ++row) {
    for (unsigned col = 0; col < NUM_DIGITS; ++col) {
      const auto& sq = m_grid[index++];
      // Check that there are no repeats.
      if (sq.m_number != 0 && bs[sq.m_number - 1]) {
        m_state = PuzzleState::Invalid_Puzzle;
        return;
      }
      bs[sq.m_number - 1] = true;
    }
    bs.reset();
  }
  index = 0;

  // Check that the columns going down are valid.
  for (unsigned col = 0; col < NUM_DIGITS; ++col) {
    for (unsigned row = 0; row < NUM_DIGITS; ++row) {
      const auto& sq = m_grid[index++];
      // Check that there are no repeats.
      if (sq.m_number != 0 && bs[sq.m_number - 1]) {
        m_state = PuzzleState::Invalid_Puzzle;
        return;
      }
      bs[sq.m_number - 1] = true;
    }
    bs.reset();
  }

  // Check that the subgrids are valid.
  for (unsigned row = 0; row < NUM_DIGITS; row += BOX_SIDE) {
    for (unsigned col = 0; col < NUM_DIGITS; col += BOX_SIDE) {
      if (!check_subgrid(row, col)) {
        m_state = PuzzleState::Invalid_Puzzle;
        return;
      }
    }
  }
}

void Grid::find_possible(grid_t::iterator it) noexcept {
  // Start with all values possible.
  auto& square = *it;
  auto& bs = square.m_possible_values.value();
  bs.set();

  // Constrain by row.
  for (unsigned col = 0; col < NUM_DIGITS; ++col) {
    const auto& sq = at(square.m_row, col);
    if (sq.m_number != 0) bs[sq.m_number - 1] = false;
  }

  // Constrain by column.
  for (unsigned row = 0; row < NUM_DIGITS; ++row) {
    const auto& sq = at(row, square.m_column);
    if (sq.m_number != 0) bs[sq.m_number - 1] = false;
  }

  // Constrain by subgrid.
  const unsigned row_base = square.m_row - (square.m_row % BOX_SIDE);
  const unsigned col_base = square.m_column - (square.m_column % BOX_SIDE);

  for (unsigned row = row_base; row < row_base + BOX_SIDE; ++row) {
    for (unsigned col = col_base; col < col_base + BOX_SIDE; ++col) {
      const auto& sq = at(row, col);
      if (sq.m_number != 0) bs[sq.m_number - 1] = false;
    }
  }
}

bool Grid::solve_helper(grid_t::iterator it) {
  // If we've reached the end of the grid, there's nothing more to do!
  if (it == m_grid.end()) return true;

  // Let the bitset reflect the possible digit values.
  find_possible(it);

  // If the bitset has no possible values for this square, return false.
  if (it->m_possible_values.value().none()) {
    it->m_number = 0;
    return false;
  }

  // Get the possible digits.
  auto digits = it->possible();

  const auto next_it = find_if(next(it), m_grid.end(), is_valid_square);

  for (auto num : digits) {
    // Set the square to this number.
    it->m_number = num;

    // Recursive call - if it returns success, we're done. Otherwise, keep
    // trying digits!
    if (solve_helper(next_it)) return true;
  }
  /*
  By reaching this point, there are one of two options possible.
  Either every digit returned false on the recursive call,
  or the bitset was all 0's (no valid digits) so the loop body
  never executes. In both cases, this square doesn't work.
  Return false and backtrack.
  */
  it->m_number = 0;
  return false;
}

ostream& operator<<(ostream& os, Grid& g) {
  switch (g.m_state) {
    case Grid::PuzzleState::Solution_Exists:
    case Grid::PuzzleState::Unsolved: {
      unsigned index = 0;
      for (unsigned row = 0; row < NUM_DIGITS; ++row) {
        for (unsigned col = 0; col < NUM_DIGITS; ++col) {
          os << +g.m_grid[index++].m_number << ' ';
        }
        os << '\n';
      }
      break;
    }
    case Grid::PuzzleState::No_Solution: {
      os << "There does not exist a solution to this puzzle.\n";
      break;
    }
    case Grid::PuzzleState::Invalid_Puzzle: {
      os << "The given puzzle is invalid.\n";
      break;
    }
  }
  return os;
}

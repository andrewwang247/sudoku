/*
Copyright 2020. Siwei Wang.
*/
#pragma once
#include <bitset>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

// Returns floor of square root of x
constexpr unsigned floor_sqrt(unsigned x) {
  if (x == 0 || x == 1) return x;
  unsigned i = 1;
  for (unsigned result = 1; result <= x; result = i * i) {
    ++i;
  }
  return i - 1;
}

//! Set parameter for NUM_DIGITS. Must be square.

static constexpr unsigned NUM_DIGITS = 9;
static constexpr unsigned BOX_SIDE = floor_sqrt(NUM_DIGITS);
static constexpr unsigned NUM_SQUARES = NUM_DIGITS * NUM_DIGITS;

/**
 * A Square represents each individual position on the Sudoku grid.
 */
class Square {
 public:
  // The row and column coordinates of this Square.
  unsigned m_row, m_column;

  // The current number of this square (0 for unassigned).
  unsigned char m_number;

  // Reports the possible digits where bitset[i] represents whether i + 1 is a
  // valid number.
  std::optional<std::bitset<NUM_DIGITS> > m_possible_values;

  /**
   * Default value of 0 for this square.
   */
  Square() noexcept;

  /**
   * Looks through possible_values and writes down those that are possible.
   * REQUIRES : possible_values.has_value(). Otherwise, std::optional throws.
   */
  std::vector<unsigned char> possible() const;
};

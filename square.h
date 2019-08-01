#pragma once
#include <iostream>
#include <optional>
#include <bitset>
#include <utility>
#include <vector>

constexpr unsigned NUM_DIGITS = 9;
constexpr unsigned BOX_SIDE = NUM_DIGITS / 3;

/**
 * A Square represents each individual position on the Sudoku grid.
 */
class Square {
public:

	// The row and column coordinates of this Square.
	unsigned m_row, m_column;

	// The current number of this square (0 for unassigned).
	unsigned char m_number;

	// Reports the possible digits where bitset[i] represents whether i + 1 is a valid number.
	std::optional< std::bitset<NUM_DIGITS> > m_possible_values;

	// A simple constructor given the coordinates of this square. Default value of 0 if no number is given.
	Square() noexcept;

	/**
	 * Looks through possible_values and writes down those that are possible.
	 * REQUIRES : possible_values.has_value(). Otherwise, std::optional throws.
	 */
	std::vector<unsigned char> possible() const;

	/**
	 * A basic print operator that simply prints out the value associated with it.
	 * Uses unary prefix operator+ to get numeric result of number.
	 */
	friend std::ostream& operator<< ( std::ostream& os, const Square& sq );
};
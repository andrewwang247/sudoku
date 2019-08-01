#pragma once
#include <iostream>
#include <optional>
#include <bitset>
#include <utility>

constexpr unsigned NUM_DIGITS = 9;
constexpr unsigned BOX_SIDE = 3;

/**
 * A Square represents each individual position on the Sudoku grid.
 */
class Square {
private:

	// The (row, column) coordinates of this Square. Could also just be 2 unsigned variables?
	const std::pair< unsigned, unsigned > coordinates;

public:

	// The current number of this square (0 for unassigned).
	unsigned char number;

	// Reports the possible digits where bitset[i] represents whether i + 1 is a valid number.
	std::optional< std::bitset<NUM_DIGITS> > possible_values;

	// A simple constructor given the coordinates of this square. Default value of 0 if no number is given.
	Square( unsigned row, unsigned col, unsigned char num = 0 ) noexcept;

	// Returns the row/column values respectively.
	inline unsigned row() const noexcept;
	inline unsigned column() const noexcept;

	// A basic print operator that simply prints out the value associated with it.
	friend std::ostream& operator<< ( std::ostream& os, const Square& sq );
};
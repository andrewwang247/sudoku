#pragma once
#include "square.h"
#include <array>
#include <iterator>

using row_t = std::array< Square, NUM_DIGITS >;
using grid_t = std::array< row_t, NUM_DIGITS >;

// Forward declaration for Grid class.
class grid_iterator;

class Grid {
using iterator = grid_iterator;
private:

	// Two-dimensional 9-by-9 array of Squares
	grid_t m_grid;

	enum class PuzzleState {
		Solution_Exists, No_Solution, Invalid_Puzzle
	};

public:

	/**
	 * Basic default constructor that sets coordinates on Squares,
	 * initializes all values to 0, and sets state to Solution_Exists.
	 */
	Grid();

	/**
	 * Reads a 9x9 square of digits from is into the grid.
	 * Non-numeric characters are ignored in the input.
	 * Use a capital 'X' for unknown entries.
	 * ASSUMES: g is in just-constructed state.
	 * REPLACES: Grid, state.
	 * THROWS IF : grid cannot be formed.
	 */
	friend std::istream& operator>>( std::istream& is, Grid& g );

	/**
	 * If state == Solution_Exists, prints g to os in a nicely formatted manner.
	 * If state == No_Solution, states that no solution could be found for the puzzle.
	 * If state == Invalid_Puzzle, displays an error message to the screen.
	 */
	friend std::ostream& operator<<( std::ostream& os, Grid& g );

	/**
	 * Interface function that solves the Sudoku grid.
	 * Only solves if state != Invalid_Puzzle.
	 * MODIFIES: grid, solution_exists.
	 */
	void solve();

	// Begin and end iterators.
	inline iterator begin();
	inline iterator end();

private:

	// Contains the state of the puzzle. Should remain relatively unchanged after >>.
	PuzzleState m_state;

	/**
	 * Used at the beginning to ensure that the grid is in a valid state before starting.
	 * If invalid, modifies m_state to the invalid state.
	 */
	void check_state() noexcept;

	/**
	 * Updates the bitset in the Square *it to reflect the possible values *it can take.
	 * @param it: A iterator reference pointing to a square in the grid.
	 * REQUIRES: Grid is in a valid state.
	 * MODIFIES: it->possible_values.
	 */
	void find_possible( iterator it ) noexcept;

	/**
	 * ! This procedure is at the heart of the backtracking algorithm.
	 * Helper function for Grid::solve that allows for recursive backtracking calls.
	 * RETURNS: true if puzzle is solved and false if no solution exists.
	 */
	bool solve_helper ( iterator it );
};

// Iterator class for the grid - leverages std::array's iterator
class grid_iterator : std::iterator< std::forward_iterator_tag, Square > {
private:

	// Contains a Square pointer under the hood that keep track of everything.
	Square* m_ptr;
	// A reference to the underlying grid.
	grid_t& m_grid;

public:
	/**
	 * We need information about the underlying grid container.
	 * This constructor thus takes in both.
	 */
	grid_iterator( decltype(m_ptr) ptr_in, decltype(m_grid) grid_in );

	// Dereference operator
	inline Square& operator*();

	// Comparison operators
	inline bool operator==( const grid_iterator& other ) const;
	inline bool operator!=( const grid_iterator& other ) const;

	// Increment operators to find the next open Square in grid_ref.
	grid_iterator& operator++();
	grid_iterator  operator++(int);
};
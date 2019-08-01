#pragma once
#include "square.h"
#include <array>
#include <iterator>

using grid_t = std::array< std::array< Square, NUM_DIGITS >, NUM_DIGITS >;

class Grid {
private:

	// Two-dimensional 9-by-9 array of Squares
	grid_t grid;

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
	inline void solve();

private:

	// Contains the state of the puzzle. Should remain relatively unchanged after >>.
	PuzzleState state;

	// Iterator class for the grid - leverages std::array's iterator
	class iterator : std::iterator< std::bidirectional_iterator_tag, Square > {
	private:
	
		// A reference to the container we are pointing at.
		Grid& grid_ref;
	
		// Contains two unidimensional iterators under the hood that keep track of everything.
		std::array< std::array< Square, NUM_DIGITS >, NUM_DIGITS >::iterator it_1;
					std::array< Square, NUM_DIGITS >			  ::iterator it_2;
	public:

		// Constructor taking in types. Other ctors are defaulted.
		iterator( decltype(grid_ref) grid_in, decltype(it_1) it_1_in, decltype(it_2) it_2_in );

		// Dereference operator
		Square& operator*();

		// Comparison operators
		inline bool operator==( const iterator& other ) const;
		inline bool operator!=( const iterator& other ) const;

		// Decrement/increment operators find the previous/next open Square in grid_ref.
		iterator& operator++();
		iterator  operator++(int);
		iterator& operator--();
		iterator  operator--(int);
	};

	// Begin and end iterators.
	inline iterator begin() const;
	inline iterator end() const;

private:
	/**
	 * Updates the bitset in the Square *it to reflect the possible values *it can take.
	 * @param it: A Grid::iterator reference pointing to a square in the grid.
	 * REQUIRES: Grid is in a valid state.
	 * MODIFIES: it->possible_values.
	 */
	void find_possible( iterator& it );

	/**
	 * ! This procedure is at the heart of the backtracking algorithm.
	 * Helper function for Grid::solve that allows for recursive backtracking calls.
	 * RETURNS: true if puzzle is solved and false if no solution exists.
	 */
	bool solve_helper ( iterator it );
};
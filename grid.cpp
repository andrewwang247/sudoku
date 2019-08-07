#include "grid.h"
#include <string>
#include <algorithm>
#include <exception>
using namespace std;

/*------ HELPER FUNCTIONS ------*/

/**
 * Returns whether ptr points to an open square or not.
 * REQUIRES: ptr is non-null. Does not check for this!
 */
inline bool is_valid_square( const Square& sq ) {
	return sq.m_possible_values.has_value();
}

/**
 * Checks the 3-by-3 subgrid starting at (row_start, col_start)
 * REQUIRES: bound indexing is correct (ie row_start, col_start <= 6)
 */
bool check_subgrid( unsigned row_start, unsigned col_start, const grid_t& g ) {
	bitset<NUM_DIGITS> bs;
	for ( unsigned row = row_start; row < row_start + BOX_SIDE; ++row ) {
		for ( unsigned col = col_start; col < col_start + BOX_SIDE; ++col ) {
			const auto& sq = g[row][col];
			// Check for any invalid numbers that have gotten in.
			if ( sq.m_number > NUM_DIGITS ) {
				return false;
			}

			// Check that there are no repeats.
			if ( sq.m_number != 0 && bs[sq.m_number - 1] ) {
				return false;
			}
			bs[sq.m_number - 1].flip();
		}
		bs.reset();
	}
	return true;
}

/*------------------------------*/

Grid::Grid() : m_state( PuzzleState::Solution_Exists ) {
	// Get the row and column listings into each square.
	for ( unsigned i = 0; i < NUM_DIGITS; ++i ) {
		for ( unsigned j = 0; j < NUM_DIGITS; ++j ) {
			Square& sq = m_grid[i][j];
			sq.m_row = i;
			sq.m_column = j;
		}
	}
}

inline row_t::iterator Grid::next_valid( row_t::iterator it ) {
	do {
		// If there's still stuff left in the row, increment.
		if (it->m_column < NUM_DIGITS - 1) ++it;
		else if (it->m_row < NUM_DIGITS - 1) {
			// Otherwise, move onto the next row if there's space.
			it = &m_grid[it->m_row + 1][0];
		} else {
			it = m_grid.back().end();
		}
	} while ( it != m_grid.back().end() && !is_valid_square(*it) );
	return it;
}

istream& operator>>( istream& is, Grid& g ) {
	// Read 81 integer valued items from is.
	char buffer;
	for (unsigned row = 0; row < NUM_DIGITS; ++row) {
		for (unsigned col = 0; col < NUM_DIGITS; ++col) {
			// Keep getting chars until digit is found.
			do { is >> buffer; } while (!isdigit(buffer));
			int number = buffer - '0';

			if ( buffer < 0 || static_cast<unsigned>(number) > NUM_DIGITS )
				throw runtime_error("Input Sudoku digit out of bounds.");
			
			g.m_grid[row][col].m_number = static_cast<unsigned char>(number);
			// Recall that the std::optional is default constructed to nullopt in Square.
			if (number == 0) {
				bitset<NUM_DIGITS> bs;
				bs.set();
				g.m_grid[row][col].m_possible_values.emplace(bs);
			}
		}
	}
	// Ensure that the state is valid.
	g.check_state();
	return is;
}

ostream& operator<<( ostream& os, Grid& g ) {
	switch ( g.m_state ) {
	case Grid::PuzzleState::Solution_Exists:
		// Split into groups of 3 rows -- individual row -- every 3 cell group -- every cell.
		for( unsigned row_group = 0; row_group < BOX_SIDE; ++row_group ) {
			cout << "- - - - - - - - - - - - -\n";
			for ( unsigned row_index = 0; row_index < BOX_SIDE; ++row_index ) {
				cout << "| ";
				for ( unsigned col_group = 0; col_group < BOX_SIDE; ++col_group ) {
					for ( unsigned col_index = 0; col_index < BOX_SIDE; ++col_index ) {
						/*
						The resultant row is 3 * row_group + row_index.
						The resultant column is 3 * col_group + col_index
						*/
						const unsigned row = 3 * row_group + row_index;
						const unsigned column = 3 * col_group + col_index;
						cout << g.m_grid[row][column] << ' ';
					}
					cout << "| ";
				}
				cout << '\n';
			}
		}
		cout << "- - - - - - - - - - - - -\n";
		break;
	case Grid::PuzzleState::No_Solution:
		os << "There does not exist a solution to this puzzle.\n";
		break;
	case Grid::PuzzleState::Invalid_Puzzle:
		os << "The given puzzle is invalid.\n";
		break;
	}
	return os;
}

void Grid::solve() {
	// No work needs to be done if the puzzle is invalid.
	if ( m_state == PuzzleState::Invalid_Puzzle ) return;
	// Get the first open square.
	const auto b_it ( m_grid.front().begin() );
	row_t::iterator it = is_valid_square( *b_it ) ? b_it : next_valid( b_it );
	// Fancy ternary operator that sets state based on the result of solve_helper.
	m_state = solve_helper( it ) ? PuzzleState::Solution_Exists : PuzzleState::No_Solution;
}

void Grid::check_state() noexcept {
	bitset<NUM_DIGITS> bs;

	// Check that the rows going across are valid.
	for ( const auto& row : m_grid ) {
		for ( const auto& sq : row ) {
			// Check for any invalid numbers that have gotten in.
			if ( sq.m_number > NUM_DIGITS ) {
				m_state = PuzzleState::Invalid_Puzzle;
				return;
			}

			// Check that there are no repeats.
			if ( sq.m_number != 0 && bs[sq.m_number - 1] ) {
				m_state = PuzzleState::Invalid_Puzzle;
				return;
			}
			bs[sq.m_number - 1].flip();
		}
		bs.reset();
	}

	// Check that the columns going down are valid.
	for ( unsigned col = 0; col < NUM_DIGITS; ++col ) {
		for ( unsigned row = 0; row < NUM_DIGITS; ++row ) {
			const auto& sq = m_grid[row][col];
			// Check for any invalid numbers that have gotten in.
			if ( sq.m_number > NUM_DIGITS ) {
				m_state = PuzzleState::Invalid_Puzzle;
				return;
			}

			// Check that there are no repeats.
			if ( sq.m_number != 0 && bs[sq.m_number - 1] ) {
				m_state = PuzzleState::Invalid_Puzzle;
				return;
			}
			bs[sq.m_number - 1].flip();
		}
		bs.reset();
	}

	// Check that the 9 3-by-3 subgrids are valid.
	for ( unsigned row = 0; row < NUM_DIGITS; row += BOX_SIDE ) {
		for ( unsigned col = 0; col < NUM_DIGITS; col += BOX_SIDE ) {
			if ( !check_subgrid( row, col, m_grid ) ) {
				m_state = PuzzleState::Invalid_Puzzle;
				return;
			}
		} 
	}
}

void Grid::find_possible( row_t::iterator it ) noexcept {
	// Start with all values possible.
	auto& square = *it;
	auto& bs = square.m_possible_values.value();
	bs.set();

	// Constrain by row.
	for ( const auto& sq : m_grid[square.m_row] ) {
		if ( sq.m_number != 0 ) bs[sq.m_number - 1] = 0;
	}

	// Constrain by column.
	for ( unsigned row = 0; row < NUM_DIGITS; ++row ) {
		const auto& sq = m_grid[row][square.m_column];
		if ( sq.m_number != 0 ) bs[sq.m_number - 1] = 0;
	}

	// Constrain by subgrid.
	const unsigned row_base = square.m_row - (square.m_row % BOX_SIDE);
	const unsigned col_base = square.m_column - (square.m_column % BOX_SIDE);
	for ( unsigned row = row_base; row < row_base + BOX_SIDE; ++row ) {
		for ( unsigned col = col_base; col < col_base + BOX_SIDE; ++col ) {
			const auto& sq = m_grid[row][col];
			if ( sq.m_number != 0 ) bs[sq.m_number - 1] = 0;
		}
	}
}

bool Grid::solve_helper( row_t::iterator it ) {
	// If we've reached the end of the grid, there's nothing more to do!
	if ( it == m_grid.back().end() ) return true;

	// Let the bitset reflect the possible digit values.
	find_possible( it );

	// If the bitset has no possible values for this square, return false.
	if ( it->m_possible_values.value().none() ) return false;

	// Get the possible digits.
	auto digits = it->possible();
	// Record the current iterator and find next valid.
	auto original_it = it;
	const auto next_it ( next_valid(it) );

	for ( auto num : digits ) {
	
		// Set the square to this number.
		original_it->m_number = num;

		// Recursive call - if it returns success, we're done. Otherwise, keep trying digits!
		if ( solve_helper( next_it ) ) return true;
	}
	/*
    By reaching this point, there are one of two options possible.
	Either every digit returned false on the recursive call,
	or the	bitset was all 0's (no valid digits) so the loop body
	never executes. In both cases, this square doesn't work.
	Return false and backtrack.
    */
    return false;
}
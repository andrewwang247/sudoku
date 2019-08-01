#include "grid.h"
using namespace std;

Grid::Grid() {
	// Get the row and column listings into each square.
	for ( unsigned i = 0; i < NUM_DIGITS; ++i ) {
		for ( unsigned j = 0; j < NUM_DIGITS; ++j ) {
			Square& sq = grid[i][j];
			sq.m_row = i;
			sq.m_column = j;
		}
	}
}

void Grid::solve() {
	// No work needs to be done if the puzzle is invalid.
	if ( state == PuzzleState::Invalid_Puzzle ) return;
	// Fancy ternary operator that sets state based on the result of solve_helper.
	state = solve_helper( begin() ) ? PuzzleState::Solution_Exists : PuzzleState::No_Solution;
}
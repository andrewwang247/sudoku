#include "grid.h"
#include <exception>
using namespace std;

int main() {
	ios_base::sync_with_stdio( false );

	while ( cin ) {
		Grid sudoku ( cin );
		cout << "Input Puzzle:\n" << sudoku;
		sudoku.solve();
		cout << "Computer Solution:\n" << sudoku;
	}
}
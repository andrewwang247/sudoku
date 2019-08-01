#include "grid.h"
#include <exception>
using namespace std;

int main() {
	ios_base::sync_with_stdio( false );

	for ( Grid sudoku; cin >> sudoku; ) {
		sudoku.solve();
		cout << sudoku;
	}
}
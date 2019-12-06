#include "grid.h"
#include <exception>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main() {
	ios_base::sync_with_stdio( false );

	while ( cin ) {
		Grid sudoku ( cin );
		cout << "Input Puzzle:\n" << sudoku;
		const auto start_time = high_resolution_clock::now();
		sudoku.solve();
		const auto end_time = high_resolution_clock::now();
		cout << "Computer Solution:\n" << sudoku;
		const auto duration = chrono::duration_cast<microseconds>(end_time - start_time).count();
		cout << "Time to solve: " << duration << " microseconds.\n";
	}
}
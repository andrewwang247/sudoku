/*
Copyright 2020. Siwei Wang.
*/
#include <chrono>
#include <exception>
#include <fstream>
#include "grid.h"
using std::cout;
using std::ifstream;
using std::ios_base;
using std::runtime_error;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

int main(int argc, char** argv) {
  ios_base::sync_with_stdio(false);
  if (argc != 2) {
    throw runtime_error("Usage ./sudoku puzzle.txt > solution.txt");
  }
  ifstream fin(argv[1]);

  Grid sudoku(fin);
  cout << "Input Puzzle:\n" << sudoku;
  const auto start_time = high_resolution_clock::now();
  sudoku.solve();
  const auto end_time = high_resolution_clock::now();
  cout << "Computer Solution:\n" << sudoku;
  const auto duration =
      duration_cast<microseconds>(end_time - start_time).count();
  cout << "Time to solve: " << duration << " microseconds.\n";
}

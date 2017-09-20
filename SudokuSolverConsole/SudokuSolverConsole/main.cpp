#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <iostream>
#include <utility>

int main()
{
	SudokuSolver solver;
	SudokuBoard board;
	solver.readFile("input.txt", board);
	std::cout << board.toString();
	solver.dfs(board);
	std::cout << std::endl << board.toString();
	int x;
	std::cin >> x;
	return 0;
}
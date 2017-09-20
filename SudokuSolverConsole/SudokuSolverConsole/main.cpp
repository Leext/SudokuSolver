#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <iostream>
#include <utility>
#include <cassert>
#include <ctime>
int main()
{
	SudokuSolver solver;
	SudokuBoard board;
	board[0][0] = 4;
	clock_t t = clock();
	SudokuBoard *c;
	for (int i = 0; i < 20; i++)
	{
		c = &solver.generate(board);
		assert(solver.check(*c));
		std::cout << c->toString() << std::endl;
		delete c;
	}
	t = clock() - t;
	std::cout << "time comsumed" << ((double)t) / CLOCKS_PER_SEC << std::endl;
	int x;
	std::cin >> x;
	return 0;
}
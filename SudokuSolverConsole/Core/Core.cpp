// Core.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SudokuSolver.h"

extern "C" _declspec(dllexport) void generateM(int number, int mode, int result[][81])
{
	if (number > 10000 || number < 1)
		throw std::invalid_argument("invalid number");
	SudokuSolver::generate(number, mode, result);
}

extern "C" _declspec(dllexport) void generateR(int number, int lower, int upper, bool unique, int result[][81])
{
	if (number > 10000 || number < 1)
		throw std::invalid_argument("invalid number");
	SudokuSolver::generate(number, lower, upper, unique, result);
}

extern "C" _declspec(dllexport) bool solve(int puzzle[], int solution[])
{
	for (int i = 0; i < 81; i++)
		if (puzzle[i] > 9 || puzzle[i] < 0)
			throw std::invalid_argument("invalid puzzle");
	return SudokuSolver::solve(puzzle, solution);
}

extern "C" _declspec(dllexport) bool check(int board[81])
{
	SudokuBoard b(board);
	return SudokuSolver::check(b);
}

extern "C" _declspec(dllexport) int getFeasible(int board[81], int x, int y)
{
	SudokuBoard b(board);
	return b.getFeasible(x, y);
}

extern "C" _declspec(dllexport) const char* generateN(int n)
{
	SudokuBoard board;
	SudokuSolver solver;
	board.set(0, 0, 4);
	return (solver.generateN(n, board).c_str());
}

extern "C" _declspec(dllexport) int* generate(int mode)
{
	int r[1][81];
	SudokuSolver::generate(1, mode, r);
	int *result = new int[81];
	memcpy(result, r[0], sizeof(int) * 81);
	return result;
}
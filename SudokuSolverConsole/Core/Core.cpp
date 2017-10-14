// Core.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SudokuSolver.h"

extern _declspec(dllexport) void generate(int number, int mode, int result[][81])
{
	if (number > 10000 || number < 1)
		throw std::invalid_argument("invalid number");
	SudokuSolver::generate(number, mode, result);
}

extern _declspec(dllexport) void generate(int number, int lower, int upper, bool unique, int result[][81])
{
	if (number > 10000 || number < 1)
		throw std::invalid_argument("invalid number");
	SudokuSolver::generate(number, lower, upper, unique, result);
}

extern _declspec(dllexport) bool solve(int puzzle[], int solution[])
{
	for (int i = 0; i < 81; i++)
		if (puzzle[i] > 9 || puzzle[i] < 0)
			throw std::invalid_argument("invalid puzzle");
	return SudokuSolver::solve(puzzle, solution);
}

extern _declspec(dllexport) bool check(int board[81])
{
	SudokuBoard b(board);
	return SudokuSolver::check(b);
}

extern _declspec(dllexport) int getFeasible(int board[81], int x, int y)
{
	SudokuBoard b(board);
	return b.getFeasible(x, y);
}

extern _declspec(dllexport) char* generateN(int n)
{
	SudokuBoard board;
	SudokuSolver solver;
	board.set(0, 0, 4);
	std::string str = solver.generateN(n, board);
	char *r = new char[str.size() + 1];
	memcpy(r, str.c_str(), str.size());
	return r;
}

extern _declspec(dllexport) void generate(int number, int result[][81])
{
	if (number < 1 || number>1000000)
		throw std::invalid_argument("invalid number");
	SudokuSolver::generate(number, result);
}
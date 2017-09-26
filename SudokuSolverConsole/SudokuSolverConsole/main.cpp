#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <utility>
#include <cassert>
#include <ctime>
#include <iostream>
#include <cstdlib>

int copeGenerate(char* arg);
int copeSolve(char* arg);
void test();
int main(int argc, char **argv)
{
	if (argc < 3)
	{
		test();
		return 0;
	}
	if (strcmp("-c", argv[1]) == 0)
		return copeGenerate(argv[2]);
	else if (strcmp("-s", argv[1]) == 0)
		return copeSolve(argv[2]);
	return 0;
}


int copeGenerate(char* arg)
{
	SudokuBoard board;
	SudokuSolver solver;
	FILE* output = fopen("sudoku.txt", "w");
	if (output == NULL)
	{
		printf("cannot open sudoku.txt");
		return 0;
	}
	int n;
	try
	{
		n = std::stoi(arg);
		printf("%d\n", n);
	}
	catch (...)
	{
		printf("invalid argument %s", arg);
		return 0;
	}
	board.set(0, 0, 4);
	fprintf(output, solver.generateN(n, board).c_str());
	fclose(output);
	return 0;
}

int copeSolve(char* arg)
{
	SudokuSolver solver;
	auto puzzles = solver.readFile(arg);
	FILE* output = fopen("sudoku.txt", "w");
	if (output == NULL)
	{
		printf("cannot open sudoku.txt");
		return 0;
	}
	for (auto p : puzzles)
	{
		SudokuBoard *s = solver.solve(*p);
		fprintf(output, "%s\n", (*(s->toString())).c_str());
		delete s;
	}
	fclose(output);
	return 0;
}

void test()
{
	SudokuBoard board = SudokuBoard(std::string("012345678000000000000000000000000000000000000000000000000000000000000000000000000"));
	assert((1 << 8) == (board.getFeasible(0, 0) >> 1));
	assert(1 == board.countFeasible(0, 0));
	board = SudokuBoard(std::string("012345678900000000000000000000000000000000000000000000000000000000000000000000000"));
	assert(0 == board.countFeasible(0, 0));
	board = SudokuBoard(std::string("012300000400000000507000000000000000000000000600000000000000000600000000000000000"));
	assert(2 == board.countFeasible(0, 0));
	auto p = board.findFewest();
	assert(0 == p.first && 0 == p.second);
	board = SudokuBoard(std::string("000000010400000000020000000000050407008000300001090000300400200050100000000806000"));
	SudokuSolver solver;
	SudokuBoard *b = solver.solve(board);
	assert(solver.check(*b));
	solver.generate(board);
	solver.generateN(3, board);
	copeSolve("a.txt");
	copeGenerate("10");
}
#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <utility>
#include <cassert>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <unordered_set>

int copeGenerate(char* arg);
int copeSolve(char* arg);
void test();
int result[10000][81];
int main(int argc, char **argv)
{
	SudokuBoard b = SudokuBoard(std::string("000000010400000000020000000000050604008000300001090000300400200050100000000807000"));
	SudokuSolver::evalDifficulty(b);
	int r[81];
	srand(time(NULL));
	SudokuSolver::generate(10000, 33, 44, false, result);
	printf("end");
	//SudokuSolver::generate(10000, 2, result);
	//for (int i = 0; i < 100; i++)
	//{
	//	for (int j = 0; j < 81; j++)
	//		printf("%d", result[i][j]);
	//	printf("\n");
	//}
	int t;
	std::cin >> t;
	if (argc > 2)
	{
		if (strcmp("-c", argv[1]) == 0)
			return copeGenerate(argv[2]);
		else if (strcmp("-s", argv[1]) == 0)
			return copeSolve(argv[2]);
	}
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

	board = SudokuBoard(std::string("000000010400000000020000000000050604008000300001090000300400200050100000000807000"));
	b = solver.solve(board);
	assert(solver.check(*b));

	board = SudokuBoard(std::string("000000012003600000000007000410020000000500300700000600280000040000300500000000000"));
	b = solver.solve(board);
	assert(solver.check(*b));

	board = SudokuBoard(std::string("000000012008030000000000040120500000000004700060000000507000300000620000000100000"));
	b = solver.solve(board);
	assert(solver.check(*b));

	board = SudokuBoard(std::string("000000012040050000000009000070600400000100000000000050000087500601000300200000000"));
	b = solver.solve(board);
	assert(solver.check(*b));

	board = SudokuBoard(std::string("000000012050400000000000030700600400001000000000080000920000800000510700000003000"));
	b = solver.solve(board);
	assert(solver.check(*b));

	board = SudokuBoard(std::string("000000013000030080070000200000206000030000900000010000600500204000400700100000000"));
	b = solver.solve(board);
	assert(b == NULL);

	//solver.generate(board);
	solver.generateN(3, board);
	copeSolve("a.txt");
	copeGenerate("10");
}
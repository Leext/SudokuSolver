#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <utility>
#include <cassert>
#include <ctime>

int copeGenerate(char* arg);
int copeSolve(char* arg);
int main(int argc, char **argv)
{
	if (argc < 3)
	{
		copeGenerate("1000000");
		//copeSolve("b.txt");
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
	SudokuBoard board;
	SudokuSolver solver;
	if (!solver.readFile(arg, board))
		return 0;
	FILE* output = fopen("sudoku.txt", "w");
	if (output == NULL)
	{
		printf("cannot open sudoku.txt");
		return 0;
	}
	fprintf(output, (*solver.solve(board).toString()).c_str());
	fclose(output);
	return 0;
}
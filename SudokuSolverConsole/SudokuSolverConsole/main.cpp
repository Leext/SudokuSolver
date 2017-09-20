#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <utility>
#include <cassert>
#include <ctime>
int main(int argc,char **argv)
{
	if (argc < 3)
	{
		printf("not enough arguments!\n");
		return 0;
	}
	SudokuSolver solver;
	SudokuBoard board;
	if (strcmp("-c", argv[1]) == 0)
	{
		FILE* output = fopen("sudoku.txt", "w");
		if (output == NULL)
		{
			printf("cannot open sudoku.txt");
			return 0;
		}
		int n;
		try
		{
			n = std::stoi(argv[2]);
		}
		catch (...)
		{
			printf("invalid argument %s", argv[2]);
			return 0;
		}
		board.set(0, 0, 4);
		fprintf(output, solver.generateN(n, board).c_str());
		fclose(output);
	}
	else if (strcmp("-s", argv[1]) == 0)
	{
		if (!solver.readFile(argv[2], board))
			return 0;
		FILE* output = fopen("sudoku.txt", "w");
		if (output == NULL)
		{
			printf("cannot open sudoku.txt");
			return 0;
		}
		fprintf(output, solver.solve(board).toString().c_str());
		fclose(output);
	}
	
	return 0;
}
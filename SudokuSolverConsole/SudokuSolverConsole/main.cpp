#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <utility>
#include <cassert>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <unordered_set>
#include "test.h"

int copeGenerate(char* arg);
int copeSolve(char* arg);
int result[10000][81];
int main(int argc, char **argv)
{
	int r[81], s[81];

	//testBase();
	//testArgM();
	//testArgR();
	SudokuSolver::generate(10000, 55, 55, true, result);
	//FILE *output = fopen("output.txt", "w");
	//for (int i = 0; i < 1000; i++)
	//{
	//	for (int j = 0; j < 81; j++)
	//		fprintf(output,"%d", result[i][j]);
	//	fprintf(output, "\n");
	//}
	//fclose(output);
	printf("Done\n");
	int t;
	//std::cin >> t;
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
	std::vector<std::shared_ptr<SudokuBoard>> puzzles;
	solver.readFile(arg, puzzles);
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


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
int main(int argc, char **argv)
{
	//if (argc < 3)
	//{
	//	//copeGenerate("1000000");
	//	//copeSolve("b.txt");
	//	FILE *input = fopen("sudoku17.txt", "r");
	//	char buffer[83];
	//	SudokuSolver solver;
	//	std::vector<std::shared_ptr<std::string>> solu;
	//	int c = 0;
	//	while (fgets(buffer, 83, input) != NULL)
	//	{
	//		SudokuBoard board = SudokuBoard(std::string(buffer));
	//		SudokuBoard *so;
	//		so = solver.solve(board);
	//		if (so == NULL)
	//		{
	//			std::cout << "!!!!!!!!!!\n";
	//			continue;
	//		}
	//		auto s = so->toString();
	//		solu.push_back(s);
	//		if (c++ > 2000)
	//			break;
	//	}
	//	FILE *output = fopen("output.txt", "w");
	//	for (auto& s : solu)
	//		fprintf(output, (*s).c_str());
	//	return 0;
	//}
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
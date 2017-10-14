#pragma once
#include <cassert>
#include "SudokuSolver.h"

int re[10000][81];
int sol[81];
void testBase()
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

	int result[10][81];
	SudokuSolver::generate(10, result);
	for (int i = 0; i < 10; i++)
		assert(SudokuSolver::check(SudokuBoard(result[i])));
}
void testArgM()
{
	printf("test generate(10000, 1, result)\n");
	SudokuSolver::generate(10000, 1, re);
	for (int i = 0; i < 10000; i++)
	{
		SudokuSolver::solve(re[i], sol);
		if(!SudokuSolver::check(SudokuBoard(sol)))
			printf("fail\n");
	}
	printf("test generate(1000, 2, result)\n");
	SudokuSolver::generate(1000, 2, re);
	for (int i = 0; i < 1000; i++)
	{
		SudokuSolver::solve(re[i], sol);
		if(!SudokuSolver::check(SudokuBoard(sol)))
			printf("fail\n");
	}
	printf("test generate(100, 3, result)\n");
	SudokuSolver::generate(1000, 3, re);
	for (int i = 0; i < 1000; i++)
	{
		SudokuSolver::solve(re[i], sol);
		if(!SudokuSolver::check(SudokuBoard(sol)))
			printf("fail\n");
	}
}
void testArgR()
{
	printf("test generate(10000,20,55,false,result)\n");
	SudokuSolver::generate(10000, 20, 55, false, re);
	for (int i = 0; i < 10000; i++)
	{
		SudokuSolver::solve(re[i], sol);
		if(!SudokuSolver::check(SudokuBoard(sol)))
			printf("fail\n");
	}
	printf("test generate(10000,20,55,true,result)\n");
	SudokuSolver::generate(10000, 20, 55, true, re);
	for (int i = 0; i < 10000; i++)
	{
		if (!SudokuSolver::isU(SudokuBoard(re[i])))
			printf("fail\n");
		SudokuSolver::solve(re[i], sol);
		if(!SudokuSolver::check(SudokuBoard(sol)))
			printf("fail\n");
	}

}

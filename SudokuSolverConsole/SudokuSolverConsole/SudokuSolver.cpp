#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>

SudokuSolver::SudokuSolver()
{
	srand((unsigned int)(time(NULL)));
}

SudokuSolver::~SudokuSolver()
{
}

bool SudokuSolver::readFile(char fileName[], SudokuBoard &board)
{
	FILE *input = fopen(fileName, "r");
	if (input == NULL)
	{
		printf("invalid file path %s", fileName);
		return false;
	}
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			fscanf(input, "%d", &board[i][j]);
	fclose(input);
	return true;
}

std::vector<std::shared_ptr<SudokuBoard>> SudokuSolver::readFile(char fileName[])
{
	std::vector<std::shared_ptr<SudokuBoard>> rtn;
	FILE *input = fopen(fileName, "r");
	if (input == NULL)
	{
		printf("invalid file path %s", fileName);
		return rtn;
	}
	char buf[19];
	while (true)
	{
		auto board = std::shared_ptr<SudokuBoard>(new SudokuBoard());
		for (int i = 0; i < 9; i++)
		{
			fgets(buf, 19, input);
			for (int j = 0; j < 9; j++)
				(*board)[i][j] = buf[j * 2] - '0';
		}
		rtn.push_back(board);
		if (fgets(buf, 19, input) == NULL)
			break;
	}
	fclose(input);
	return rtn;
}

bool SudokuSolver::check(SudokuBoard &board)
{
	const int complete = 0x3fe;
	int temp;
	//for lines
	for (int i = 0; i < 9; i++)
	{
		temp = 0;
		for (int j = 0; j < 9; j++)
			temp |= 1 << board[i][j];
		if (temp != complete)
			return false;
	}
	//for columns
	for (int j = 0; j < 9; j++)
	{
		temp = 0;
		for (int i = 0; i < 9; i++)
			temp |= 1 << board[i][j];
		if (temp != complete)
			return false;
	}
	//for grids
	for (int grid = 0; grid < 9; grid++)
	{
		int start_x = (grid % 3) * 3;
		int start_y = grid - (grid % 3);
		temp = 0;
		for (int i = start_x; i < start_x + 3; i++)
			for (int j = start_y; j < start_y + 3; j++)
				temp |= 1 << board[i][j];
		if (temp != complete)
			return false;
	}
	return true;
}

bool SudokuSolver::dfs(SudokuBoard &board)
{
	std::pair<int, int> &target = board.findFewest();
	if (target.first == -1) // end
	{
		_solveCount++;
		solutions->push_back(board.toString());
		return _solveCount >= _solveLimit;
	}
	if (target.second == -1) // no solution
		return false;
	int feasible = board.getFeasible(target.first, target.second);
	for (int i = 1; i <= 10; i++)
	{
		if ((feasible >> i) & 1)
		{
			board.set(target, i);
			if (dfs(board))
				return true;
		}
	}
	board.set(target, 0);
	return false;
}

SudokuBoard *SudokuSolver::solve(SudokuBoard &board)
{
	SudokuBoard *r = new SudokuBoard(board);
	_solveCount = 0;
	_solveLimit = 1;
	solutions = new std::vector<std::shared_ptr<std::string>>();
	if (!dfs(*r))
		return NULL;
	return r;
}
void SudokuSolver::generate(SudokuBoard &board)
{
	SudokuBoard *r = new SudokuBoard(board);
	int x, y;
	for (int i = 0; i < 0; i++)
	{
		x = rand() % 9;
		y = rand() % 9;
		if ((*r)[x][y] == 0)
		{
			auto solveVector = r->getSolveVector(x, y);
			(*r)[x][y] = solveVector[rand() % solveVector.size()];
			//delete &solveVector;
		}
	}
	dfs(*r);
}

std::string SudokuSolver::generateN(int n, SudokuBoard &board)
{
	std::string r;
	//for (int i = 0; i < n; i++)
	//r += generate(board).toString() + '\n';
	_solveCount = 0;
	_solveLimit = n;
	solutions = new std::vector<std::shared_ptr<std::string>>();
	generate(board);
	for (auto &s : *solutions)
		r += *s + '\n';
	delete solutions;
	return r;
}
int SudokuBoard::getFeasible(int x, int y)
{
	int bit = 0;
	const int complete = 0x3fe;
	for (int i = 0; i < 9; i++)
		bit |= 1 << _board[i][y];
	for (int j = 0; j < 9; j++)
		bit |= 1 << _board[x][j];
	int start_x = x / 3 * 3;
	int start_y = y / 3 * 3;
	for (int i = start_x; i < start_x + 3; i++)
		for (int j = start_y; j < start_y + 3; j++)
			bit |= 1 << _board[i][j];
	return bit ^ complete;
}
int SudokuBoard::countFeasible(int x, int y)
{
	// _board[x][y] must be 0
	int bit = getFeasible(x, y) >> 1;
	int count = 0;
	while (bit)
	{
		bit &= (bit - 1);
		count++;
	}
	return count;
}

std::pair<int, int> SudokuBoard::findFewest()
{
	int min = 100;
	int temp;
	std::pair<int, int> minPair(-1, 0);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (_board[i][j] == 0)
			{
				temp = countFeasible(i, j);
				if (temp == 0)
					return std::pair<int, int>(0, -1);
				if (temp < min)
				{
					min = temp;
					minPair.first = i;
					minPair.second = j;
				}
			}
	return minPair;
}

std::vector<int> &SudokuBoard::getSolveVector(int x, int y)
{
	int feasilbe = getFeasible(x, y);
	std::vector<int> *rtn = new std::vector<int>;
	for (int i = 1; i < 10; i++)
		if ((feasilbe >> i) & 1)
			rtn->push_back(i);
	return *rtn;
}

std::shared_ptr<std::string> SudokuBoard::toString()
{
	auto rtn = std::shared_ptr<std::string>(new std::string());
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			(*rtn).push_back(_board[i][j] + '0');
			(*rtn).push_back(j == 8 ? '\n' : ' ');
		}
	return rtn;
}
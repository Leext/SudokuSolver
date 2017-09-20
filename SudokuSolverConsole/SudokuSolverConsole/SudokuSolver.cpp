#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <stack>


SudokuSolver::SudokuSolver()
{
}


SudokuSolver::~SudokuSolver()
{
}

void SudokuSolver::readFile(char fileName[], SudokuBoard &board)
{
	FILE* input = fopen(fileName, "r");
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			fscanf(input, "%d", &board[i][j]);
	fclose(input);
}

bool SudokuSolver::check(SudokuBoard& board)
{
	const int complete = 0x1ff;
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
	for (int j = 0; j < 0; j++)
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

bool SudokuSolver::dfs(SudokuBoard& board)
{
	std::pair<int, int>& target = board.findFewest();
	if (target.first == -1) // end
		return true;
	if (target.second == -1) // no solution
		return false;
	auto solveVector = board.getSolveVector(target);
	for (auto s : solveVector)
	{
		board.set(target, s);
		if (dfs(board))
			return true;
	}
	board.set(target, 0);
}

bool *SudokuBoard::getBanArray(int x, int y)
{
	bool *banArray = new bool[10];
	for (int i = 0; i < 10; i++)
		banArray[i] = false;
	for (int i = 0; i < 9; i++)
		banArray[_board[i][y]] = true;
	for (int j = 0; j < 9; j++)
		banArray[_board[x][j]] = true;
	int start_x = x / 3 * 3;
	int start_y = y / 3 * 3;
	for (int i = start_x; i < start_x + 3; i++)
		for (int j = start_y; j < start_y + 3; j++)
			banArray[_board[i][j]] = true;
	return banArray;
}
int SudokuBoard::countPossibilties(int x, int y)
{
	// _board[x][y] must be 0
	bool *banArray = getBanArray(x, y);
	int count = 0;
	for (int i = 1; i < 10; i++)
		if (!banArray[i])
			count++;
	delete[] banArray;
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
				temp = countPossibilties(i, j);
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

std::vector<int>& SudokuBoard::getSolveVector(int x, int y)
{
	bool *banArray = getBanArray(x, y);
	std::vector<int>* rtn = new std::vector<int>;
	for (int i = 1; i < 10; i++)
		if (!banArray[i])
			rtn->push_back(i);
	delete banArray;
	return *rtn;
}

std::string& SudokuBoard::toString()
{
	std::string *r = new std::string();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			r->push_back(_board[i][j] + '0');
			r->push_back(j == 8 ? '\n' : ' ');
		}
	return *r;
}
#pragma once
#include <string>
#include <utility>
#include <vector>
class SudokuBoard
{
public:
	int * operator[](const int i)
	{
		return _board[i];
	}
	SudokuBoard()
	{}

	SudokuBoard(SudokuBoard& c)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				(*this)[i][j] = c[i][j];
	}
	void set(int x, int y, int value)
	{
		_board[x][y] = value;
	}
	void set(std::pair<int, int> &p, int value)
	{
		_board[p.first][p.second] = value;
	}
	bool *getBanArray(int x, int y);
	int countPossibilties(int x, int y);
	std::pair<int, int> findFewest();
	std::vector<int>& getSolveVector(int x, int y);
	std::vector<int>& getSolveVector(std::pair<int, int> grid)
	{
		return getSolveVector(grid.first, grid.second);
	}
	std::string& toString();
private:
	int _board[9][9];
};
class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	static void readFile(char fileName[], SudokuBoard &board);
	static bool check(SudokuBoard& board);
	SudokuBoard& solve(SudokuBoard& board);
	bool dfs(SudokuBoard& board);
	SudokuBoard& generate(SudokuBoard& board);
private:
	SudokuBoard *solution;
};


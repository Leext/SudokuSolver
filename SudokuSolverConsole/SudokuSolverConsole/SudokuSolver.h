#pragma once
#include <string>
#include <utility>
#include <vector>
#include <memory>
class SudokuBoard
{
public:
	int * operator[](const int i)
	{
		return _board[i];
	}
	SudokuBoard()
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				_board[i][j] = 0;
	}

	SudokuBoard(SudokuBoard& c)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				(*this)[i][j] = c[i][j];
	}

	SudokuBoard(std::string &s)
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				_board[i][j] = s[i * 9 + j] - '0';
	}
	void set(int x, int y, int value)
	{
		_board[x][y] = value;
	}
	void set(std::pair<int, int> &p, int value)
	{
		_board[p.first][p.second] = value;
	}
	int getFeasible(int x, int y);
	int countFeasible(int x, int y);
	std::pair<int, int> findFewest();
	std::vector<int>& getSolveVector(int x, int y);
	std::vector<int>& getSolveVector(std::pair<int, int> grid)
	{
		return getSolveVector(grid.first, grid.second);
	}
	std::shared_ptr<std::string> toString();
private:
	int _board[9][9];
};

class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	static bool readFile(char fileName[], SudokuBoard &board);
	static std::vector<std::shared_ptr<SudokuBoard>> readFile(char fileName[]);
	static bool check(SudokuBoard& board);
	SudokuBoard* solve(SudokuBoard& board);
	bool dfs(SudokuBoard& board);
	void generate(SudokuBoard& board);
	std::string generateN(int n, SudokuBoard& board);
private:
	SudokuBoard *solution;
	FILE *output;
	int _solveCount, _solveLimit;
	std::vector<std::shared_ptr<std::string>>* solutions;
};


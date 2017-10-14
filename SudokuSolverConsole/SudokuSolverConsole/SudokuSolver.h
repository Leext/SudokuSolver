#pragma once
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <cstring>
class SudokuBoard
{
public:
	int& operator[](const int i)
	{
		return _board[i];
	}
	int loc(int x, int y) const
	{
		return _board[x * 9 + y];
	}
	int loc(int x) const
	{
		return _board[x];
	}
	SudokuBoard()
	{
		for (int i = 0; i < 81; i++)
			_board[i] = 0;
	}
	SudokuBoard(const SudokuBoard& c)
	{
		memcpy(_board, c._board, sizeof(int) * 81);
	}

	SudokuBoard(std::string &s)
	{
		for (int i = 0; i < 81; i++)
			_board[i] = s[i] - '0';
	}
	SudokuBoard(int b[81])
	{
		memcpy(_board, b, sizeof(int) * 81);
	}
	void copyTo(int dst[81])
	{
		memcpy(dst, _board, sizeof(int) * 81);
	}
	void copyTo(SudokuBoard &dst)
	{
		memcpy(dst._board, _board, sizeof(int) * 81);
	}
	void set(int x, int y, int value)
	{
		_board[x * 9 + y] = value;
	}
	void set(std::pair<int, int> &p, int value)
	{
		_board[p.first * 9 + p.second] = value;
	}
	void clear()
	{
		memset(_board, 0, sizeof(int) * 81);
	}
	int getFeasible(int x, int y);
	int countFeasible(int x, int y);
	int countFeasible(int i)
	{
		return countFeasible(i / 9, i % 9);
	}
	std::pair<int, int> findFewest();
	int findMost();
	int findF();
	std::vector<int>& getSolveVector(int x, int y);
	std::vector<int>& getSolveVector(std::pair<int, int> grid)
	{
		return getSolveVector(grid.first, grid.second);
	}
	int getRandFeasible(int x, int y);
	std::shared_ptr<std::string> toString();
	static std::string toLineString(int b[81])
	{
		std::string r(81, 0);
		for (int i = 0; i < 81; i++)
			r[i] = b[i];
		return r;
	}
	std::string toLineString()
	{
		return toLineString(_board);
	}
private:
	int _board[81];
};

class SudokuSolver
{
public:
	SudokuSolver();
	~SudokuSolver();
	static bool readFile(char fileName[], SudokuBoard &board);
	static void readFile(char fileName[],std::vector<std::shared_ptr<SudokuBoard>>&);
	static void readFileLine(char fileName[], std::vector<std::shared_ptr<SudokuBoard>>&);
	static bool check(const SudokuBoard& board);
	SudokuBoard* solve(SudokuBoard& board);
	bool dfs(SudokuBoard& board);
	void generate(SudokuBoard& board, int initNum);
	std::string generateN(int n, SudokuBoard& board);
	static bool isU(SudokuBoard board);
	static void generate(int number, int mode, int result[][81]);
	static void generate(int number, int lower, int upper, bool unique, int result[][81]);
	static bool solve(int puzzle[], int solution[]);
	static int loc(int x, int y)
	{
		return x * 9 + y;
	}
private:
	SudokuBoard *solution;
	FILE *output;
	int _solveCount, _solveLimit;
	std::vector<std::shared_ptr<std::string>>* solutions;
	static void generateHard(int number, int result[][81]);
	static void generateU(int n, int result[81]);
	static void generate(int blank, int result[81]);
	static bool search(SudokuBoard& board, int&);
	static void makeBlank(SudokuBoard& board, int num);
	static void generateFinal(int initNum, int result[81]);
	static void generateFinal(int initNum, SudokuBoard& result);
	static bool fill(SudokuBoard& board, int &tryCount);
	static double evalDifficulty(SudokuBoard board);
};


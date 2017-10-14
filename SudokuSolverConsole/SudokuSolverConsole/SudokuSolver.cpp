#define _CRT_SECURE_NO_WARNINGS
#include "SudokuSolver.h"
#include <cstdio>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <cassert>
#include <unordered_set>

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
	int t;
	for (int i = 0; i < 81; i++)
	{
		fscanf(input, "%d", &t);
		board[i] = t;
	}
	fclose(input);
	return true;
}

void SudokuSolver::readFile(char fileName[], std::vector<std::shared_ptr<SudokuBoard>>& result)
{
	FILE *input = fopen(fileName, "r");
	if (input == NULL)
	{
		printf("invalid file path %s", fileName);
		return;
	}
	char buf[19];
	while (true)
	{
		auto board = std::shared_ptr<SudokuBoard>(new SudokuBoard());
		for (int i = 0; i < 9; i++)
		{
			fgets(buf, 19, input);
			for (int j = 0; j < 9; j++)
				//(*board)[i][j] = buf[j * 2] - '0';
				(*board).set(i, j, buf[j * 2] - '0');
		}
		result.push_back(board);
		if (fgets(buf, 19, input) == NULL)
			break;
	}
	fclose(input);
}
void SudokuSolver::readFileLine(char fileName[], std::vector<std::shared_ptr<SudokuBoard>>& result)
{
	FILE *input = fopen(fileName, "r");
	if (input == NULL)
	{
		std::string e("invalid file path ");
		e += fileName;
		throw std::invalid_argument(e);
		return;
	}
	char buf[100];
	while (true)
	{
		auto r = fgets(buf, 100, input);
		if (r == NULL)
			break;
		auto board = std::shared_ptr<SudokuBoard>(new SudokuBoard());
		for (int i = 0; i < 81; i++)
			(*board)[i] = buf[i] - '0';
		result.push_back(board);
	}
}
bool SudokuSolver::check(const SudokuBoard &board)
{
	const int complete = 0x3fe;
	int temp;
	//for lines
	for (int i = 0; i < 9; i++)
	{
		temp = 0;
		for (int j = 0; j < 9; j++)
			temp |= 1 << board.loc(i, j);
		if (temp != complete)
			return false;
	}
	//for columns
	for (int j = 0; j < 9; j++)
	{
		temp = 0;
		for (int i = 0; i < 9; i++)
			temp |= 1 << board.loc(i, j);
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
				temp |= 1 << board.loc(i, j);
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
bool SudokuSolver::fill(SudokuBoard &board, int& tryCount)
{
	if (tryCount > 100000)
		return false;
	std::pair<int, int> &target = board.findFewest();
	if (target.first == -1) // end
		return true;
	if (target.second == -1) // no solution
		return false;
	int feasible = board.getFeasible(target.first, target.second);
	for (int i = 1; i <= 10; i++)
	{
		if ((feasible >> i) & 1)
		{
			board.set(target, i);
			tryCount++;
			if (fill(board, tryCount))
				return true;
		}
	}
	board.set(target, 0);
	return false;
}
bool SudokuSolver::search(SudokuBoard& board, int& count)
{
	std::pair<int, int> &target = board.findFewest();
	if (target.first == -1) // end
	{
		count++;
		return count >= 2;
	}
	if (target.second == -1) // no solution
		return false;
	int feasible = board.getFeasible(target.first, target.second);
	for (int i = 1; i <= 10; i++)
	{
		if ((feasible >> i) & 1)
		{
			board.set(target, i);
			if (search(board, count))
				return true;
		}
	}
	board.set(target, 0);
	return false;
}
bool SudokuSolver::isU(SudokuBoard board)
{
	int c = 0;
	return !search(board, c);
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
void SudokuSolver::generate(SudokuBoard &board, int initNum)
{
	SudokuBoard *r = new SudokuBoard(board);
	int x, y;
	for (int i = 0; i < initNum; i++)
	{
		x = rand() % 9;
		y = rand() % 9;
		if ((*r).loc(x, y) == 0)
		{
			auto solveVector = r->getSolveVector(x, y);
			(*r)[x * 9 + y] = solveVector[rand() % solveVector.size()];
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
	generate(board, 0);
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
		bit |= 1 << loc(i, y);
	for (int j = 0; j < 9; j++)
		bit |= 1 << loc(x, j);
	int start_x = x / 3 * 3;
	int start_y = y / 3 * 3;
	for (int i = start_x; i < start_x + 3; i++)
		for (int j = start_y; j < start_y + 3; j++)
			bit |= 1 << loc(i, j);
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
			if (loc(i, j) == 0)
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
int SudokuBoard::findF()
{
	int min = 100;
	int temp;
	int minN;
	int mem;
	for (int i = 0; i < 81; i++)
		if (_board[i] != 0)
		{
			mem = _board[i];
			_board[i] = 0;
			temp = countFeasible(i);
			if (temp < min)
			{
				min = temp;
				minN = i;
			}
			_board[i] = mem;
		}
	return minN;
}
int SudokuBoard::findMost()
{
	int max = 0;
	int temp;
	int maxN;
	int mem;
	for (int i = 0; i < 81; i++)
		if (_board[i] != 0)
		{
			mem = _board[i];
			_board[i] = 0;
			temp = countFeasible(i);
			if (temp > max)
			{
				max = temp;
				maxN = i;
			}
			_board[i] = mem;
		}
	return maxN;
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
int SudokuBoard::getRandFeasible(int x, int y)
{
	int feasible = getFeasible(x, y);
	int count = 0;
	int s[9];
	for (int i = 1; i <= 9; i++)
		if ((feasible >> i) & 1)
			s[count++] = i;
	return s[rand() % count];
}
std::shared_ptr<std::string> SudokuBoard::toString()
{
	auto rtn = std::shared_ptr<std::string>(new std::string());
	for (int i = 0; i < 81; i++)
	{
		(*rtn).push_back(_board[i] + '0');
		(*rtn).push_back(((i + 1) % 9) == 0 ? '\n' : ' ');
	}
	return rtn;
}

void SudokuSolver::generateFinal(int initNum, int result[81])
{
	SudokuBoard r;
	int x, y;
	while (true)
	{
		for (int i = 0; i < initNum; i++)
		{
			while (true)
			{
				x = rand() % 9;
				y = rand() % 9;
				if (r.loc(x, y) == 0)
				{
					r.set(x, y, r.getRandFeasible(x, y));
					break;
				}
			}
		}
		if (fill(r, x))
		{
			r.copyTo(result);
			return;
		}
		else
			r.clear();
	}
}
void SudokuSolver::generateFinal(int initNum, SudokuBoard& result)
{
	SudokuBoard r;
	int x, y;
	while (true)
	{
		for (int i = 0; i < initNum; i++)
		{
			while (true)
			{
				x = rand() % 9;
				y = rand() % 9;
				if (r.loc(x, y) == 0)
				{
					r.set(x, y, r.getRandFeasible(x, y));
					break;
				}
			}
		}
		if (fill(r, x))
		{
			r.copyTo(result);
			return;
		}
		else
			r.clear();
	}
}
double SudokuSolver::evalDifficulty(SudokuBoard board)
{
	int blank = 0;
	int feasibleSolveCount = 0;
	int tryCount = 0;
	for (int i = 0; i < 81; i++)
	{
		if (board[i] == 0)
		{
			blank++;
			feasibleSolveCount += board.countFeasible(i);
		}
	}
	fill(board, tryCount);
	//printf("blank %d  FSC %d  rate %f  tryC %d\n", blank, feasibleSolveCount, (double)feasibleSolveCount / blank, tryCount);
	return tryCount;
}
void SudokuSolver::makeBlank(SudokuBoard& board, int num)
{
	int x;
	int i;
	for (i = 0; i < num && i < 2; i++)
	{
		while (true)
		{
			x = rand() % 81;
			if (board[x] != 0)
			{
				board[x] = 0;
				break;
			}
		}
	}
	//for (; i < num*0.5; i++)
	//{
	//	x = board.findMost();
	//	board[x] = 0;
	//}
	for (; i < num; i++)
		board[board.findF()] = 0;
}
void SudokuSolver::generate(int blank, int result[81])
{
	SudokuBoard b;
	generateFinal(11, b);
	makeBlank(b, blank);
	b.copyTo(result);
}
void SudokuSolver::generate(int number, int mode, int result[][81])
{
	int r;
	int tryCountL, tryCountR;
	srand(time(NULL));
	switch (mode)
	{
	case 0:
		r = 45;
		tryCountL = 0;
		tryCountR = 200000;
		break;
	case 2:
		r = 60;
		tryCountL = 200;
		tryCountR = 200000;
		break;
	case 3:
		generateHard(number, result);
		return;
	case 1:
	default:
		r = 45;
		tryCountL = 10;
		tryCountR = 200;
		break;
	}
	SudokuBoard b;
	int difficulty;
	std::unordered_set<std::string> set;
	for (int i = 0; i < number; i++)
	{
		while (true)
		{
			SudokuSolver::generateFinal(11, b);
			SudokuSolver::makeBlank(b, r);
			difficulty = SudokuSolver::evalDifficulty(b);
			if (difficulty >= tryCountL && difficulty < tryCountR)
				break;
			b.clear();
		}
		auto s = b.toLineString();
		auto got = set.find(s);
		if (got == set.end())
			set.insert(s);
		else
			i--;
		b.copyTo(result[i]);
	}
}
void SudokuSolver::generateU(int blank, int result[81])
{
	SudokuBoard b;
	int mem;
	int count;
	const int seq[9] = { 0,3,6,1,4,7,2,5,8 };
	do {
		count = 0;
		generateFinal(11, b);
		for (int j = 0; j < 9; j++)
			for (int i = seq[j] * 9; i < seq[j] * 9 + 9 && count < blank; i++)
			{
				mem = b[i];
				b[i] = 0;
				if (isU(b))
				{
					count++;
					continue;
				}
				b[i] = mem;
			}
	} while (count < blank);
	b.copyTo(result);
}

void SudokuSolver::generate(int number, int lower, int upper, bool unique, int result[][81])
{
	srand(time(NULL));
	int blank = lower + (((upper - lower) != 0) ? rand() % (upper - lower) : 0);
	std::unordered_set<std::string> set;
	if (unique)
	{
		for (int i = 0; i < number; i++)
		{
			SudokuSolver::generateU(blank, result[i]);
			auto s = SudokuBoard::toLineString(result[i]);
			auto got = set.find(s);
			if (got == set.end())
				set.insert(s);
			else
				i--;
		}
	}
	else
	{
		for (int i = 0; i < number; i++)
		{
			SudokuSolver::generate(blank, result[i]);
			auto s = SudokuBoard::toLineString(result[i]);
			auto got = set.find(s);
			if (got == set.end())
				set.insert(s);
			else
				i--;
		}
	}
}
bool SudokuSolver::solve(int puzzle[], int solution[])
{
	SudokuBoard b(puzzle);
	int x = INT_MIN;
	if (fill(b, x))
	{
		b.copyTo(solution);
		return true;
	}
	else
		return false;
}
void SudokuSolver::generateHard(int number, int result[][81])
{
	std::vector<std::shared_ptr<SudokuBoard>> puzzles;
	readFileLine("sudoku17.txt", puzzles);
	srand(time(NULL));
	int start = rand() % (puzzles.size() - number - 1);
	int diff[81];
	for (int i = 0; i < number; i++)
	{
		auto p = *puzzles[i];
		p.copyTo(result[i]);
	}
}
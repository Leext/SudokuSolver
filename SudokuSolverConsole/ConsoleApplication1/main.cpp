#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <Windows.h>
#include <string>
typedef void(*GenerateM)(int number, int mode, int result[][81]);
typedef void(*GenerateR)(int number, int lower, int upper, bool unique, int result[][81]);
typedef bool(*Solve)(int puzzle[], int solution[]);
typedef bool(*Check)(int board[81]);
typedef int(*GetFeasible)(int board[81], int x, int y);
typedef char*(*GenerateN)(int n);
GenerateM generateM;
GenerateR generateR;
Solve solve;
Check check;
GetFeasible getFeasible;
GenerateN generateN;
int parseArg();
int result[10000][81];

int main(int argc, char **argv)
{
	HMODULE hdll = LoadLibrary(L"Core.dll");
	if (!hdll)
		return 0;
	generateM = (GenerateM)GetProcAddress(hdll, "generateM");
	generateR = (GenerateR)GetProcAddress(hdll, "generateR");
	solve = (Solve)GetProcAddress(hdll, "solve");
	check = (Check)GetProcAddress(hdll, "check");
	generateN = (GenerateN)GetProcAddress(hdll, "generateN");

	bool cFlag, sFlag, nFlag, mFlag, rFlag, uFlag;
	cFlag = sFlag = nFlag = mFlag = rFlag = uFlag = false;
	int mode, lower, upper, number;
	char *fileName = NULL;

	try {
		for (int p = 1; p < argc; p++)
		{
			if (strcmp(argv[p], "-c") == 0)
			{
				cFlag = true;
				if (++p >= argc)
					throw std::invalid_argument("not enough arg");
				size_t end;
				try
				{
					number = std::stoi(std::string(argv[p]), &end);
				}
				catch (...)
				{
					std::string e("invalid number ");
					e += argv[p];
					throw std::invalid_argument(e);;
				}
				if (number < 1 || number > 1000000 || strlen(argv[p]) != end)
				{
					std::string e("invalid number ");
					e += argv[p];
					throw std::invalid_argument(e);
				}
			}
			else if (strcmp(argv[p], "-s") == 0)
			{
				sFlag = true;
				if (++p >= argc)
					throw std::invalid_argument("not enough arg");
				FILE *f = fopen(argv[p], "r");
				if (f == NULL)
				{
					std::string e("invalid file path ");
					e += argv[p];
					throw std::invalid_argument(e);
				}
				fileName = argv[p];
			}
			else if (strcmp(argv[p], "-n") == 0)
			{
				nFlag = true;
				if (++p >= argc)
					throw std::invalid_argument("not enough arg");
				size_t end;
				try {
					number = std::stoi(std::string(argv[p]), &end);
				}
				catch (...)
				{
					std::string e("invalid number ");
					e += argv[p];
					throw std::invalid_argument(e);;
				}
				if (number < 1 || number > 10000 || strlen(argv[p]) != end)
				{
					std::string e("invalid number ");
					e += argv[p];
					throw std::invalid_argument(e);
				}
			}
			else if (strcmp(argv[p], "-m") == 0)
			{
				mFlag = true;
				if (++p >= argc)
					throw std::invalid_argument("not enough arg");
				size_t end;
				try {
					mode = std::stoi(std::string(argv[p]), &end);
				}
				catch (...)
				{
					std::string e("invalid number ");
					e += argv[p];
					throw std::invalid_argument(e);;
				}
				if (mode < 1 || mode > 3 || strlen(argv[p]) != end)
				{
					std::string e("invalid mode ");
					e += argv[p];
					throw std::invalid_argument(e);
				}
			}
			else if (strcmp(argv[p], "-r") == 0)
			{
				rFlag = true;
				if (++p >= argc)
					throw std::invalid_argument("not enough arg");
				bool findSym = false;
				int i;
				for (i = 0; argv[p][i] != '\0'; i++)
					if (argv[p][i] == '~')
					{
						findSym = true;
						break;
					}
				if (findSym == false)
				{
					std::string e("invalid range ");
					e += argv[p];
					throw std::invalid_argument(e);
				}
				argv[p][i] = '\0';
				size_t end1, end2;
				try
				{ 
				lower = std::stoi(std::string(argv[p]), &end1);
				upper = std::stoi(std::string(&argv[p][i + 1]), &end2);
				}
				catch (...)
				{
					std::string e("invalid number ");
					argv[p][i] = '~';
					e += argv[p];
					throw std::invalid_argument(e);;
				}
				if (lower < 20 || lower >55 || strlen(argv[p]) != end1)
				{
					std::string e("invalid lower ");
					e += argv[p];
					throw std::invalid_argument(e);
				}
				if (upper < 20 || upper > 55 || strlen(&argv[p][i + 1]) != end2)
				{
					std::string e("invalid upper ");
					e += &argv[p][i + 1];
					throw std::invalid_argument(e);
				}
				if (upper < lower)
					throw std::invalid_argument("upper cannot be bigger than lower");
			}
			else if (strcmp(argv[p], "-u") == 0)
			{
				uFlag = true;
			}
			else
			{
				std::string e("invalid arg ");
				e += argv[p];
				throw std::invalid_argument(e);
			}
			//c is not unique
			bool e1 = cFlag && (sFlag || nFlag || mFlag || rFlag || uFlag);
			if (e1)
				throw std::invalid_argument("arg c must be the only arg");
			//s is not unique
			bool e2 = sFlag && (cFlag || nFlag || mFlag || rFlag || uFlag);
			if (e2)
				throw std::invalid_argument("arg s must be the only arg");
		}

		//wrong m arg
		bool e3 = mFlag && (!nFlag || rFlag || uFlag);
		if (e3)
			throw std::invalid_argument("arg m must be used with n and without r,u");
		//wrong r arg
		bool e4 = rFlag && !nFlag;
		if (e4)
			throw std::invalid_argument("arg m must be used with n");
		if (sFlag)
		{
			int puzzle[81], solution[81];
			FILE *input = fopen(fileName, "r");
			FILE *output = fopen("sudoku.txt", "w");
			while (true)
			{
				for (int i = 0; i < 81; i++)
					if (fscanf(input, "%d", &puzzle[i]) == EOF)
						goto out;
				solve(puzzle, solution);
				for (int i = 0; i < 81; i++)
					fprintf(output, "%d%c", solution[i], ((i + 1) % 9 == 0) ? '\n' : ' ');
				fprintf(output, "\n");
			}
		out:
			fclose(input);
			fclose(output);
		}
		else if (cFlag)
		{
			FILE *output = fopen("sudoku.txt", "w");
			fprintf(output, generateN(number));
			fclose(output);
		}
		else if (mFlag)
		{
			generateM(number, mode, result);
			FILE *output = fopen("sudoku.txt", "w");
			for (int i = 0; i < number; i++)
			{
				for (int j = 0; j < 81; j++)
					fprintf(output, "%d%c", result[i][j], ((j + 1) % 9 == 0) ? '\n' : ' ');
				fprintf(output, "\n");
			}
			fclose(output);
		}
		else if (rFlag)
		{
			generateR(number, lower, upper, uFlag, result);
			FILE *output = fopen("sudoku.txt", "w");
			for (int i = 0; i < number; i++)
			{
				for (int j = 0; j < 81; j++)
					fprintf(output, "%d%c", result[i][j], ((j + 1) % 9 == 0) ? '\n' : ' ');
				fprintf(output, "\n");
			}
			fclose(output);
		}
		else if (nFlag)
		{
			generateM(number, 0, result);
			FILE *output = fopen("sudoku.txt", "w");
			for (int i = 0; i < number; i++)
			{
				for (int j = 0; j < 81; j++)
					fprintf(output, "%d%c", result[i][j], ((j + 1) % 9 == 0) ? '\n' : ' ');
				fprintf(output, "\n");
			}
			fclose(output);
		}
		else
		{
			printf("invalid args");
		}
	}
	catch (std::exception e)
	{
		printf("%s\n", e.what());
	}

	FreeLibrary(hdll);
	return 0;
}


int parseArg()
{
	return 0;
}
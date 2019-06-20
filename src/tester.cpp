#include "tester.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>

Tester::Tester()
{
	for(int i = 1; i <= TEST_NUMBER; i++)
		importTestCase(i);
}

void Tester::importTestCase(int testCase)
{
	std::ifstream inFile("input" + std::to_string(testCase) + ".in");
	std::string line;

	if(!inFile)
	{
		std::cout << "Testcase #" << testCase << " file not exists... Plz check your ./testfiles folder" << std::endl;
		exit(1);
	}

	try 
	{
		inFile >> isValid[testCase];
		for(int row = 1; row <= 4; row++)
		{
			inFile >> line;
			for(int col = 1; col <= 4; col++)
				testCases[testCase][row][col] = line[col - 1];
		}
	}
	catch(...)
	{
		std:: cout << "Testcase #" << testCase << " has wrong format... Plz check your file in ./testfiles folder" << std::endl;
		exit(1);
	}
}

void Tester::testAll()
{
	int correctTestCases = 0, result;
	std::string message;

	for(int t = 1; t <= TEST_NUMBER; t++)
	{
		std::cout << "Testcase #" << t << ": ";
		result = test(t);

		switch(result)
		{
			case 0:
				correctTestCases++;
				message = "Correct!";
				break;
			case 1:
				message = "Wrong Answer...";
				break;
			case 2:
				message = "Invalid Movements...";
				break;
			default:
				message = "System Error... Plz contact to the developer";
		}

		std::cout << message << std::endl;
	}

	std::cout << "Total Result: " << correctTestCases << " / " << TEST_NUMBER << std::endl;
	if(correctTestCases == TEST_NUMBER)
	{
		std::cout << "\nCongratulations! You've passed all the tests! Have a good day :)" << std::endl;
	}
}

/*
 * return values 
 * true : simulation has successfully finished.
 * false : invalid movement
 */
bool Tester::simulation(char board[][5], std::vector<pr>& process)
{
	int startx, starty, endx, endy;
	int diffx, diffy;
	int row, col, dirx, diry;
	char horse;

	for(pr action : process)
	{
		startx = action.first / 10, starty = action.first % 10;
		endx = action.second / 10, endy = action.second % 10;
		diffx = abs(startx - endx);
		diffy = abs(starty - starty);

		if(startx == endx && starty == endy)
			return false;
		if(startx < 1 || starty < 1 || endx < 1 || endy < 1)
			return false;
		if(startx > 4 || starty > 4 || endx > 4 || endy > 4)
			return false;

		if(startx < endx) dirx = 1;
		else if(startx > endx) dirx = -1;
		else dirx = 0;

		if(starty < endy) diry = 1;
		else if(starty > endy) diry = -1;
		else diry = 0;

		horse = board[startx][starty];
		if(board[endx][endy] == '.')
			return false;

		if(horse == '.')
			return false;
		else if(horse == 'E')
		{
			if(diffx > 1 || diffy > 1)
				return false;
		}
		else if(horse == 'Q')
		{
			if(diffx * diffy != 0 && diffx != diffy)
				return false;

			for(int row = startx + dirx; row != endx; row += dirx)
				for(int col = starty + diry; col != endy; col += diry)
					if(board[row][col] != '.')
						return false;
		}
		else if(horse == 'K')
		{
			if(diffx * diffy != 2)
				return false;
		}
		else if(horse == 'B')
		{
			if(diffx != diffy)
				return false;

			for(int row = startx + dirx; row != endx; row += dirx)
				for(int col = starty + diry; col != endy; col += diry)
					if(board[row][col] != '.')
						return false;
		}
		else if(horse == 'R')
		{
			if(diffx * diffy != 0)
				return false;

			for(int row = startx + dirx; row != endx; row += dirx)
				for(int col = starty + diry; col != endy; col += diry)
					if(board[row][col] != '.')
						return false;
		}
		else if(horse == 'P')
		{
			if(diffx * diffy != 1 || startx < endx)
				return false;
		}
		else
			return false;

		board[endx][endy] = horse;
		board[startx][starty] = '.';
	}

	return true;
}

bool Tester::isCorrect(char board[][5])
{
	int cnt = 0;
	for(int row = 1; row <= 4; row++)
		for(int col = 1; col <= 4; col++)
			if(board[row][col] != '.')
				cnt++;

	return (cnt == 1 ? true : false);
}

/*
 * return values 
 * 0 : correct answer
 * 1 : correct movements, but wrong answer
 * 2 : invalid movements
 */
int Tester::test(int testCase)
{
	char board[5][5];
	bool result;
	std::vector<pr> ans;

	for(int row = 1; row <= 4; row++)
		memcpy(board[row] + 1, testCases[testCase][row] + 1, 4 * sizeof(char));
	solution(board, ans);

	for(int row = 1; row <= 4; row++)
		memcpy(board[row] + 1, testCases[testCase][row] + 1, 4 * sizeof(char));
	result = simulation(board, ans);

	if(!result)
		return 2;

	if(!isValid[testCase] && ans.size() == 0)
		return 0;
	else if(isValid[testCase] && isCorrect(board))
		return 0;

	return 1;
}
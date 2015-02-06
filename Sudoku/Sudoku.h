//
//  Sudoku.h
//  Sudoku Project
//
//  Created by Julien on 5/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku_Project__Sudoku__
#define __Sudoku_Project__Sudoku__

#include <stdio.h>
#include <string>
#include <vector>
#include "StructsAndEnums.h"


#define BASE 4

#define SIZE BASE*BASE
// Sudoku.readFile(), Sudoku.displaySol() and Sudoku.displayCand()
// all assumes BASE=3 and SIZE=9 !!! Needs modification if BASE>3...

using namespace std;




class Sudoku{
	
	int solution[SIZE][SIZE];
	// solution[row][col] gives the value of the box [row][col]
	// if already determined, and -1 otherwise
	// for compatibility with array indexing, sudoku numbers go from 0 to 8 (not 1 to 9)
	
	int nUnknown = SIZE*SIZE;
	// number of undetermined boxes left
	
	bool candidate[SIZE][SIZE][SIZE];
	// if solution[row][col] == -1 (is undetermined yet) then
	// candidate[row][col][k] gives wether k is still a possible candidate
	
	Coord block[BASE][BASE][SIZE];
	// block[baseRow][baseCol] gives the list of boxes in the block [baseRow][baseCol]
	
	bool rowVal[SIZE][SIZE] = {};
	bool colVal[SIZE][SIZE] = {};
	bool blockVal[BASE][BASE][SIZE] = {};
	// rowCand[row][k] gives wether the row 'row' already has the value 'k' determined
	// similar definition for colCand and blockCand
	
	
	
public:
	
	// Constructor from a file
	Sudoku(string filePath);
	
	// Constructor from list of hints
	Sudoku(vector<Guess> hints);
	
	// Random Constructor
	Sudoku();
	
	
	void initialize();
	
	
	// read the input test file and updates accordingly
	void readFile(string filePath);
	
	
	void applyHints(vector<Guess> hints);
	
	
	// update 'solution' and 'candidate', 'rowVal, ... accordingly
	void update(int row, int col, int value);
	
	
	// display the solution
	const void displaySol();
	
	
	// check if a box has a unique candidate left
	// and if so update accordingly
	// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
	checkResult checkBoxObvious(int row, int col);
	
	
	// check if a row has a unique box (or column) left with 'value' as possible candidate
	// and if so update accordingly
	// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
	checkResult checkRowObvious(int row, int value);
	
	
	// check if a row has a unique box (or column) left with 'value' as possible candidate
	// and if so update accordingly
	// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
	checkResult checkColObvious(int col, int value);
	
	
	// check if a block has a unique box left with 'value' as possible candidate
	// and if so update accordingly
	// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
	checkResult checkBlockObvious(int baseRow, int baseCol, int value);
	
	
	// check all obvious situations and update accordingly
	// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
	checkResult checkObvious();
	
	
	// find the first undetermined box
	// return true if there is one, false otherwise (meaning the grid is solved)
	const bool getFirstUnknown(Coord &c);
	
	// find a random undetermined box
	// return true if there is one, false otherwise (meaning the grid is solved)
	const bool getRandomUnknown(Coord &c);
	
	
	// find the first candidate of box [row][col]
	const int getFirstCand(int row, int col);
	
	// find the first candidate of box [row][col]
	const int getRandomCand(int row, int col);
	
	
	// remove candidate 'value' from box [row][col]
	void removeCand(int row, int col, int value);
	
	
	// Solve the problem and applies the solution (optional)
	// return TRUE if successful, FALSE otherwise
	bool solve(bool applySol=true);
	
	
	////////////////////////////////////////
	// DEBUG FUNCTIONS:
	
	
	// Count the number of possible solutions
	// Returns either NONE (0), UNIQUE (1) or SEVERAL (2)
	NumSol findAllSol();
	
	
	// check if the final solution is valid
	const bool finalCheck();
	
	
	// display the candidates
	const void displayCand();
	
	
};

#endif /* defined(__Sudoku_Project__Sudoku__) */



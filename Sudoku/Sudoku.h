//
//  Sudoku.h
//  Sudoku Project
//
//  Created by Julien on 5/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku_Project__Sudoku__
#define __Sudoku_Project__Sudoku__

#include "StructsAndEnums.h"
#include "Partitions.h"

//#include <stdio.h>
#include <string>
#include <vector>


//#define BASE 3
// Avoid using #define as much as possible. "const int" works just as well, and tells the compiler
// that BASE is actually an int (and not just an arbitrary  string of letters), which may lead to 
// more readable error messages and error detection.
const int BASE = 4;

//#define SIZE BASE*BASE
const int SIZE = BASE*BASE;


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
    
    static std::vector<int*>** partitions;
	
	// Constructor from a file
//	Sudoku(string filePath);
    
    // Change 1: For clarity (and sometimes to help the compiler optimize) make function parameters 
    //           that you are not changing in your function 'const'
    // Change 2: When passing a const function parameter that may be large, it's usually faster
    //           to pass a reference to it rather than making a copy. For std::strings, this may
    //           be unnecessary with modern c++ compilers, but it doesn't hurt.
	Sudoku(const string& filePath);

	
	// Constructor from list of hints
    // Change: passing as a const reference instead of sending a copy. If vector<Guess> is large, 
    //         this is certainly going to be more efficient.
	Sudoku(const vector<Guess>& hints);
	
	// Random Constructor
	Sudoku();
	
	
	void initialize();
	
	
	// read the input test file and updates accordingly
	void readFile(const string& filePath);
	
	
	void applyHints(const vector<Guess>& hints);
	
	
	// update 'solution' and 'candidate', 'rowVal, ... accordingly
	void update(int row, int col, int value);
	
	
	// display the solution
//	const void displaySol();
    // Your declaration says that the function returns a "void" which is "const", which isn't useful atall.
    // Now, with "const" at the end of the declaration, it says that displaySol() is a const member function, 
    // and will not modify the Sudoku object.
	void displaySol() const;
	
	
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
	//const bool getFirstUnknown(Coord &c);
    // Same error as above, your const refer to the bool instead of the member function.
    bool getFirstUnknown(Coord &c) const;

	// find a random undetermined box
	// return true if there is one, false otherwise (meaning the grid is solved)
	bool getRandomUnknown(Coord &c) const;
	
	
	// find the first candidate of box [row][col]
	int getFirstCand(int row, int col) const;
	
	// find the first candidate of box [row][col]
	int getRandomCand(int row, int col) const;
	
	
	// remove candidate 'value' from box [row][col]
	void removeCand(int row, int col, int value);
	
	
	// Solve the problem and applies the solution (optional)
	// return TRUE if successful, FALSE otherwise
	bool solve(bool applySol=true);
    
    void checkAllPartRow();
    void checkAllPartRow(int row);
    void checkPartRow(const int* part, int partSize, int row, const vector<int>& unknownCols);
    
    void checkAllPartCol();
    void checkAllPartCol(int col);
    void checkPartCol(const int* part, int partSize, int col, const vector<int>& unknownRows);
    
    void checkAllPartBlock();
    void checkAllPartBlock(int baseRow, int baseCol);
    void checkPartBlock(const int* part, int partSize, int baseRow, int baseCol, const vector<int>& unknownBoxes);
	
	////////////////////////////////////////
	// DEBUG FUNCTIONS:
	
	
	// Count the number of possible solutions
	// Returns either NONE (0), UNIQUE (1) or SEVERAL (2)
	NumSol findAllSol() const;
	
	
	// check if the final solution is valid
	bool finalCheck() const;
	
	
	// display the candidates
	void displayCand() const;
	
	
};

#endif /* defined(__Sudoku_Project__Sudoku__) */



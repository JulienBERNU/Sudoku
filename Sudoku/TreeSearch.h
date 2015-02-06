//
//  TreeSearch.h
//  Sudoku Project
//
//  Created by Julien on 5/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku_Project__TreeSearch__
#define __Sudoku_Project__TreeSearch__

#include <stdio.h>
#include <stack>
#include <vector>
#include "Sudoku.h"
#include "StructsAndEnums.h"


class TreeSearch{
	
//	stack<Sudoku*> SudokuStack;		// contains the series of the temporary grid states as guesses are made
//	stack<Coord> coordStack;		// contains the series of Coord's where the guesses are made
//	stack<int> valueStack;			// contains the series of values for the guesses
	
	stack<Guess> allGuesses;	// contains the guess history
	stack<Sudoku*> allStatesBeforeGuesses;
	
public:
	
	void newGuess(Coord c, int value, Sudoku* current);
	
	bool backtrack(Sudoku* &current);
	
	void clear();
	
	bool findSol(Sudoku* problem, bool applySol);
	
	NumSol findAllSol(Sudoku* problem);
	
	vector<Guess> generateHints(Sudoku* problem);
	
};

#endif /* defined(__Sudoku_Project__TreeSearch__) */




//
//  TreeSearch.h
//  Sudoku Project
//
//  Created by Julien on 5/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku_Project__TreeSearch__
#define __Sudoku_Project__TreeSearch__


#include "Sudoku.h"
#include "StructsAndEnums.h"

#include <stack>
#include <vector>


class TreeSearch{
	
	std::stack<Guess> allGuesses;	// contains the guess history
	std::stack<Sudoku*> allStatesBeforeGuesses;


public:
	
	void newGuess(Coord c, int value, Sudoku* current);
	
	bool backtrack(Sudoku* &current);
	
	void clear();
	
	bool findSol(Sudoku* problem, bool applySol);
	
	NumSol findAllSol(const Sudoku* problem);
	
	vector<Guess> generateHints(Sudoku* problem);
	
};

#endif /* defined(__Sudoku_Project__TreeSearch__) */




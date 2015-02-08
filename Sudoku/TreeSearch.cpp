//
//  TreeSearch.cpp
//  Sudoku Project
//
//  Created by Julien on 5/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

// Always include the header file coresponding to the current .cpp file first.
// Reason: This assures that all definitions (from other .h files) needed by 
// your TreeSearch.h file are being explicitly included by TreeSearch.h.
#include "TreeSearch.h"
#include "StructsAndEnums.h"

#include <iostream>
#include <vector>

using namespace std;


// Add a new guess to the history
void TreeSearch::newGuess(Coord c, int value, Sudoku* current){
	Sudoku* copy = new Sudoku(*current);
	Guess guess = {c,value};
	allGuesses.push(guess);
	allStatesBeforeGuesses.push(copy);
}


// Go one step back in the history (*current is updated accordingly)
bool TreeSearch::backtrack(Sudoku* &current){
	if (allGuesses.empty())
		// no bactracking possible
		return false;
	// backtrack: we go back to before the last guess
	delete current;
	Guess lastGuess = allGuesses.top();
	allGuesses.pop();
	Sudoku* lastState = allStatesBeforeGuesses.top();
	allStatesBeforeGuesses.pop();
	current = lastState;
	// and we update that candidate
	current->removeCand(lastGuess.coord.row, lastGuess.coord.col, lastGuess.value);
	return true;
}


// Clear the memory
void TreeSearch::clear(){
	while (!allStatesBeforeGuesses.empty()) {
		delete allStatesBeforeGuesses.top();
		allStatesBeforeGuesses.pop();
	}
}


// Find a solution and apply it (optionnal)
// Return TRUE if successful, FALSE otherwise
bool TreeSearch::findSol(Sudoku* problem, bool applySol){
	
	bool finished = false;
	Sudoku* current = new Sudoku(*problem);
	
	bool foundSol = false;
	
	while (!finished) {
		checkResult check = current->checkObvious(); // do all obvious checks until no more progress or error...
		switch (check) {
			case FOUND_NOTHING:
				// no more obvious move
				Coord c;
				if (current->getFirstUnknown(c)) {						// Search systematically: first unknown box
					// there's still some undetermined box >> make a new guess
					int value = current->getFirstCand(c.row,c.col);		// Search systematically: first candidate
					newGuess(c,value,current);
					current->update(c.row,c.col,value);
				}
				else {
					// there aren't any undetermined boxes >> we've found the (assuming unique) solution!
					if (applySol)
						*problem = *current;
					foundSol = true;
					finished = true;
				}
				break;
			case FOUND_ERROR:
				// reached a dead-end
				if (!backtrack(current)) {
					// was not able to backtrack
					finished = true;
				}
				break;
		}
		
	}
	
	// clean up memory
	delete current;
	clear();
	
	return foundSol;

}


// Generate a list a 'hints' defining a random problem with unique solution
vector<Guess> TreeSearch::generateHints(Sudoku* problem){
	
	// Start by finding a random solution (normally from an empty problem)
	// This is very similar to findSol(problem, false)
	// Possible to avoid duplicate code?
	Sudoku* current = new Sudoku(*problem);
	bool finished = false;
	while (!finished) {
		checkResult check = current->checkObvious(); // do all obvious checks until no more progress or error...
		switch (check){
			case FOUND_NOTHING:
				// no more obvious move
				Coord c;
				if (current->getRandomUnknown(c)) {						// Seach randomly to generate random problems
					// there's still some undetermined box >> make a new guess
					int value = current->getRandomCand(c.row,c.col);	// Seach randomly to generate random problems
					newGuess(c,value,current);
					current->update(c.row,c.col,value);
				}
				else {
					// there aren't any undetermined boxes >> we've found a solution!
					finished = true;
				}
				break;
			case FOUND_ERROR:
				// reached a dead-end
				backtrack(current);
				break;
		}
	}
	
	// bactrack to check wether all guesses wouldn't actually have been forced
	vector<Guess> hints;	// contains the hints already identified as necessary
	while (!allGuesses.empty()) {
		Guess lastGuess = allGuesses.top();
		backtrack(current);
		// apply all hints already identified
		current->applyHints(hints);
		if (current->solve(false)){
			// another solution was found after backtracking
			// >> last guess is a necessary hint to reach our solution
			hints.push_back(lastGuess);
		}
	}
	
	// clean up memory
	delete current;
	clear(); // should not be necessary
	
	return hints;
	
	
}



//////////////////////////
// For check and debug...


// Count the number of possible solutions
// Returns either NONE (0), UNIQUE (1) or SEVERAL (2)
NumSol TreeSearch::findAllSol(Sudoku* problem){
	
	NumSol nSol = NONE;
	bool finished = false;
	Sudoku* current = new Sudoku(*problem);
	
	while (!finished) {
		checkResult check = current->checkObvious(); // do all obvious checks until no more progress or error...
		switch (check) {
			case FOUND_NOTHING:
				// no more obvious move
				Coord c;
				if (current->getFirstUnknown(c)) {						// Search systematically: first unknown box
					// there's still some undetermined box >> make a new guess
					int value = current->getFirstCand(c.row,c.col);		// Search systematically: first candidate
					newGuess(c,value,current);
					current->update(c.row,c.col,value);
				}
				else {
					// there aren't any undetermined boxes >> we've found a solution!
					switch (nSol) {
						case NONE:
							nSol = UNIQUE;
							if (!backtrack(current)) {
								// was not able to backtrack
								finished = true;
							}
							break;
						case UNIQUE:
							nSol = SEVERAL;
							finished = true;
					}
				}
				break;
			case FOUND_ERROR:
				// reached a dead-end
				if (!backtrack(current)) {
					// was not able to backtrack
					finished = true;
				}
				break;
		}
		
	}
	
	// clean up memory
	delete current;
	clear();
	
	return nSol;
	
}








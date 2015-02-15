//
//  Sudoku.cpp
//  Sudoku Project
//
//  Created by Julien on 5/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#include "Sudoku.h"
#include "TreeSearch.h"
#include "Partitions.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <stack>
#include <cassert>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <vector>

using namespace std;

const Partitions Sudoku::P = Partitions(SIZE);

// Constructor from a file
Sudoku::Sudoku(const string & filePath){
	initialize();
	readFile(filePath);
}

// Constructor form list of hints
Sudoku::Sudoku(const vector<Guess>& hints){
	initialize();
	applyHints(hints);
}

// Random Constructor
Sudoku::Sudoku(){
	initialize();
	TreeSearch Search;
	vector<Guess> hints = Search.generateHints(this);
	applyHints(hints);
}


void Sudoku::initialize(){
	for (int row=0; row<SIZE; row++) {
		for (int col=0; col<SIZE; col++) {
			solution[row][col] = -1;
			for (int cand=0; cand<SIZE; cand++) {
				candidate[row][col][cand] = true;
			}
		}
	}
	for (int baseRow=0; baseRow<BASE; baseRow++) {
		for (int baseCol=0; baseCol<BASE; baseCol++) {
			for (int i=0; i<SIZE; i++){
				block[baseRow][baseCol][i].row = baseRow*BASE+i/BASE;
				block[baseRow][baseCol][i].col = baseCol*BASE+i%BASE;
			}
		}
	}
}


// read the input test file and updates accordingly
// ONLY VALID FOR BASE<4
void Sudoku::readFile(const string& filePath){
	
	// Open test problem file
	ifstream file(filePath);
	if (!file) {
		cerr << "Input file could not be opened!" << endl;
		exit(1);
	}
	
	// Read file , fill 'solution' and update 'candidate' accordingly
	char c;
	for (int row=0; row<SIZE; row++) {
		for (int col=0; col<SIZE; col++) {
			file.get(c);
			if (c>='1' && c<='9')
				// a 'hint' value is given
				update(row, col, int(c-'1'));
			else {
				if (c!=' ') {
					cerr << "Bad input file!" << endl;
					cerr << "Invalid character at position (" << row+1 << "," << col+1 << ")" << endl;
					cerr << "Should be either a digit 1-9 or a white space." << endl;
				}
			}
		}
		file.get(c);
		if (c != '\n') {
			cerr << "Bad input file!" << endl;
			cerr << "Too many characters at line " << row << endl;
		}
	}
	
}


// updates the problem with all given hints
void Sudoku::applyHints(const vector<Guess>& hints){
    for (const Guess & guess : hints)
        update(guess.coord.row, guess.coord.col, guess.value);
}


// update 'solution' and 'candidate', 'rowVal, ... accordingly
void Sudoku::update(int row, int col, int value){
	
	if (solution[row][col] == -1) {
		
		// 'value' is no longer a possible candidate for the other boxes
		// in the same row, column or block as [row][col]
		Coord* b = block[row/BASE][col/BASE];
		for (int i=0; i<SIZE; i++) {
			candidate[row][i][value] = false;						// same row
			candidate[i][col][value] = false;						// same column
			candidate[b[i].row][b[i].col][value] = false;	// same block
		}
		// the other candidates for this box are also no longer possible
		// Updating this should not be necessary, but nicer for display...
		for (int i=0; i<SIZE; i++) {
			candidate[row][col][i] = (i==value);
		}
		
		// update the solution itself
		solution[row][col] = value;
		rowVal[row][value] = true;
		colVal[col][value] = true;
		blockVal[row/BASE][col/BASE][value] = true;
		
		nUnknown--;
		
	}
	
}



// display the solution
void Sudoku::displaySol() const {
	for (int row=0; row<SIZE; row++) {
		if (!(row%BASE)) {
            for (int baseCol=0; baseCol<BASE; baseCol++) {
                cout << '+';
                for (int blockCol=0; blockCol<BASE; blockCol++) {
                    cout << '-';
                }
            }
            cout << '+' << endl;
		}
		for (int col=0; col<SIZE; col++) {
			if (!(col%BASE)) {
				cout << '|';
			}
			if (solution[row][col] == -1)
				cout << '.';
			else
                if (solution[row][col]<10) {
                    cout << solution[row][col];
                }
                else {
                    cout << char(solution[row][col]-9+'A');
                }
		}
		cout << '|' << endl;
	}
    for (int baseCol=0; baseCol<BASE; baseCol++) {
        cout << '+';
        for (int blockCol=0; blockCol<BASE; blockCol++) {
            cout << '-';
        }
    }
    cout << '+' << endl;
}


// check if a box has a unique candidate left
// and if so update accordingly
// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
checkResult Sudoku::checkBoxObvious(int row, int col){
	int nCand = 0;
	int value = 0;
	int uniqueValue = 0;
	while (nCand<2 && value<SIZE) {		// can stop the search if more than 1 have already been found
		if (candidate[row][col][value]) {
			nCand++;
			uniqueValue = value;		// will be used only if actually unique
		}
		value++;
	}
	switch (nCand) {
		case 0:
			return FOUND_ERROR;
		case 1:
			update(row,col,uniqueValue);
			return FOUND_SOMETHING;
		default:
			return FOUND_NOTHING;
	}
}


// check if a row has a unique box (or column) left with 'value' as possible candidate
// and if so update accordingly
// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
checkResult Sudoku::checkRowObvious(int row, int value){
	int nCol = 0;
	int col = 0;
	int uniqueCol = 0;
	while (nCol<2 && col<SIZE) {	// can stop the search if more than 1 have already been found
		if (candidate[row][col][value]) {
			nCol++;
			uniqueCol = col;		// will be used only if actually unique
		}
		col++;
	}
	switch (nCol) {
		case 0:
			return FOUND_ERROR;
		case 1:
			update(row,uniqueCol,value);
			return FOUND_SOMETHING;
		default:
			return FOUND_NOTHING;
	}
}


// check if a row has a unique box (or column) left with 'value' as possible candidate
// and if so update accordingly
// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
checkResult Sudoku::checkColObvious(int col, int value) {
	int nRow = 0;
	int row = 0;
	int uniqueRow = 0;
	while (nRow<2 && row<SIZE) {	// can stop the search if more than 1 have already been found
		if (candidate[row][col][value]) {
			nRow++;
			uniqueRow = row;		// will be used only if actually unique
		}
		row++;
	}
	switch (nRow) {
		case 0:
			return FOUND_ERROR;
		case 1:
			update(uniqueRow,col,value);
			return FOUND_SOMETHING;
		default:
			return FOUND_NOTHING;
	}
}


// check if a block has a unique box left with 'value' as possible candidate
// and if so update accordingly
// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
checkResult Sudoku::checkBlockObvious(int baseRow, int baseCol, int value) {
	int nBox = 0;
	int i = 0;
	int unique_i = 0;
	Coord* box = block[baseRow][baseCol];
	while (nBox<2 && i<SIZE) {	// can stop the search if more than 1 have already been found
		if (candidate[box[i].row][box[i].col][value]) {
			nBox++;
			unique_i = i;		// will be used only if actually unique
		}
		i++;
	}
	switch (nBox) {
		case 0:
			return FOUND_ERROR;
		case 1:
			update(box[unique_i].row,box[unique_i].col,value);
			return FOUND_SOMETHING;
		default:
			return FOUND_NOTHING;
	}
}


// check all obvious situations and update accordingly
// Returns either FOUND_NOTHING (0), FOUND_SOMETHING (1) or FOUND_ERROR (2)
checkResult Sudoku::checkObvious(){
	
	checkResult result = FOUND_NOTHING;
	
	// check all boxes
	for (int row=0; row<SIZE; row++) {
		for (int col=0; col<SIZE; col++) {
			if (solution[row][col] == -1) {
				switch (checkBoxObvious(row,col)) {
					case FOUND_ERROR:
						return FOUND_ERROR;
					case FOUND_SOMETHING:
						result = FOUND_SOMETHING;
				}
			}
		}
	}
	
	// check all rows
	for (int row=0; row<SIZE; row++) {
		for (int value=0; value<SIZE; value++){
			if (!rowVal[row][value]) {
				switch (checkRowObvious(row,value)) {
					case FOUND_ERROR:
						return FOUND_ERROR;
					case FOUND_SOMETHING:
						result = FOUND_SOMETHING;
				}
			}
		}
	}
	
	// check all columns
	for (int col=0; col<SIZE; col++) {
		for (int value=0; value<SIZE; value++){
			if (!colVal[col][value]) {
				switch (checkColObvious(col,value)) {
					case FOUND_ERROR:
						return FOUND_ERROR;
					case FOUND_SOMETHING:
						result = FOUND_SOMETHING;
				}
			}
		}
	}
	
	// check all blocks
	for (int baseRow=0; baseRow<BASE; baseRow++) {
		for (int baseCol=0; baseCol<BASE; baseCol++) {
			for (int value=0; value<SIZE; value++){
				if (!blockVal[baseRow][baseCol][value]) {
					switch (checkBlockObvious(baseRow,baseCol,value)) {
						case FOUND_ERROR:
							return FOUND_ERROR;
						case FOUND_SOMETHING:
							result = FOUND_SOMETHING;
					}
				}
			}
		}
	}
	
	return result;
	
}


// find the first (or random) undetermined box
// return true if there is one, false otherwise (meaning the grid is solved)
bool Sudoku::getFirstUnknown(Coord &c) const{
	for (int row=0; row<SIZE; row++) {
		for (int col=0; col<SIZE; col++) {
			if (solution[row][col] == -1) {
				c.row = row;
				c.col = col;
				return true;
			}
		}
	}
	return false;
	
}

// find a random undetermined box
// return true if there is one, false otherwise (meaning the grid is solved)
bool Sudoku::getRandomUnknown(Coord &c) const{
	if (nUnknown==0)
		return false;
	int n = rand()%nUnknown + 1;
	for (int row=0; row<SIZE; row++) {
		for (int col=0; col<SIZE; col++) {
			if (solution[row][col] == -1) {
				n--;
				if (n==0) {
					c.row = row;
					c.col = col;
					return true;
				}
			}
		}
	}
	cerr << "Should not print that!" << endl;
	return false;
	
}


// find the first candidate of box [row][col]
int Sudoku::getFirstCand(int row, int col) const{
	int cand=0;
	while (!candidate[row][col][cand]) cand++;
	return cand;
}

// find a random candidate from box [row][col]
int Sudoku::getRandomCand(int row, int col) const{
	
	int nCand = 0;
	// count the number of candidates left
	for (int value=0; value<SIZE; value++) {
		if (candidate[row][col][value])
			nCand++;
	}
	// chose a random candidate
	nCand = rand()%nCand;
	nCand++;
	
	int cand=0;
	while (nCand!=0) {
		while (!candidate[row][col][cand]) cand++;
		cand++;
		nCand--;
	}
	cand--;
	return cand;
}


// remove candidate 'value' from box [row][col]
void Sudoku::removeCand(int row, int col, int value) {
	candidate[row][col][value] = false;
}


// Solve the problem and applies the solution (optional)
// return TRUE if successful, FALSE otherwise
bool Sudoku::solve(bool applySol){
	TreeSearch Search;
	return Search.findSol(this, applySol);
}


void Sudoku::checkAllPartRow(int row) {
    
    // first find unknowns
    vector<int> unknownCols;
    for (int col=0; col<SIZE; col++) {
        if (solution[row][col] == -1)
            unknownCols.push_back(col);
    }
    int nUnknownCols = unknownCols.size();
    
    
}

void Sudoku::checkPartRow(const int* part, int partSize, int row, const vector<int>& unknownCols){
    int nPartCand = 0;
    bool partCand[SIZE] = {};
    for (int i=0; i<partSize || nPartCand>partSize; i++) {
        for (int value=0; value<SIZE || nPartCand>partSize; value++) {
            if (candidate[row][unknownCols[i]][value] && !partCand[value]) {
                partCand[value] = true;
                nPartCand++;
            }
        }
    }
    if (nPartCand<=partSize) {
        cout << "found partition at row " << row << endl;
        displayCand();
        exit(23);
    }
    
}


////////////////////////////////////////
// DEBUG FUNCTIONS:


// Count the number of possible solutions
// Returns either NONE (0), UNIQUE (1) or SEVERAL (2)
NumSol Sudoku::findAllSol() const{
	TreeSearch Search;
	return Search.findAllSol(this);
}



// check if the final solution is valid
bool Sudoku::finalCheck() const{
	// check rows
	for (int row=0; row<SIZE; row++) {
		for (int value=0; value<SIZE; value++) {
			int n = 0;
			for (int col=0; col<SIZE; col++) {
				if (solution[row][col]==value) n++;
			}
			if (n!=1) return false;
		}
	}
	// check columns
	for (int col=0; col<SIZE; col++) {
		for (int value=0; value<SIZE; value++) {
			int n = 0;
			for (int row=0; row<SIZE; row++) {
				if (solution[row][col]==value) n++;
			}
			if (n!=1) return false;
		}
	}
	// check blocks
	for (int baseRow=0; baseRow<BASE; baseRow++) {
		for (int baseCol=0; baseCol<BASE; baseCol++) {
			for (int value=0; value<SIZE; value++) {
				int n = 0;
				const Coord* box = block[baseRow][baseCol];
				for (int i=0; i<SIZE; i++) {
					if (solution[box[i].row][box[i].col]==value) n++;
				}
				if (n!=1) return false;
			}
		}
	}
	return true;
}



// display the candidates
void Sudoku::displayCand() const{
    for (int i=0; i<1+BASE*(2+BASE*(BASE+1)); i++) {
        cout << '#';
    }
    cout << endl;
	for (int row=0; row<SIZE; row++) {
        if (!(row%BASE)) {
            if (row!=0) {
                for (int baseCol=0; baseCol<BASE; baseCol++) {
                    cout << "#+";
                    for (int blockCol=0; blockCol<BASE; blockCol++) {
                        for (int candCol=0; candCol<BASE; candCol++) {
                            cout << '-';
                        }
                        cout << '+';
                    }
                }
                cout << '#' << endl;
                for (int i=0; i<1+BASE*(2+BASE*(BASE+1)); i++) {
                    cout << '#';
                }
                cout << endl;
			}
        }
        for (int baseCol=0; baseCol<BASE; baseCol++) {
            cout << "#+";
            for (int blockCol=0; blockCol<BASE; blockCol++) {
                for (int candCol=0; candCol<BASE; candCol++) {
                    cout << '-';
                }
                cout << '+';
            }
        }
        cout << '#' << endl;
        for (int base=0; base<BASE; base++) {
            for (int col=0; col<SIZE; col++) {
                if (!(col%BASE)) {
                    if (col!=0) cout << "|";
                    cout << "#";
                }
                cout << '|';
                for (int cand=base*BASE; cand<(base+1)*BASE; cand++) {
                    if (candidate[row][col][cand]) {
                        if (cand<10) {
                            cout << cand;
                        }
                        else {
                            cout << char(cand-9+'A');
                        }
                    }
                    else
                        cout << ' ';
                }
            }
            cout << "|#" << endl;
        }
    }
    for (int baseCol=0; baseCol<BASE; baseCol++) {
        cout << "#+";
        for (int blockCol=0; blockCol<BASE; blockCol++) {
            for (int candCol=0; candCol<BASE; candCol++) {
                cout << '-';
            }
            cout << '+';
        }
    }
    cout << '#' << endl;
    for (int i=0; i<1+BASE*(2+BASE*(BASE+1)); i++) {
        cout << '#';
    }
    cout << endl;
}







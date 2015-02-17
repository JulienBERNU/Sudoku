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


std::vector<int*>** initPartitions(){
    std::vector<int*>** partitions = new std::vector<int*>*[SIZE+1];
    for (int range=0; range<=SIZE; range++) {
        partitions[range] = new std::vector<int*>[SIZE+1];
        Partitions::fillAllParts(range, partitions[range]);
    }
    return partitions;
}

std::vector<int*>** Sudoku::partitions = initPartitions();

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
            // same row
            if (i!=col && candidate[row][i][value]) {
                candidate[row][i][value] = false;
                needCheckBox[row][i] = true;
                needCheckCol[i][value] = true;
                needCheckBlock[row/BASE][i/BASE][value] = true;
                needCheckPartCol[i] = true;
                needCheckPartBlock[row/BASE][i/BASE] = true;
            }
            // same column
            if (i!=row && candidate[i][col][value]) {
                candidate[i][col][value] = false;
                needCheckBox[i][col] = true;
                needCheckRow[i][value] = true;
                needCheckBlock[i/BASE][col/BASE][value] = true;
                needCheckPartRow[i] = true;
                needCheckPartBlock[i/BASE][col/BASE] = true;
            }
            // same block
            if ((b[i].row!=row || b[i].col!=col) && candidate[b[i].row][b[i].col][value]) {
                candidate[b[i].row][b[i].col][value] = false;
                needCheckBox[b[i].row][b[i].col] = true;
                needCheckRow[b[i].row][value] = true;
                needCheckCol[b[i].col][value] = true;
                needCheckPartRow[b[i].row] = true;
                needCheckPartCol[b[i].col] = true;
            }
            
            // the other candidates for this box are also no longer possible
            // Updating this should not be necessary, but nicer for display...
            if (i!=value && candidate[row][col][i]) {
                candidate[row][col][i] = false;
                needCheckRow[row][i] = true;
                needCheckCol[col][i] = true;
                needCheckBlock[row/BASE][col/BASE][value] = true;
                needCheckPartRow[row] = true;
                needCheckPartCol[col] = true;
                needCheckPartBlock[row/BASE][col/BASE] = true;
            }
        }
        
        // update the solution itself
        solution[row][col] = value;
        
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
	
	// boxes
	for (int row=0; row<SIZE; row++) {
		for (int col=0; col<SIZE; col++) {
            if (needCheckBox[row][col]) {
                needCheckBox[row][col] = false;
                switch (checkBoxObvious(row,col)) {
                    case FOUND_ERROR:
                        return FOUND_ERROR;
                    case FOUND_SOMETHING:
                        result = FOUND_SOMETHING;
                    default:;
                }
            }
		}
	}
	
	// rows
	for (int row=0; row<SIZE; row++) {
		for (int value=0; value<SIZE; value++){
            if (needCheckRow[row][value]) {
                needCheckRow[row][value] = false;
                switch (checkRowObvious(row,value)) {
                    case FOUND_ERROR:
                        return FOUND_ERROR;
                    case FOUND_SOMETHING:
                        result = FOUND_SOMETHING;
                    default:;
                }
            }
		}
	}
	
	// columns
	for (int col=0; col<SIZE; col++) {
		for (int value=0; value<SIZE; value++){
            if (needCheckCol[col][value]) {
                needCheckCol[col][value] = false;
                switch (checkColObvious(col,value)) {
                    case FOUND_ERROR:
                        return FOUND_ERROR;
                    case FOUND_SOMETHING:
                        result = FOUND_SOMETHING;
                    default:;
                }
            }
		}
	}
	
	// check all blocks
	for (int baseRow=0; baseRow<BASE; baseRow++) {
		for (int baseCol=0; baseCol<BASE; baseCol++) {
			for (int value=0; value<SIZE; value++){
                if (needCheckBlock[baseRow][baseCol][value]) {
                    needCheckBlock[baseRow][baseCol][value] = false;
                    switch (checkBlockObvious(baseRow,baseCol,value)) {
                        case FOUND_ERROR:
                            return FOUND_ERROR;
                        case FOUND_SOMETHING:
                            result = FOUND_SOMETHING;
                        default:;
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
    assert(false);  // should never reach that
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
    needCheckBox[row][col] = true;
    needCheckRow[row][value] = true;
    needCheckCol[col][value] = true;
    needCheckBlock[row/BASE][col/BASE][value] = true;
    needCheckPartRow[row] = true;
    needCheckPartCol[col] = true;
    needCheckPartBlock[row/BASE][col/BASE] = true;
}


// Solve the problem and applies the solution (optional)
// return TRUE if successful, FALSE otherwise
bool Sudoku::solve(bool applySol){
	TreeSearch Search;
	return Search.findSol(this, applySol);
}


// check all partitions
// return true if something new was found
bool Sudoku::checkAllPart() {
    bool result = false;
    result |= checkAllPartRow();
    result |= checkAllPartCol();
    result |= checkAllPartBlock();
    return result;
}

// check all partitions in rows
// return true if something new was found
bool Sudoku::checkAllPartRow() {
    bool result = false;
    for (int row=0; row<SIZE; row++) {
        if (needCheckPartRow[row]) {
            needCheckPartRow[row] = false;
            result |= checkAllPartRow(row);
        }
    }
    return result;
}

// check all partitions in a given row
// return true if something new was found
bool Sudoku::checkAllPartRow(int row) {
    bool result = false;
    // first find unknowns
    vector<int> unknownCols;
    for (int col=0; col<SIZE; col++) {
        if (solution[row][col] == -1)
            unknownCols.push_back(col);
    }
    // check all non trivial partitions
    int nUnknownCols = int(unknownCols.size());
    for (int partSize=2; partSize<nUnknownCols; partSize++) {
        for (int i=0; i<partitions[nUnknownCols][partSize].size(); i++) {
            result |= checkPartRow(partitions[nUnknownCols][partSize][i], partSize, row, unknownCols);
        }
    }
    return result;
}

// check a given partition in a given row
// return true if something new was found
bool Sudoku::checkPartRow(const int* part, int partSize, int row, const vector<int>& unknownCols){
    // count the number of candidates in part
    int nPartCand = 0;
    bool partCand[SIZE] = {};
    // partCand[value] tells wether value is a candidate in part
    for (int i=0; i<partSize && nPartCand<=partSize; i++) {
        for (int value=0; value<SIZE && nPartCand<=partSize; value++) {
            // as soon as nPartCand > partSize we can stop since the following
            // is condition upon nPartCand <= partSize
            if (candidate[row][unknownCols[part[i]]][value] && !partCand[value]) {
                partCand[value] = true;
                nPartCand++;
            }
        }
    }
    bool result = false;
    if (nPartCand<=partSize) {
        // we found a conclusive partition
        // its candidates can be removed from its complementary partition
        int complement[unknownCols.size()-partSize];
        Partitions::getComplementPart(part, partSize, int(unknownCols.size()), complement);
        for (int i=0; i<unknownCols.size()-partSize; i++) {
            for (int value=0; value<SIZE; value++) {
                if (partCand[value] && candidate[row][unknownCols[complement[i]]][value]) {
                    result = true;
                    removeCand(row, unknownCols[complement[i]], value);
                }
            }
        }
    }
    return result;
}


// similar process as rows
// see comments for rows
bool Sudoku::checkAllPartCol() {
    bool result = false;
    for (int col=0; col<SIZE; col++) {
        if (needCheckPartCol[col]) {
            needCheckPartCol[col] = false;
            result |= checkAllPartCol(col);
        }
    }
    return false;
}

// similar process as rows
// see comments for rows
bool Sudoku::checkAllPartCol(int col) {
    bool result = false;
    // first find unknowns
    vector<int> unknownRows;
    for (int row=0; row<SIZE; row++) {
        if (solution[row][col] == -1)
            unknownRows.push_back(row);
    }
    int nUnknownRows = int(unknownRows.size());
    for (int partSize=2; partSize<nUnknownRows; partSize++) {
        for (int i=0; i<partitions[nUnknownRows][partSize].size(); i++) {
            result |= checkPartCol(partitions[nUnknownRows][partSize][i], partSize, col, unknownRows);
        }
    }
    return result;
}

// similar process as rows
// see comments for rows
bool Sudoku::checkPartCol(const int* part, int partSize, int col, const vector<int>& unknownRows){
    int nPartCand = 0;
    bool partCand[SIZE] = {};
    for (int i=0; i<partSize && nPartCand<=partSize; i++) {
        for (int value=0; value<SIZE && nPartCand<=partSize; value++) {
            if (candidate[unknownRows[part[i]]][col][value] && !partCand[value]) {
                partCand[value] = true;
                nPartCand++;
            }
        }
    }
    bool result = false;
    if (nPartCand<=partSize) {
        int complement[unknownRows.size()-partSize];
        Partitions::getComplementPart(part, partSize, int(unknownRows.size()), complement);
        for (int i=0; i<unknownRows.size()-partSize; i++) {
            for (int value=0; value<SIZE; value++) {
                if (partCand[value] && candidate[unknownRows[complement[i]]][col][value]) {
                    removeCand(unknownRows[complement[i]], col, value);
                    result = true;
                }
            }
        }
    }
    return result;
}

// similar process as rows
// see comments for rows
bool Sudoku::checkAllPartBlock() {
    bool result = false;
    for (int baseRow=0; baseRow<BASE; baseRow++) {
        for (int baseCol=0; baseCol<BASE; baseCol++) {
            if (needCheckPartBlock[baseRow][baseCol]) {
                needCheckPartBlock[baseRow][baseCol] = false;
                result |= checkAllPartBlock(baseRow,baseCol);
            }
        }
    }
    return result;
}

// similar process as rows
// see comments for rows
bool Sudoku::checkAllPartBlock(int baseRow, int baseCol) {
    bool result = false;
    // first find unknowns
    vector<int> unknownBoxes;
    Coord* box = block[baseRow][baseCol];
    for (int i=0; i<SIZE; i++) {
        if (solution[box[i].row][box[i].col] == -1)
            unknownBoxes.push_back(i);
    }
    int nUnknownBoxes = int(unknownBoxes.size());
    for (int partSize=2; partSize<nUnknownBoxes; partSize++) {
        for (int i=0; i<partitions[nUnknownBoxes][partSize].size(); i++) {
            result |= checkPartBlock(partitions[nUnknownBoxes][partSize][i], partSize, baseRow, baseCol, unknownBoxes);
        }
    }
    return result;
}

// similar process as rows
// see comments for rows
bool Sudoku::checkPartBlock(const int* part, int partSize, int baseRow, int baseCol, const vector<int>& unknownBoxes){
    int nPartCand = 0;
    bool partCand[SIZE] = {};
    Coord* box = block[baseRow][baseCol];
    for (int i=0; i<partSize && nPartCand<=partSize; i++) {
        for (int value=0; value<SIZE && nPartCand<=partSize; value++) {
            if (candidate[box[unknownBoxes[part[i]]].row][box[unknownBoxes[part[i]]].col][value] && !partCand[value]) {
                partCand[value] = true;
                nPartCand++;
            }
        }
    }
    bool result = false;
    if (nPartCand<=partSize) {
        int complement[unknownBoxes.size()-partSize];
        Partitions::getComplementPart(part, partSize, int(unknownBoxes.size()), complement);
        for (int i=0; i<unknownBoxes.size()-partSize; i++) {
            for (int value=0; value<SIZE; value++) {
                if (partCand[value] && candidate[box[unknownBoxes[complement[i]]].row][box[unknownBoxes[complement[i]]].col][value]) {
                    removeCand(box[unknownBoxes[complement[i]]].row, box[unknownBoxes[complement[i]]].col, value);
                    result = true;
                }
            }
        }
    }
    return result;
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







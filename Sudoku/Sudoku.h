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

#include <string>
#include <vector>


const int BASE = 3;
const int SIZE = BASE*BASE;
// Sudoku.readFile()
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
    
    bool needCheckBox[SIZE][SIZE] = {};
    bool needCheckRow[SIZE][SIZE] = {};
    bool needCheckCol[SIZE][SIZE] = {};
    bool needCheckBlock[BASE][BASE][SIZE] = {};
    bool needCheckPartRow[SIZE];
    bool needCheckPartCol[SIZE];
    bool needCheckPartBlock[BASE][BASE];
    // contains information for solving
    // for example if needCheckBox[row][col] = false,
    // the solving routine will skip checking box [row][col]
    // (because we identified this check as useless)
    
    
    
public:
    

    static std::vector<int*>** partitions;
    // A big array/vector containing all possible partitions.
    // partitions[range][partSize] is a vector conatining
    // all partitions of size partSize in the range {0...(range-1)}
    // (each partition is an int[])
    // for eaxample partition[4][2] = (vector) :
    // {{0,1}, {0,2}, {0,3}, {1,2}, {1,3}, {2,3}}
    
    // Constructor from a file
    Sudoku(const string& filePath);
    
    
    // Constructor from list of hints
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
    
    // check all partitions
    bool checkAllPart();
    // in a row (or column, or block) if we can find amongst the undetermined boxes
    // a partition (i.e. a subset) which collectively contains no more that sizePart candidate
    // then we know those candidates must be in that partition, and thus can be removed from
    // the complementary partition
    
    // check partitions in rows
    bool checkAllPartRow();         // for all rows
    bool checkAllPartRow(int row);  // for a given row
    bool checkPartRow(const int* part, int partSize, int row, const vector<int>& unknownCols);  // for a given partition
    
    // in columns
    bool checkAllPartCol();
    bool checkAllPartCol(int col);
    bool checkPartCol(const int* part, int partSize, int col, const vector<int>& unknownRows);
    
    // in blocks
    bool checkAllPartBlock();
    bool checkAllPartBlock(int baseRow, int baseCol);
    bool checkPartBlock(const int* part, int partSize, int baseRow, int baseCol, const vector<int>& unknownBoxes);
    
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



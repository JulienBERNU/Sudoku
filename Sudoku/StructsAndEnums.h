//
//  StructsAndEnums.h
//  Sudoku
//
//  Created by Julien on 6/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku__StructsAndEnums__
#define __Sudoku__StructsAndEnums__

#include <stdio.h>

enum checkResult {
	FOUND_NOTHING,
	FOUND_SOMETHING,
	FOUND_ERROR
};

enum NumSol {
	NONE,
	UNIQUE,
	SEVERAL
};

struct Coord{
	int row;
	int col;
};

struct Guess{
	Coord coord;
	int value;
};


#endif /* defined(__Sudoku__StructsAndEnums__) */

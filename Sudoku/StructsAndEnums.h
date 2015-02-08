//
//  StructsAndEnums.h
//  Sudoku
//
//  Created by Julien on 6/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku__StructsAndEnums__
#define __Sudoku__StructsAndEnums__

// To reduce compile times, try hard to avoid including header files that are not needed.
// Not important in your case, but makes a big difference in large projects.
//#include <stdio.h>

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

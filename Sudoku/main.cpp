//
//  main.cpp
//  Sudoku
//
//  Created by Julien on 29/01/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#include "Sudoku.h"
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

// Using namespace std is ok in example code and tiny projects, but should be avoided in larger programs.
// Reason 1: Makes it clear that "stack" is actually "std::stack" and not something else.
// Reason 2: Avoids conflict between, for example, a local variable "int stack" and std::stack.
using namespace std;




int main(){
	
	clock_t start, iStart;
	double duration;
	start = clock();
	
	
	////////////////////////////
	
//	string filePath = "/Users/Julien/Documents/Programming/Sudoku Project/hardest3.txt";
//	Sudoku problem(filePath);

	
	srand(unsigned(time(0)));
	
	for (int i=0; i<1;i++) {
		
		iStart = clock();
		Sudoku problem;	// generates a random problem
		//		problem.displaySol();
		duration = ( clock() - iStart ) / (double) CLOCKS_PER_SEC;
		cout << i << " generation: " << duration << '\n';
		assert(duration<1);
		
		iStart = clock();
		problem.solve();
		assert(problem.finalCheck());
		//		problem.displaySol();
		assert(problem.findAllSol()==UNIQUE);
		duration = ( clock() - iStart ) / (double) CLOCKS_PER_SEC;
		cout << i << " solving: " << duration << '\n';
		assert(duration<1);
		
		cout << endl;
	}
	
	
	
	////////////////////////////
	
	
	duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
	cout << "total duration: " << duration << '\n';
    
//    int ensemble[] = {1,42,3,24};
//    void (*actionToPerform)(const int*, int) = Partitions::printPart;
//    Partitions::scanAllParts(4, actionToPerform);
//    actionToPerform(part,4);
    
    Sudoku::P.printAllParts();
    
	return 0;
	
}

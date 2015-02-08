//
//  main.cpp
//  Sudoku
//
//  Created by Julien on 29/01/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//


#include <fstream>
#include <iostream>
#include <ctime>
#include <stack>
#include <cassert>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "Sudoku.h"

// Using namespace std is ok in example code and tiny projects, but should be avoided in larger programs.
// Reason 1: Makes it clear that "stack" is actually "std::stack" and not something else.
// Reason 2: Avoids conflict between, for example, a local variable "int stack" and std::stack.
using namespace std;



int main(){
	
	clock_t start;
	double duration;
	start = clock();
	
	
	////////////////////////////
	
//	string filePath = "/Users/Julien/Documents/Programming/Sudoku Project/hardest3.txt";
//	Sudoku problem(filePath);

	
	srand(unsigned(time(0)));
	
	Sudoku problem;	// generates a random problem
    duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    problem.displaySol();
    cout << "generation duration: " << duration << '\n';
    
	problem.solve();
	problem.displaySol();
	
    
	////////////////////////////
	
	
	duration = ( clock() - start ) / (double) CLOCKS_PER_SEC - duration ;
	cout << "solving duration: " << duration << '\n';
	
	return 0;
	
}

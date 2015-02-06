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

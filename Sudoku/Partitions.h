//
//  Partitions.h
//  Sudoku
//
//  Created by Julien Bernu on 14/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#ifndef __Sudoku__Partitions__
#define __Sudoku__Partitions__

#include <iostream>
#include <vector>


namespace Partitions {
    
//    int size;
//    std::vector<int*>* partitions;
    
    void fill(int* part, int partSize, std::vector<int*>* partitions);
    
    void completeAndFillPart(int* part, int currentSize, int partSize, int range, std::vector<int*>* partitions);
    
    void fillAllSizedParts(int partSize, int range, std::vector<int*>* partitions);
    
    void fillAllParts(int range, std::vector<int*>* partitions);
    
    void getComplementPart(const int* part, int partSize, int range, int* complement);
    
    
    
    void printPart(const int* part, int partSize);
    
    void printSizedParts(int partSize, std::vector<int*>& sizedPartitions);
    
    void printRangedParts(int range, std::vector<int*>* partitions);
    
    void printAllParts(int SIZE, std::vector<int*>** partitions);
    
    
//    Partitions(int size);
    
    
};


#endif /* defined(__Sudoku__Partitions__) */

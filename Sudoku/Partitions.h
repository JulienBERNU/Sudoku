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


class Partitions {
    
    int size;
    std::vector<int*>* partitions;
    
    void fill(int* part, int partSize);
    
    void completeAndFillPart(int* part, int currentSize, int partSize);
    
    void fillAllSizedParts(int partSize);
    
    void fillAllParts();
    
    void printAllSizedParts(int partSize) const;
    
    
    
public:
    
    
    void printAllParts() const;
    
    void printPart(const int* part, int partSize) const;
    
    Partitions(int maxSize);
    
    
};


#endif /* defined(__Sudoku__Partitions__) */

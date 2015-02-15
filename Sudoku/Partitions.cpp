//
//  Partitions.cpp
//  Sudoku
//
//  Created by Julien Bernu on 14/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#include "Partitions.h"

#include <iostream>


void Partitions::printPart(const int* part, int partSize) const {
    for (int i=0; i<partSize; i++) {
        std::cout << part[i] << ' ';
    }
    std::cout << '\n';
//    for (int i=0; i<part.size(); i++)
//        std::cout << part[i] << ' ';
//    std::cout << '\n';
}

void Partitions::printAllParts() const {
    for (int partSize=0; partSize<=size; partSize++) {
        std::cout << "Partitions of size " << partSize <<":\n";
        printAllSizedParts(partSize);
    }
}

void Partitions::printAllSizedParts(int partSize) const {
    for (int i=0; i<partitions[partSize].size(); i++){
        printPart(partitions[partSize][i], partSize);
    }
//    for (int* & part : partitions[partSize]) {
//        printPart(part, partSize);
//    }
}

void Partitions::fill(int* part, int partSize) {
    int* copyPart = new int[partSize];
    for (int i=0; i<partSize; i++) {
        copyPart[i] = part[i];
    }
//    std::cout << "tutu\n";
        partitions[partSize].push_back(copyPart);
    
//    std::cout << "toto\n";

}


void Partitions::completeAndFillPart(int* part, int currentSize, int partSize){
    int stillToGo = partSize-currentSize;
    if (stillToGo==0) {
        fill(part, partSize);
        return;
    }
    int start = 0;
    if (currentSize>0)
        start = part[currentSize-1]+1;
    for (int i=start; i<size-stillToGo+1; i++) {
        part[currentSize] = i;
        completeAndFillPart(part,currentSize+1,partSize);
    }
}

void Partitions::fillAllSizedParts(int partSize){
    int* part = new int[partSize];
    completeAndFillPart(part, 0, partSize);
    delete part;
}


void Partitions::fillAllParts() {
    for (int partSize=0; partSize<=size; partSize++) {
//        std::cout << "Filling partitions of size " << partSize <<":\n";
        fillAllSizedParts(partSize);
    }
}

Partitions::Partitions(int maxSize) {
    
    size = maxSize;
    partitions = new std::vector<int*>[maxSize+1];
    
//    std::vector<int*> parts;
//    std::vector<std::vector<int*>> partitions(maxSize, parts);
    fillAllParts();
    
}




//
//void Partitions::completePart(int size, int* part, int currentSize, int partSize, void (*actionToPerform)(const int* part, int size)){
//    int stillToGo = partSize-currentSize;
//    if (stillToGo==0) {
//        actionToPerform(part, partSize);
//        return;
//    }
//    int start = 0;
//    if (currentSize>0)
//        start = part[currentSize-1]+1;
//    for (int i=start; i<size-stillToGo+1; i++) {
//        part[currentSize] = i;
//        completePart(size,part,currentSize+1,partSize,actionToPerform);
//    }
//}
//
//void Partitions::scanAllSizedParts(int size, int partSize, void (*actionToPerform)(const int*, int)){
//    int* part = new int[partSize];
//    completePart(size, part, 0, partSize, actionToPerform);
//    delete part;
//}
//
//void Partitions::scanAllParts(int size, void (*actionToPerform)(const int*, int)) {
//    for (int partSize=0; partSize<=size; partSize++) {
//        std::cout << "Partitions of size " << partSize <<":\n";
//        scanAllSizedParts(size, partSize, actionToPerform);
//    }
//}










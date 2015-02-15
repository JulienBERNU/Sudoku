//
//  Partitions.cpp
//  Sudoku
//
//  Created by Julien Bernu on 14/02/2015.
//  Copyright (c) 2015 Julien. All rights reserved.
//

#include "Partitions.h"

#include <iostream>


void Partitions::printPart(const int* part, int partSize) {
    for (int i=0; i<partSize; i++) {
        std::cout << part[i] << ' ';
    }
    std::cout << '\n';
//    for (int i=0; i<part.size(); i++)
//        std::cout << part[i] << ' ';
//    std::cout << '\n';
}

void Partitions::printSizedParts(int partSize, std::vector<int*>& sizedPartitions) {
    for (int i=0; i<sizedPartitions.size(); i++){
//        std::cout << "part number " << i << std::endl;
        printPart(sizedPartitions[i], partSize);
    }
    //    for (int* & part : partitions[partSize]) {
    //        printPart(part, partSize);
    //    }
}

void Partitions::printRangedParts(int range, std::vector<int*>* rangedPartitions) {
    for (int partSize=0; partSize<=range; partSize++) {
        std::cout << "Partitions of size " << partSize <<":\n";
        printSizedParts(partSize, rangedPartitions[partSize]);
    }
}


void Partitions::printAllParts(int SIZE, std::vector<int*>** partitions) {
    for (int range = 0; range<SIZE; range++) {
        std::cout << "Partitions of range " << range <<":\n";
        printRangedParts(range, partitions[range]);
    }
}




void Partitions::fill(int* part, int partSize, std::vector<int*>* rangedPartitions) {
    int* copyPart = new int[partSize];
    for (int i=0; i<partSize; i++) {
        copyPart[i] = part[i];
    }
//    std::cout << "tutu\n";
        rangedPartitions[partSize].push_back(copyPart);
    
//    std::cout << "toto\n";

}


void Partitions::completeAndFillPart(int* part, int currentSize, int partSize, int range, std::vector<int*>* rangedPartitions){
    int stillToGo = partSize-currentSize;
    if (stillToGo==0) {
        fill(part, partSize, rangedPartitions);
        return;
    }
    int start = 0;
    if (currentSize>0)
        start = part[currentSize-1]+1;
    for (int i=start; i<range-stillToGo+1; i++) {
        part[currentSize] = i;
        completeAndFillPart(part, currentSize+1, partSize, range, rangedPartitions);
    }
}

void Partitions::fillAllSizedParts(int partSize, int range, std::vector<int*>* rangedPartitions){
    int* part = new int[partSize];
    completeAndFillPart(part, 0, partSize, range, rangedPartitions);
    delete part;
}


void Partitions::fillAllParts(int range, std::vector<int*>* rangedPartitions) {
    for (int partSize=0; partSize<=range; partSize++) {
//        std::cout << "Filling partitions of size " << partSize <<":\n";
        fillAllSizedParts(partSize, range, rangedPartitions);
    }
}

void Partitions::getComplementPart(const int* part, int partSize, int range, int* complement) {
    int i = 0;
    int k = 0;
    for (int j=0; j<range; j++) {
        if (j == part[i]) {
            i++;
            continue;
        }
        complement[k] = j;
        k++;
    }
}

//Partitions::Partitions(int size) {
//    
////    size = maxSize;
//    partitions = new std::vector<int*>[size+1];
//    
////    std::vector<int*> parts;
////    std::vector<std::vector<int*>> partitions(maxSize, parts);
//    fillAllParts(size);
//    
//}




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










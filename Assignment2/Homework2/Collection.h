//
//  Collection.h
//  MazeGeneration
//
//  Created by Computer Science on 1/13/16.
//  Copyright (c) 2016 Computer Science. All rights reserved.
//

#ifndef __MazeGeneration__Collection__
#define __MazeGeneration__Collection__

#include <stdio.h>
#include <iostream>
#include <vector>

struct Cell
{
	int row;
	int column;
};

struct Collection
{
	std::vector<Cell> cellsToOpen;
};

void AddCell(Collection& col, int row, int column);
void RemoveCell(Collection& col, int row, int column);
Cell GetRandomCell(const Collection& col);
bool CollectionIsEmpty(const Collection& col);


#endif /* defined(__MazeGeneration__Collection__) */

//
//  Collection.cpp
//  MazeGeneration
//
//  Created by Computer Science on 1/13/16.
//  Copyright (c) 2016 Computer Science. All rights reserved.
//

#include "Collection.h"

void AddCell(Collection& col, int row, int column)
{
	Cell c = { row, column };
	col.cellsToOpen.push_back(c);
}

void RemoveCell(Collection& col, int row, int column)
{
	for (int i = 0; i < col.cellsToOpen.size(); i++)
	{
		if (row == col.cellsToOpen[i].row && column == col.cellsToOpen[i].column)
		{
			col.cellsToOpen[i] = col.cellsToOpen.back();
			col.cellsToOpen.pop_back();
			return;
		}
	}
}

Cell GetRandomCell(const Collection& col)
{
	return col.cellsToOpen[rand() % col.cellsToOpen.size()];
}

bool CollectionIsEmpty(const Collection& col)
{
	return col.cellsToOpen.size() == 0;
}
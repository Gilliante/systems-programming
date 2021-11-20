//Gillian Ehman

#include <iostream>
#include <cstdint>
#include <time.h>
#include <stdlib.h>
#include "Collection.h"
#include <ctime>
#include <cstdlib>

//to toggle a bit, xor it with 1
//16 * 64 bits, 32x32, 32 is first row and next 32 is second row, one cell per index, each cell is one bit
//if given row and col, how to determing the corresponding bit within that corrspeonding integer, shift and and once you figure out row and collinked to 
//mod rand with 4, it goes from 0 to max int

//struct for a Maze
struct Maze
{
	uint64_t array[16];
	int rows;
	int cols;
};

//initializes the variables of the struct and populates all cells with value 0.
void InitializeMaze(Maze& m)
{
	m.rows = 32;
	m.cols = 32;
	for (int i = 0; i < m.rows; i++)
	{
		m.array[i] = 0;
	}
}

//Return the value of the bit corresponding to row and column.
//temp var that stores number for that index and shift 1 over column if even, if not column + 32 so your're lined up with bit you want
//if you and bit it will give value?
bool GetCellValue(const Maze& m, int row, int column)
{
	uint64_t mask = 0;
	if (row % 2 == 0)
	{
		mask = (uint64_t)((m.array[row / 2] >> column) & 1ul);
	}
	//row % 2 != 0
	else
	{
		mask = (uint64_t)((m.array[row / 2] >> (column + 32)) & 1ul);
	}
	return mask;
}

/*You will print the maze in ASCII characters based on the value of the cell/bit.
If the bit is 0 then you print " X" (a space and X)
If the bit is 1 then you print "  " (two spaces)
The extra space is to increase the width of the maze so it is closer to a square.*/
void PrintMaze(Maze maze)
{
	uint64_t indexCount = 0;
	uint64_t bitCount = 0;

	for (int i = 0; i < 1024; i++)
	{
		int cellValue = GetCellValue(maze, indexCount, bitCount);
		if (cellValue == 0)
		{
			std::cout << " X";
		}
		else
		{
			std::cout << "  ";
		}
		if (bitCount == 31)
		{
			bitCount = 0;
			indexCount++;
			std::cout << "\n";
		}
		else
		{
			bitCount++;
		}
	}
}

//for the given row and column, set the value of the bit to 1
//two steps away open it? The room next to an open cell is two steps away, is blocked means it's value is 0
void SetWalkable(Maze& m, int row, int column)
{
	if (row % 2 == 0)
	{
		uint64_t mask = (uint64_t)((m.array[row / 2] >> column) & 1ul);
		mask = (uint64_t)(mask | 1ul);
		uint64_t newMaze = m.array[row / 2] | mask;
		m.array[row / 2] = newMaze;
	}
	//row % 2 != 0
	else
	{
		uint64_t mask = (uint64_t)((m.array[row / 2] >> (column + 32)) & 1ul);
		mask = (uint64_t)(mask | 1ul);
		uint64_t newMaze = m.array[row / 2] | mask;
		m.array[row / 2] = newMaze;
	}
}

//helper method to calculate a random number to choose a random direction
int getRandomNumber()
{
	srand(time(0));
	int r = (rand() % 4);
	return r;
}

//Generate a random maze and return the maze
/*Initialize maze by setting all cells to walls (0)
declare a collection cellsToExpand to store cells explored
add center cell to cellsToExpand.
Open center cell (set it to 1)
while cellsToExpand is not empty do
	   currentCell <-- random cell from cellsToExpand   //note that the neighbors of this cell will be walls
	   randomDirection <-- a random direction (north, south, east, west)
	   if neighbor two steps away from currentCell in direction randomDirection is not out of bound and is blocked
					add neighboring cell (two steps away) to cellsToExpand
					open the selected neighboring cell and the wall between current cell and the selected neighbor.
		 end
		 if currentCell is on the boundary or all its neighbors (two cells away) are open
					remove currentCell from cellsToExpand
		 end
end
*/
//return a random number for the direction
Maze MakeMaze()
{
	//create maze and collection structs 
	Maze maze;
	Collection cellsToExpand;

	//set maze to all walls (values all = 0)
	InitializeMaze(maze);

	//create cell struct for the center cell in the maze and initialize its variables
	Cell centerCell;
	centerCell.row = 16;
	centerCell.column = 16;

	//add center cell
	AddCell(cellsToExpand, centerCell.row, centerCell.column);

	//set center cell to 1 to open it
	SetWalkable(maze, centerCell.row, centerCell.column);

	while (!CollectionIsEmpty(cellsToExpand))
	{
		Cell currentCell = GetRandomCell(cellsToExpand);
		int randomDirection = getRandomNumber();
		int nextCellToOpenRow = 0;
		int nextCellToOpenColumn = 0;

		//direction is north
		if (randomDirection == 0)
		{
			if (currentCell.row == 0)
			{
				nextCellToOpenRow = currentCell.row - 2;
				nextCellToOpenColumn = currentCell.column;
				if (nextCellToOpenRow >= 0 && GetCellValue(maze, nextCellToOpenRow, nextCellToOpenColumn) != 1)
				{
					AddCell(cellsToExpand, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, currentCell.row - 1, nextCellToOpenColumn);
				}
				else
				{
					int eastRow = currentCell.row;
					int eastColumn = currentCell.column + 2;
					int southRow = currentCell.row + 2;
					int southColumn = currentCell.column;
					int westRow = currentCell.row;
					int westColumn = currentCell.column - 2;

					if (GetCellValue(maze, eastRow, eastColumn) == 1 && GetCellValue(maze, southRow, southColumn) == 1 &&
						GetCellValue(maze, westRow, westColumn) == 1)
					{
						RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
					}
				}
			}
			RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
		}

		//direction is east
		if (randomDirection == 1)
		{
			if (currentCell.column == 32)
			{
				nextCellToOpenRow = currentCell.row;
				nextCellToOpenColumn = currentCell.column + 2;
				if (nextCellToOpenColumn <= 32 && GetCellValue(maze, nextCellToOpenRow, nextCellToOpenColumn) != 1)
				{
					AddCell(cellsToExpand, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, nextCellToOpenRow, currentCell.column + 1);
				}
				else
				{
					int northRow = currentCell.row - 2;
					int northColumn = currentCell.column;
					int southRow = currentCell.row + 2;
					int southColumn = currentCell.column;
					int westRow = currentCell.row;
					int westColumn = currentCell.column - 2;

					if (GetCellValue(maze, northRow, northColumn) == 1 && GetCellValue(maze, southRow, southColumn) == 1 &&
						GetCellValue(maze, westRow, westColumn) == 1)
					{
						RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
					}
				}
			}
			RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
		}

		//direction is south
		if (randomDirection == 2)
		{
			if (currentCell.row == 32)
			{
				nextCellToOpenRow = currentCell.row + 2;
				nextCellToOpenColumn = currentCell.column;
				if (nextCellToOpenRow <= 32 && GetCellValue(maze, nextCellToOpenRow, nextCellToOpenColumn) != 1)
				{
					AddCell(cellsToExpand, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, currentCell.row + 1, nextCellToOpenColumn);
				}
				else
				{
					int northRow = currentCell.row - 2;
					int northColumn = currentCell.column;
					int eastRow = currentCell.row;
					int eastColumn = currentCell.column + 2;
					int westRow = currentCell.row;
					int westColumn = currentCell.column - 2;

					if (GetCellValue(maze, northRow, northColumn) == 1 && GetCellValue(maze, eastRow, eastColumn) == 1 &&
						GetCellValue(maze, westRow, westColumn) == 1)
					{
						RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
					}
				}
			}
			RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
		}

		//direction is west
		if (randomDirection == 3)
		{
			if (currentCell.column == 0)
			{
				nextCellToOpenRow = currentCell.row;
				nextCellToOpenColumn = currentCell.column - 2;
				if (nextCellToOpenColumn >= 0 && GetCellValue(maze, nextCellToOpenRow, nextCellToOpenColumn) != 1)
				{
					AddCell(cellsToExpand, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, nextCellToOpenRow, nextCellToOpenColumn);
					SetWalkable(maze, nextCellToOpenRow, currentCell.column - 1);
				}
				else
				{
					int northRow = currentCell.row - 2;
					int northColumn = currentCell.column;
					int eastRow = currentCell.row;
					int eastColumn = currentCell.column + 2;
					int southRow = currentCell.row + 2;
					int southColumn = currentCell.column;

					if (GetCellValue(maze, northRow, northColumn) == 1 && GetCellValue(maze, eastRow, eastColumn) == 1 &&
						GetCellValue(maze, southRow, southColumn) == 1)
					{
						RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
					}
				}
			}
			RemoveCell(cellsToExpand, currentCell.row, currentCell.column);
		}
	}
	return maze;
}

int main(int argc, const char * argv[])
{
	//seeding the random number generator
	srand((int)time(NULL));
	Maze m = MakeMaze();
	PrintMaze(m);

	system("pause");
}

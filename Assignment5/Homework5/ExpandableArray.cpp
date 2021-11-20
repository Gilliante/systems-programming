#include "ExpandableArray.h"
#include <string.h>

//Gillian Ehman 
//Collaberate with Samantha Holstine

//Constructs theArray with an initial size of 16.
ExpandableArray::ExpandableArray()
{
	theArray = new char*[16];
	maxSize = 16;
	currentIndex = 0;
}

//Constructs the array with the specified initial size
ExpandableArray::ExpandableArray(int initialSize)
{
	theArray = new char*[initialSize];
	maxSize = initialSize;
	currentIndex = 0;
}

// Free the memory allocated for theArray
ExpandableArray::~ExpandableArray()
{
	delete[] theArray;
}

//Append an element to the end of theArray as follows:
//a) If there is space in the array, add in location of next free element and return.
//b) If not, then expand the array.
//c) Add the element at the end
void ExpandableArray::Append(char* element)
{
	if (currentIndex == maxSize)
	{
		ExpandArray();
	}
	theArray[currentIndex] = element;
	currentIndex++;
}

//Sort the pointers to words in order of length from longest to shortest.  Hint: You can determine the words length using strlen
void ExpandableArray::Sort()
{
	if (currentIndex <= 1)
	{
		return;
	}
	else
	{
		for (int i = 1; i < currentIndex; i++)
		{
			char* word = theArray[i];
			int j = i - 1;

			while (j >= 0 && strlen(word) < strlen(theArray[j]))
			{
				theArray[j + 1] = theArray[j];
				j--;
			}
			theArray[j + 1] = word;
		}
	}
}

//Returns the number of elements in the array right now.
int ExpandableArray::length()
{
	return currentIndex;
}

//Return the element at index i in the array
char* ExpandableArray::get(int i)
{
	return theArray[i];
}

//A private method that expands the array.  The array doubles in size when it fills up. Expand as follows:
//a) make new array that is twice the size of the old one.
//b) Copy the old array into the new array.
//c) Destroy the old
void ExpandableArray::ExpandArray()
{
	//ExpandableArray newArray = ExpandableArray(maxSize * 2);
	char** newArray = new char*[maxSize * 2];
	for (int i = 0; i < maxSize; i++)
	{
		newArray[i] = theArray[i];
	}
	maxSize *= 2;
	delete[] theArray;
	theArray = newArray;
}
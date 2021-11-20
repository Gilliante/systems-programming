#ifndef ExpandableArray_h
#define ExpandableArray_h

#include <stdio.h>

class ExpandableArray
{
public:

	//Constructs theArray with an initial size of 16.
	ExpandableArray();

	//Constructs the array with the specified initial size
	ExpandableArray(int initialSize);

	// Free the memory allocated for theArray
	~ExpandableArray();

	//Append an element to the end of theArray as follows:
	//a) If there is space in the array, add in location of next free element and return.
	//b) If not, then expand the array.
	//c) Add the element at the end
	void Append(char* element);

	//Sort the pointers to words in order of length from longest to shortest.  Hint: You can determine the words length using strlen
	void Sort();

	//Returns the number of elements in the array right now.
	int length();

	//Return the element at index i in the array
	char* get(int i);

private:

	//This array contains pointers to c-strings.
	//Note that this an array of pointers to characters such that each pointer points to the first character of a word.  The words will be stored in a single string in BearHelper.
	char** theArray;

	//This is the size of the expandableArray.  When the currentIndex reaches the maxSize then the array needs to expand
	int maxSize;

	//The currentIndex is an index to the next open spot in the array. Therefore currentIndex tells you the number of elements in the array
	int currentIndex;

	//A private method that expands the array.  The array doubles in size when it fills up. Expand as follows:
	//a) make new array that is twice the size of the old one.
	//b) Copy the old array into the new array.
	//c) Destroy the old
	void ExpandArray();

};
#endif /* ExpandableArray_h */
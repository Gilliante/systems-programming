#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <fstream>
#include "BearHelper.h"

#include "ExpandableArray.h"

//Gillian Ehman
//Collaberated with Samantha Holstine

/*Constructor:  In the constructor you will read the entire file into a single string and then replace the newline
	characters with null characters.*/
	//This method constructs BearHelper object by reading the file f into the dictionary string
BearHelper::BearHelper(const char *f)
{
	//fseek to end of dictionary, ftell tells you how long it is
	//Open the file for reading
	FILE* dictionaryFile = fopen(f, "rb+");

	//Determine the number of characters in file.  Hint: you can use fseek and ftell.
	fseek(dictionaryFile, 0, SEEK_END);
	int numOfChars = ftell(dictionaryFile);

	//Allocate a string large enough for the entire dictionary.
	size = numOfChars;
	theDictionary = new char[size];

	//Read the entire file into the string allocated
	rewind(dictionaryFile);
	fread(theDictionary, sizeof(char), size, dictionaryFile);
	char chars = 0;

	for (int i = 0; i < size; i++)
	{
		chars = theDictionary[i];
		if (chars == '\n')
		{
			theDictionary[i] = '\0';
		}
	}
	//close the file
	fclose(dictionaryFile);
}


//Destructor for the BearHelper to free the memory used by dictionary
//Destructor: Destroy the string that stores the dictionary.
BearHelper::~BearHelper()
{
	delete[] theDictionary;
}


/*
 * Returns true if all letters in "letters" are in group of letters "group" (duplicates should be accounted for)
 * therefore the letters: "ee" are in group "tree" but not in group "tea"
 *
 * parameter: letters - list of letters, group - a collection of letter
 * return: true of all letters are in set
 */
bool BearHelper::AllLettersInSet(const char *letters, const char *set)
{
	int l[26];
	int s[26];
	for (int x = 0; x < 26; x++)
		l[x] = s[x] = 0;

	for (const char *t = letters; *t; t++)
	{
		l[tolower(*t) - 97]++;
	}
	for (const char *t = set; *t; t++)
	{
		s[tolower(*t) - 97]++;
	}
	for (int x = 0; x < 26; x++)
		if (l[x] > s[x])
			return false;
	return true;
}


/*GetAllWords: prints all words that work for the required and available sets in order of length(longest to shortest).
	detailed instructions are in the BearHelper.cpp file.*/
	//Print all words that satisfy the condition: All characters in required must be in word and all letters in word must be in available
	//The words are printed in order of length from longest to shortest.  
void BearHelper::GetAllWords(const char* required, const char* available)
{
	ExpandableArray dictionaryArray = ExpandableArray();
	char* wordPtr = theDictionary;
	char* word;
	int length = 0;

	//Go in order through the dictionary.
	for (int i = 0; i < size; i += length + 1)
	{
		length = strlen(wordPtr);
		word = new char[length + 1];
		strcpy(word, wordPtr);
		word[length] = '\0';
		wordPtr += length + 1;

		//Check if each word matches.
		if (AllLettersInSet(required, word) && AllLettersInSet(word, available))
		{
			//If it does, add it to the expandable set.
			dictionaryArray.Append(wordPtr);
		}
	}
	//Sort the expandable set.
	dictionaryArray.Sort();

	//Print the expandable set.
	for (int i = 0; i < dictionaryArray.length(); i++)
	{
		std::cout << dictionaryArray.get(i) << "\n";
	}
}

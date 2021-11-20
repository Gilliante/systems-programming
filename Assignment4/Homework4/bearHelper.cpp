#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <fstream>
#include "bearHelper.h"

//Gillian Ehman

/*
 * Counts the number of words in a file named f
 * parameter: file name
 * return: number of words in file
 */
int GetNumEntriesInFile(const char *f)
{
	//1. Open the file for reading
	FILE* dictionaryFile = fopen(f, "rb+");

	//2. Count the number of words in file.  Hint: You can count the number of '\n' characters in file. 
	//	 Use fgetc to read one character at a time.
	int chars = 0;
	int numOfWords = 0;

	while (chars != EOF)
	{
		chars = fgetc(dictionaryFile);
		if (chars == '\n')
		{
			numOfWords++;
		}
	}

	//3. close file
	fclose(dictionaryFile);

	//4.return count
	return numOfWords;
}

/*
 * Creates an array of strings of size count and populates it with words in file named f
 * parameter: f - file name, count - number of words in file
 * return: An array of c-strings
 */
char **ReadWords(const char *f, int count)
{
	//1. Open file for reading
	FILE* wordFile = fopen(f, "rb+");

	//2. Allocate the array of c-strings
	char** words = new char*[count];

	//3. For each word in the file
	//3.a Determine the length of word
	//   (hint: use ftell to determine the location of the pointer in file before you start counting the letters, 
	//	  use fgetc to read a single character, then fseek to the location of the start of the word and read it).
	//3.b Allocate a string long enough to store word
	//3.c store word in array of words
	int index = 0;
	int chars = 0;
	int numOfChars = 0;
	int location = ftell(wordFile);

	while (chars != EOF)
	{
		chars = fgetc(wordFile);
		if (chars != '\n')
		{
			numOfChars++;
		}
		else if (chars == '\n')
		{
			char* word = new char[numOfChars + 1];

			fseek(wordFile, -(numOfChars+1), SEEK_CUR);
			fread(word, numOfChars, 1, wordFile);
			location = ftell(wordFile);
			word[numOfChars] = '\0';
			fseek(wordFile, sizeof(char), SEEK_CUR);
			words[index] = word;
			index++;
			numOfChars = 0;
		}
	}

	//4. close file
	fclose(wordFile);

	//5. return array of words
	return words;
}

/*
 * Returns true if all letters in "letters" are in group of letters "group" (duplicates should be accounted for)
 * therefore the letters: "ee" are in group "tree" but not in group "tea"
 *
 * parameter: letters - list of letters, group - a collection of letter
 * return: true if all letters are in set
 */
bool AllLettersInSet(const char *letters, const char *group)
{
	int count = 0;

	for (int i = 0; i < strlen(letters); i++)
	{
		for (int j = 0; j < strlen(group); j++)
		{
			if (letters[i] == group[j])
			{
				count++;
				break;
			}
			//otherwise if they aren't a match, keep going through the group array
		}
	}
	if (count == strlen(letters))
	{
		return true;
	}
	return false;
}

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unordered_map>
#include <fstream>

#include "bearHelper.h"

int main(int argc, char **argv)
{
	//This is the group of letters that must be present in the word
	const char *required = "ak";

	//Group of available letters (note that 'required' is a subset of available letters)
	const char *available = "akdid";

	const char *file = "Dictionary.txt";
	int count = GetNumEntriesInFile(file);
	char **words = ReadWords(file, count);

	std::cout << count << "\n";

	//Walk the list of words and remove any word that doesn't contain all required letters or words that contain letters not in the available group.
	for (int x = 0; x < count; x++)
	{
		if (AllLettersInSet(required, words[x]) && AllLettersInSet(words[x], available))
		{
			// keep this word
		}
		else
		{
			delete[] words[x];
			words[x] = 0;
		}
	}

	//All surviving words satify the query and should be reported
	for (int x = 0; x < count; x++)
	{
		if (words[x] != 0)
			printf("%lu %s\n", strlen(words[x]), words[x]);
	}

	system("pause");
}
#ifndef __FunctionTemplates__bearHelper__
#define __FunctionTemplates__bearHelper__

#include <stdio.h>


class BearHelper
{
public:
	BearHelper(const char *);
	~BearHelper();
	void GetAllWords(const char* required, const char* available);

private:

	//A string that will store the entire dictionary
	char* theDictionary;

	//The size of the dictionary (number of characters)
	int size;


	bool AllLettersInSet(const char *letters, const char *set);

};


#endif /* defined(__FunctionTemplates__bearHelper__) */
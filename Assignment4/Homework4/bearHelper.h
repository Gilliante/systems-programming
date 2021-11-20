#ifndef __FunctionTemplates__bearHelper__
#define __FunctionTemplates__bearHelper__

#include <stdio.h>

int GetNumEntriesInFile(const char *f);
char **ReadWords(const char *f, int count);
bool AllLettersInSet(const char *letters, const char *set);


#endif /* defined(__FunctionTemplates__bearHelper__) */
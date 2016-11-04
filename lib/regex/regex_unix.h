#ifndef REGEX_WIN_UNIX
#define REGEX_WIN_UNIX

#include <regex.h>
#include <stdbool.h>

bool matchRegex(char* reg_expression, char* str) {
	regex_t reg;
	
	regcomp(&reg, reg_expression, REG_EXTENDED);

    if (regexec(&reg, str, 0, (regmatch_t *)NULL, 0) == 0) {
    	return true;
    }
    
    return false;
}

#endif

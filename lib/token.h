#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Import regex lib by OS
#ifdef _WIN32
#include "regex/regex_win.h"
#else
#include "regex/regex_unix.h"
#endif

typedef struct  token {
    char* name;
    char* pattern;
} Token;

typedef struct symbol {
    int position;
    Token token;

    char* lexeme;
    char* dataType;
    int line;
    int column;
} Symbol;

typedef struct symbolNode {
    Symbol item;
    struct symbolNode* next;
    struct symbolNode* prev;

} SymbolNode;

SymbolNode* symbolList;
int tokenListLength = 1;

void addSymbolInSymbolList(Token token, char* lexeme, int line, int column) {
    SymbolNode* newSymbol = (SymbolNode*) malloc(sizeof(SymbolNode));

    newSymbol->item.column = column;
    newSymbol->item.lexeme = (char*) malloc(sizeof(lexeme));
    strcpy(newSymbol->item.lexeme, lexeme);
    newSymbol->item.line = line;
    newSymbol->item.position = tokenListLength;
    newSymbol->item.token = token;
    newSymbol->next = NULL;

    if (symbolList == NULL) {
        symbolList = newSymbol;
    } else {
        SymbolNode* auxList = symbolList;

        while (auxList->next != NULL) {
            // Try to find a duplicated entry in the symbol list
            if (strcmp(newSymbol->item.lexeme, auxList->item.lexeme) == 0 && strcmp(newSymbol->item.token.name, auxList->item.token.name) == 0) {
                newSymbol->item.position = auxList->item.position;
            }

            auxList = auxList->next;
        }

        auxList->next = newSymbol;
        newSymbol->prev = auxList;
    }

    tokenListLength++;
}

void initializeTokenDict(Token* tokenDict) {
    Token token1 = {"end_of_statement", "^;"};
    tokenDict[0] = token1;

    Token token2 = {"attribution", "^[=]{1}$"};
    tokenDict[1] = token2;

    Token token3 = {"operator", "^[-|\\+|/|*]{1}$"};
    tokenDict[2] = token3;

    Token token4 = {"id", "^[a-zA-z]+"};
    tokenDict[3] = token4;

    Token token5 = {"number", "^[0-9]+"};
    tokenDict[4] = token5;

    Token token6 = {"comparison", "^[<=|==|>=|!=|<|>]"};
    tokenDict[5] = token6;

    Token token7 = {"open_parentheses", "^[\\(]"};
    tokenDict[6] = token7;

    Token token8 = {"close_parentheses", "^[\\)]"};
    tokenDict[7] = token8;

    Token token9 = {"addition_operator", "^[\\+\\+]{2}$"};
    tokenDict[8] = token9;

    Token token10 = {"subtraction_operator", "^[\\-\\-]{2}$"};
    tokenDict[9] = token10;

    Token token11 = {"power_operator", "^[**]{2}$"};
    tokenDict[10] = token11;
}

Token getTokenFromLexeme(char* lexeme, Token* tokenDict) {
    Token token;
    int i;

    token.name = NULL;

    for (i = 0; i < 11; i++) {
    	
        if (matchRegex(tokenDict[i].pattern, lexeme)) {
            token = tokenDict[i];
            break;
        }
    }

    return token;
}

Token getTokenFromChar(char ch, Token* tokenDict) {
    Token token;
    int i;

    token.name = NULL;

    for (i = 0; i < 11; i++) {
    	
        if (matchRegex(tokenDict[i].pattern, &ch)) {
            token = tokenDict[i];
            break;
        }
    }

    return token;
}

#endif

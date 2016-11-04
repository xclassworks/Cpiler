#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "lib/token.h"
#include "lib/platform.h"

void printSymbolList(SymbolNode* symbolList) {
    SymbolNode* auxList = symbolList;

    while (auxList != NULL) {
        printf("<%s, %d>", auxList->item.token.name, auxList->item.position);

        auxList = auxList->next;
    }
}

void saveSymbolListInFile(SymbolNode* symbolList) {
    SymbolNode* auxList = symbolList;
    char cwd[1024];

    getcwd(cwd, sizeof(cwd));

    strcat(cwd, PATH_SEPARATOR);
    strcat(cwd, "sp_OUTPUT.txt");

    printf("\n[INFO] Criando arquivo de saida em %s\n", cwd);

    FILE *outputTokenFile = fopen(cwd, "w");

    if (outputTokenFile == NULL) {
        printf("[ERROR] Não foi possivel salvar o arquivo!");
        exit(1);
    } else {
        // Add tokens
        while (auxList != NULL) {
            fprintf(outputTokenFile, "<%s, %d>", auxList->item.token.name, auxList->item.position);

            auxList = auxList->next;
        }

        fputs("\n\n", outputTokenFile);

        auxList = symbolList;

        // Add symbol list
        while (auxList != NULL) {
            fprintf(outputTokenFile, "<%s, %d> LEXEMA: %s LINHA: %d COLUNA: %d\n", auxList->item.token.name, auxList->item.position,
                    auxList->item.lexeme, auxList->item.line, auxList->item.column);

            auxList = auxList->next;
        }
    }

    fclose(outputTokenFile);
}

bool canBeComposedTokenType(Token token, char* buffer) {

    if (strlen(buffer) > 1) {
        return false;
    }

    if (!strcmp(token.name, "operator") || !strcmp(token.name, "attribution") || !strcmp(token.name, "comparison")) {
        return true;
    }

    return false;
}

void cleanBuffer(char* buffer, int* bufferIndex) {
	
	if (strlen(buffer) > 0) {
		free(buffer);
        buffer = calloc(80, sizeof(char));
        *bufferIndex = 0;
	}
}

int main() {
    FILE* sourceProgram;
    Token tokenDict[11];
    char cwd[1024];

    initializeTokenDict(tokenDict);

    getcwd(cwd, sizeof(cwd));

    strcat(cwd, PATH_SEPARATOR);
    strcat(cwd, "sp.txt");

    printf("[INFO] Abrindo arquivo %s\n", cwd);

    sourceProgram = fopen(cwd, "r");

    if (sourceProgram == NULL) {
        printf("[ERROR] Não foi possível abrir o arquivo!");
        exit(1);
    } else {
    	printf("[INFO] Construindo tabela de tokens\n");
    	
        char ch;
        char* buffer = calloc(80, sizeof(char));
        int bufferIndex = 0;

        int line = 1, column = 1;

        while (!feof(sourceProgram)) {
            ch = fgetc(sourceProgram);
            Token tokenChar = getTokenFromChar(ch, tokenDict);

            if (tokenChar.name != NULL && strcmp(tokenChar.name, "id") && strcmp(tokenChar.name, "number")) {

                if (canBeComposedTokenType(tokenChar, buffer)) {
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                } else {
                    Token bufferToken = getTokenFromLexeme(buffer, tokenDict);

                    if (bufferToken.name != NULL) {
                        int columnBuffer = column - strlen(buffer);

                        addSymbolInSymbolList(bufferToken, buffer, line, columnBuffer);

                        cleanBuffer(buffer, &bufferIndex);
                    }

                    if (canBeComposedTokenType(tokenChar, buffer)) {
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                    } else {
                        addSymbolInSymbolList(tokenChar, &ch, line, column);
                    }
                }
            } else {

                switch (ch) {
                    case 32:
                        // Space
                        if (strlen(buffer) > 0) {
                            Token bufferToken = getTokenFromLexeme(buffer, tokenDict);

                            if (bufferToken.name != NULL) {
                                int columnBuffer = column - strlen(buffer);

                                addSymbolInSymbolList(bufferToken, buffer, line, columnBuffer);

                                cleanBuffer(buffer, &bufferIndex);
                            }
                        }
                        break;
                    case 10:
                        // Line_break
                        if (strlen(buffer) > 0) {
                            Token bufferToken = getTokenFromLexeme(buffer, tokenDict);

                            if (bufferToken.name != NULL) {
                                int columnBuffer = column - strlen(buffer);

                                addSymbolInSymbolList(bufferToken, buffer, line, columnBuffer);

                                cleanBuffer(buffer, &bufferIndex);
                            }
                        }

                        line++;
                        column = 0;
                        break;
                    default:
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        break;
                }
            }

            column++;
        }

        fclose(sourceProgram);

        printSymbolList(symbolList);
        saveSymbolListInFile(symbolList);
    }

    return 0;
}

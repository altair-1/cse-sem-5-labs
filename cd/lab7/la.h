#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token{
    char token_name[10];
    int row, col;
    int index;
    char type[30];
};

unsigned int row = 1, col = 1;
char buf[024];

const char specialsymbols[] = {'?', ';', ':', ',', '(', ')', '{', '}', '[', ']'};
const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
const char arithmeticsymbols[] = {'*', '/', '-', '+', '^'};
const char *DataTypes[] = {"int", "char"};

int charBelongsTo(int c, char t){
    int len;
    const char *arr;
    if (t == 'a'){
        len = sizeof(arithmeticsymbols) / sizeof(char); 
        arr = arithmeticsymbols;
    }
    else if (t == 's'){
        len = sizeof(specialsymbols) / sizeof(char); 
        arr = specialsymbols;
    }
    for (int i = 0; i < len; i++){
        if (c == arr[i])
            return 1;
    }
    return 0;
}

int isDataType(const char *str){
    for (int i = 0; i < sizeof(DataTypes) / sizeof(char *); i++){
        if (strcmp(str, DataTypes[i]) == 0)
            return 1;
    }
    return 0;
}

int isKeyword(const char *str){
    for (int i = 0; i < sizeof(keywords) / sizeof(char *); i++){
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void fillToken(struct token *tkn, char c, int row, int col, char *type){
    tkn->row = row;
    tkn->col = col;
    strcpy(tkn->type, type);
    tkn->token_name[0] = c;
    tkn->token_name[1] = '\0';
}

void newLine(){
    ++row;
    col = 1;
}

struct token getNextToken(FILE *fin){
    int c;
    struct token tkn = {.row = -1};
    int gotToken = 0;
    char *prevType;
    while (!gotToken && (c = fgetc(fin)) != EOF){
        if (charBelongsTo(c, 'a')){
            int d = fgetc(fin);
            ++col;
            if (c == '/'){
                if (d == '/'){
                    while ((c = fgetc(fin)) != EOF && c != '\n')
                        ++col;
                    if (c == '\n')
                        newLine();
                    continue;
                }
                else if (d == '*'){
                    do{
                        if (d == '\n')
                            newLine();
                        while ((c = fgetc(fin)) != EOF && c != '*'){
                            ++col;
                            if (c == '\n')
                                newLine();
                        }
                        ++col;
                    } while ((d = fgetc(fin)) != EOF && d != '/' && (++col));
                    ++col;
                    continue;
                }
            }
            else if (c == '+' && d == '+'){
                fillToken(&tkn, c, row, --col, "UnaryOperator");
                strcpy(tkn.token_name, "++");
                gotToken = 1;
                fseek(fin, -1, SEEK_CUR);
                continue;
            }
            else if (c == '-' && d == '-'){
                fillToken(&tkn, c, row, --col, "UnaryOperator");
                strcpy(tkn.token_name, "--");
                gotToken = 1;
                fseek(fin, -1, SEEK_CUR);
                continue;
            }
            else if (c == '*' && strcmp(prevType, "Keyword") == 0){
                fillToken(&tkn, c, row, --col, "SpecialSymbol");
                gotToken = 1;
                fseek(fin, -1, SEEK_CUR);
                continue;
            }
            fillToken(&tkn, c, row, --col, "ArithmeticOperator");
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }
        else if (charBelongsTo(c, 's')){
            fillToken(&tkn, c, row, col, "SpecialSymbol");
            gotToken = 1;
            ++col;
        }
        else if (c == '='){
            int d = fgetc(fin);
            if (d == '='){
                fillToken(&tkn, c, row, col, "RelationalOperator");
                strcpy(tkn.token_name, "==");
                gotToken = 1;
                col += 2;
            }
        }
        else if (c == '#'){
            while ((c = fgetc(fin)) != EOF && c != '\n');
            newLine();
        }
        else if (c == '\n'){
            newLine();
        }
        else if (c == '<' || c == '>' || c == '!'){
            fillToken(&tkn, c, row, col, "RelationalOperator");
            ++col;
            int d = fgetc(fin);
            if (d == '='){
                ++col;
                strcat(tkn.token_name, "=");
            }
            else{
                if (c == '!')
                    strcpy(tkn.type, "LogicalOperator");
                fseek(fin, -1, SEEK_CUR);
            }
            gotToken = 1;
        }
        else if (c == '&' || c == '|'){
            int d = fgetc(fin);
            if (c == d){
                fillToken(&tkn, c, row, col, "LogicalOperator");
                if (c == '&'){
                    strcat(tkn.token_name, "&");
                }
                else{
                    strcat(tkn.token_name, "|");
                }
                ++col;
                gotToken = 1;
            }
            else
                fseek(fin, -1, SEEK_CUR);
            ++col;
        }
        else if (isdigit(c)){
            tkn.row = row;
            tkn.col = col++;
            tkn.token_name[0] = c;
            int k = 1;
            while ((c = fgetc(fin)) != EOF && isdigit(c)){
                tkn.token_name[k++] = c;
                col++;
            }
            strcpy(tkn.type, "Number");
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }
        else if (c == '"'){
            tkn.row = row;
            tkn.col = col;
            strcpy(tkn.type, "StringLiteral");
            int k = 1;
            tkn.token_name[0] = '"';
            while ((c = fgetc(fin)) != EOF && c != '"'){
                tkn.token_name[k++] = c;
                ++col;
            }
            tkn.token_name[k] = '"';
            gotToken = 1;
        }
        else if (isalpha(c) || c == '_'){
            tkn.row = row;
            tkn.col = col++;
            tkn.token_name[0] = c;
            int k = 1;
            while ((c = fgetc(fin)) != EOF && isalnum(c)){
                tkn.token_name[k++] = c;
                ++col;
            }
            tkn.token_name[k] = '\0';
            if (isKeyword(tkn.token_name)){
                strcpy(tkn.type, "Keyword");
                prevType = "Keyword";
            }
            else{
                strcpy(tkn.type, "Identifier");
                prevType = "Identifier";
            }
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }
        else if (isspace(c))
            ++col;
        else{
            ++col;
            prevType = "";
        }
    }
    return tkn;
}
#include <stdio.h>
#include <ctype.h>
#include "def.c"

int ind = 0;
int func_flag = 0;
int identifier_count = 0;
int row = 1;
int col = 0;

TableElement *list = NULL;
Symbol symbol;

const char *keywords[] = {"auto", "break", "case", "char", "continue", "do", "default", "const", "double", "else", "enum",
                          "extern", "for", "if", "goto", "float", "int", "long", "register", "return", "signed", "static",
                          "sizeof", "short", "struct", "switch", "typedef", "union", "void", "while", "volatile", "unsigned"};

void displayToken(Token token){
    // printf("<%s , %s , %d , %d>\n", token.type, token.token_name, token.row_no, token.column_no);
    printf("< %s , %s >\n", token.type, token.token_name);
}

void removePreprocessorDirectivesAndComments(FILE *f1, FILE *f2){
    char line[256];
    int insideQuotes = 0;
    char ch = fgetc(f1);
    while (ch != EOF){
        if (ch == '"'){
            insideQuotes = !insideQuotes;
            fputc(ch, f2);
            ch = fgetc(f1);
        }
        if (ch == '/' && !insideQuotes){
            ch = fgetc(f1);
            if (ch == '/'){
                do{
                    ch = fgetc(f1);
                } while (ch != '\n');
            }
            else if (ch == '*'){
                do{
                    while (ch != '*')
                        ch = fgetc(f1);
                    ch = fgetc(f1);
                } while (ch != '/');
                ch = fgetc(f1);
            }
            else{
                fseek(f1, -2, SEEK_CUR);
                ch = fgetc(f1);
            }
        }
        if (ch == '#' && !insideQuotes && func_flag == 0){
            do{
                ch = fgetc(f1);
            } while (ch != '\n');
        }
        else{
            if (ch == '{')
                func_flag++;
            else if (ch == '}')
                func_flag--;
            else if (ch == '\n'){
                row++;
                col = 0;
            }
            fputc(ch, f2);
            ch = fgetc(f1);
        }
    }
}

int isSpecial(char ch){
    if (ch == '\'' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '$' || ch == '[' || ch == ']' || ch == '.' || ch == ',' || ch == ':' || ch == ';')
        return 1;
    return 0;
}

int isOtherRelop(char ch){
    if (ch != '>' && ch != '<' && ch != '=' && ch != '!')
        return 1;
    return 0;
}

int isOtherArithmop(char ch){
    if (ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '%')
        return 1;
    return 0;
}

int isOtherLogop(char ch){
    if (ch != '&' && ch != '|' && ch != '!')
        return 1;
    return 0;
}

int isKeyword(char str[]){
    for (int k = 0; k < 32; k++){
        if (strcmp(keywords[k], str) == 0)
            return 1;
    }
    return 0;
}

Token getAllOperators(FILE *f){
    Token token;
    fseek(f, -1, SEEK_CUR);
    char ch1 = fgetc(f);
    char ch2 = fgetc(f);
    if (ch1 == '!' && ch2 == '='){
        strcpy(token.type, "NE");
        strcpy(token.token_name, "!=");
    }
    else if (ch1 == '<' && ch2 == '='){
        strcpy(token.type, "LE");
        strcpy(token.token_name, "<=");
    }
    else if (ch1 == '<' && isOtherRelop(ch2)){
        strcpy(token.type, "LT");
        strcpy(token.token_name, "<");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '=' && ch2 == '='){
        strcpy(token.type, "EQ");
        strcpy(token.token_name, "==");
    }
    else if (ch1 == '>' && ch2 == '='){
        strcpy(token.type, "GE");
        strcpy(token.token_name, ">=");
    }
    else if (ch1 == '>' && isOtherRelop(ch2)){
        strcpy(token.type, "GT");
        strcpy(token.token_name, ">");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '=' && isOtherRelop(ch2)){
        strcpy(token.type, "Assignment");
        strcpy(token.token_name, "=");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '+' && isOtherArithmop(ch2)){
        strcpy(token.type, "ADD");
        strcpy(token.token_name, "+");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '-' && isOtherArithmop(ch2)){
        strcpy(token.type, "SUB");
        strcpy(token.token_name, "-");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '/' && isOtherArithmop(ch2)){
        strcpy(token.type, "DIV");
        strcpy(token.token_name, "/");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '*' && isOtherArithmop(ch2)){
        strcpy(token.type, "MUL");
        strcpy(token.token_name, "*");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '%' && isOtherArithmop(ch2)){
        strcpy(token.type, "MOD");
        strcpy(token.token_name, "%");
        fseek(f, -1, SEEK_CUR);
    }
    else if (ch1 == '&' && ch2 == '&'){
        strcpy(token.type, "Logical AND");
        strcpy(token.token_name, "&&");
    }
    else if (ch1 == '|' && ch2 == '|'){
        strcpy(token.type, "Logical OR");
        strcpy(token.token_name, "||");
    }
    else if (ch1 == '!' && isOtherLogop(ch2)){
        strcpy(token.type, "Logical NOT");
        strcpy(token.token_name, "!");
        fseek(f, -1, SEEK_CUR);
    }
    // token[ind].row_no = row;
    // token[ind].column_no = *i;
    return token;
}

Token getKeywordsAndIdentifiers(FILE *f){
    char word[256];
    char str[100];
    int j = 0;
    int argCount = 0;
    Token token;
    fseek(f, -1, SEEK_CUR);
    char ch = fgetc(f);
    while (isalpha(ch) || ch == '_'){
        word[j++] = ch;
        ch = fgetc(f);
    }
    fseek(f, -1, SEEK_CUR);
    word[j] = '\0';
    if (isKeyword(word)){
        strcpy(token.type, "KW");
        strcpy(token.token_name, word);
        token.row_no = row;
        // token[ind].column_no = *i - strlen(word);
    }
    else if (!isKeyword(word)){
        int id = search(word, &list);
        if (id == 0){
            identifier_count++;
            sprintf(str, "id-%d", identifier_count);
            strcpy(token.token_name, word);
            strcpy(token.type, str);
            token.row_no = row;
            ch = fgetc(f);
            int offset = 1;
            symbol.index = identifier_count;
            strcpy(symbol.symbolName, word);
            if (ch == '('){
                strcpy(symbol.type, "FUNC");
                if (strcmp(word, "printf") == 0){
                    strcpy(symbol.returnType, "int");
                    sprintf(symbol.argNum, "%d", 1);
                }
                else{
                    ch = fgetc(f);
                    offset++;
                    if (ch == ')'){
                        sprintf(symbol.argNum, "%d", 0);
                    }
                    else{
                        do{
                            ch = fgetc(f);
                            offset++;
                            if (ch == ',')
                                argCount++;
                        } while (ch != ')');
                        argCount++;
                        sprintf(symbol.argNum, "%d", argCount);
                    }
                    strcpy(symbol.returnType, token.token_name);
                }
            }
            else{
                strcpy(symbol.type, token.token_name);
                strcpy(symbol.returnType, "--");
                sprintf(symbol.argNum, "%d", 0);
            }
            offset *= -1;
            fseek(f, offset, SEEK_CUR);
            list = insert(symbol, list);
        }
        else{
            sprintf(str, "id-%d", id);
            strcpy(token.token_name, word);
            strcpy(token.type, str);
            // token[ind].row_no = row;
            // token[ind].column_no = *i - strlen(word);
        }
        ind++;
    }
    strcpy(word, "");
    j = 0;
    return token;
}

Token getStringLiterals(FILE *f){
    char lit[256];
    int j = 0;
    Token token;
    char ch = fgetc(f);
    // token.column_no = ftell(f);
    while (ch != '"'){
        lit[j++] = ch;
        ch = fgetc(f);
    }
    lit[j] = '\0';
    if (strlen(lit) != 0){
        strcpy(token.type, "String Literal");
        strcpy(token.token_name, lit);
        // token.row_no = row;
        // ind++;
        strcpy(lit, "");
    }
    return token;
}

Token getCharacterLiterals(FILE *f){
    char lit[256];
    int j = 0;
    // token[ind].column_no = (*i)++;
    Token token;
    char ch = fgetc(f);
    while (ch != '\''){
        lit[j++] = ch;
        ch = fgetc(f);
    }
    lit[j] = '\0';
    if (strlen(lit) != 0){
        strcpy(token.type, "Character Literal");
        strcpy(token.token_name, lit);
        // token[ind].row_no = row;
        // ind++;
        strcpy(lit, "");
    }
    return token;
}

Token getNumericLiterals(FILE *f){
    char lit[256];
    int j = 0;
    Token token;
    // token[ind].column_no = *i;
    fseek(f, -1, SEEK_CUR);
    char ch = fgetc(f);
    while (isdigit(ch) || ch == '.'){
        lit[j++] = ch;
        ch = fgetc(f);
    }
    fseek(f, -1, SEEK_CUR);
    lit[j] = '\0';
    if (strlen(lit) != 0){
        strcpy(token.type, "Numeric Literal");
        strcpy(token.token_name, lit);
        // token[ind].row_no = row;
        // ind++;
        strcpy(lit, "");
        // (*i)--;
    }
    return token;
}

Token getNextToken(FILE *f){
    Token token;
    char ch = fgetc(f);
    while (isspace(ch))
        ch = fgetc(f);
    while (ch == '#'){
        printf("%c", ch);
        do{
            ch = fgetc(f);
        } while (ch != '\n');
        ch = fgetc(f);
    }
    if (ch == EOF)
        exit(0);
    if (ch == '"')
        return getStringLiterals(f);
    else if (ch == '\'')
        return getCharacterLiterals(f);
    else if (isalpha(ch) || ch == '_')
        return getKeywordsAndIdentifiers(f);
    else if (isdigit(ch))
        return getNumericLiterals(f);
    else if (isSpecial(ch)){
        strcpy(token.type, "Special Symbol");
        token.token_name[0] = ch;
        token.token_name[1] = '\0';
        token.row_no = row;
        return token;
    }
    else
        return getAllOperators(f);
}
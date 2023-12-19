#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define hash(a) a%10

struct token
{
    char token_name[128];
    int row, col;
    int index;
    char type[30];
    char ttype[30];
    char rtype[30];
    char dtype[30];
    int args;
    int id;
};

typedef struct symbol
{
    int sno;
    char token_name[128];
    char dtype[30];
    char rtype[30];
    char ttype[30];
    int args;
    struct symbol* next;
} symbol;

symbol* table[10];

int gotOpenBracket = 0;
unsigned int row = 1, col = 1;
unsigned int tindex = 0;
char buf[024];

const char specialsymbols[] = {'?', ';', ':', ',', '(', ')', '[', ']'};
const char *keywords[] = {"sizeof", "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
const char arithmeticsymbols[] = {'*', '/', '-', '+', '^'};
const char *predefinedFunc[] = {"printf", "scanf"};

int charBelongsTo(int c, char t)
{
    int len;
    const char *arr;
    if (t == 'a'){
        len = sizeof(arithmeticsymbols) / sizeof(char); 
        arr = arithmeticsymbols;
    }
    else if (t == 's'){
        len = sizeof(specialsymbols) / sizeof(char); 
        arr = specialsymbols;
        if (c == '}'){
            if (gotOpenBracket != 1)
                gotOpenBracket--;
            return 1;
        }
        if (c == '{'){
            gotOpenBracket++;
            return 1;
        }
    }
    for (int i = 0; i < len; i++)
    {
        if (c == arr[i])
            return 1;
    }
    return 0;
}

int isKeyword(const char *str)
{
    for (int i = 0; i < sizeof(keywords) / sizeof(char *); i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isPredefinedFunc(const char *str)
{
    for(int i = 0; i < sizeof(predefinedFunc) / sizeof(char *); i++){
        if (strcmp(str, predefinedFunc[i]) == 0)
            return 1;
    }
    return 0;
}

void fillToken(struct token *tkn, char c, int row, int col, char *type)
{
    tkn->row = row;
    tkn->col = col;
    strcpy(tkn->type, type);
    tkn->token_name[0] = c;
    tkn->token_name[1] = '\0';
}

void newLine()
{
    ++row;
    col = 1;
}

struct token getNextToken(FILE *fin, struct token prevToken)
{
    int c;
    struct token tkn = {.row = -1, .id = 0};
    int gotToken = 0;
    char *prevType;
    while (!gotToken && (c = fgetc(fin)) != EOF)
    {
        if (charBelongsTo(c, 'a'))
        {
            int d = fgetc(fin);
            ++col;
            if (c == '/'){
                if (d == '/')
                {
                    while ((c = fgetc(fin)) != EOF && c != '\n')
                        ++col;
                    if (c == '\n')
                        newLine();
                    continue;
                }
                else if (d == '*')
                {
                    do
                    {
                        if (d == '\n')
                            newLine();
                        while ((c = fgetc(fin)) != EOF && c != '*')
                        {
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
        else if (charBelongsTo(c, 's'))
        {
            fillToken(&tkn, c, row, col, "SpecialSymbol");
            gotToken = 1;
            ++col;
        }
        else if (c == '=')
        {
            int d = fgetc(fin);
            if (d == '=')
            {
                fillToken(&tkn, c, row, col, "RelationalOperator");
                strcpy(tkn.token_name, "==");
                gotToken = 1;
                col += 2;
            }
        }
        else if (c == '#')
        {
            while ((c = fgetc(fin)) != EOF && c != '\n');
            newLine();
        }
        else if (c == '\n')
        {
            newLine();
        }
        else if (c == '<' || c == '>' || c == '!')
        {
            fillToken(&tkn, c, row, col, "RelationalOperator");
            ++col;
            int d = fgetc(fin);
            if (d == '=')
            {
                ++col;
                strcat(tkn.token_name, "=");
            }
            else
            {
                if (c == '!')
                    strcpy(tkn.type, "LogicalOperator");
                fseek(fin, -1, SEEK_CUR);
            }
            gotToken = 1;
        }
        else if (c == '&' || c == '|')
        {
            int d = fgetc(fin);
            if (c == d)
            {
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
        else if (isdigit(c))
        {
            tkn.row = row;
            tkn.col = col++;
            tkn.token_name[0] = c;
            int k = 1;
            while ((c = fgetc(fin)) != EOF && isdigit(c))
            {
                tkn.token_name[k++] = c;
                col++;
            }
            strcpy(tkn.type, "Number");
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }
        else if (c == '"')
        {
            tkn.row = row;
            tkn.col = col;
            strcpy(tkn.type, "StringLiteral");
            int k = 1;
            tkn.token_name[0] = '"';
            while ((c = fgetc(fin)) != EOF && c != '"')
            {
                tkn.token_name[k++] = c;
                ++col;
            }
            tkn.token_name[k] = '"';
            gotToken = 1;
        }
        else if (isalpha(c) || c == '_')
        {
            tkn.row = row;
            tkn.col = col++;
            tkn.token_name[0] = c;
            int k = 1;
            while ((c = fgetc(fin)) != EOF && isalnum(c))
            {
                tkn.token_name[k++] = c;
                ++col;
            }
            fseek(fin, -1, SEEK_CUR);
            tkn.token_name[k] = '\0';
            if (isKeyword(tkn.token_name)){
                strcpy(tkn.type, "Keyword");
                prevType = "Keyword";
            }
            else{
                strcpy(tkn.type, "Identifier");
                prevType = "Identifier";
                c = fgetc(fin);
                if (c == '('){
                    if (isPredefinedFunc(tkn.token_name) == 0){
                        strcpy(tkn.rtype, prevToken.token_name);
                    }
                    int nargs = 0, cnt = 0;
                    while ((c = fgetc(fin)) != EOF && c != ')')
                    {
                        cnt++;
                        if (nargs == 0 || c == ',')
                            nargs++;
                    }
                    tkn.args = nargs;
                    tkn.id = 2;
                    strcpy(tkn.ttype, "Func");
                    fseek(fin, 0 - (cnt + 2), SEEK_CUR);
                }
                else if (c == ';' || c == '=' || c == ',' || c == ')' || c == '[' || c == ']' || c == '+' || c == '-'){
                    strcpy(tkn.dtype, prevToken.token_name);
                    tkn.id = 1;
                    strcpy(tkn.ttype, "Var");
                    fseek(fin, -1, SEEK_CUR);
                }
                else if (c == ' '){
                    c = fgetc(fin);
                    if (c == '('){
                        strcpy(tkn.rtype, prevToken.token_name);
                        int nargs = 0, cnt = 0;
                        while ((c = fgetc(fin)) != EOF && c != ')')
                        {
                            cnt++;
                            if (nargs == 0 || c == ',')
                                nargs++;
                        }
                        tkn.args = nargs;
                        strcpy(tkn.ttype, "Func");
                        fseek(fin, 0 - (cnt + 3), SEEK_CUR);
                    }
                    else if (c == '=' || c == ',' || c == ')' || c == '[' || c == ']'){
                        strcpy(tkn.dtype, prevToken.token_name);
                        tkn.id = 1;
                        strcpy(tkn.ttype, "Var");
                        fseek(fin, -2, SEEK_CUR);
                    }
                    else{
                        fseek(fin, -2, SEEK_CUR);
                    }
                }
            }
            gotToken = 1;
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

symbol* fillSymbol(struct token tkn)
{
    symbol *s = (symbol *)malloc(sizeof(symbol));
    strcpy(s->token_name, tkn.token_name);
    s->sno = tindex;
    s->next = NULL;
    strcpy(s->ttype, tkn.ttype);
    if (tkn.id == 1){
        strcpy(s->dtype, tkn.dtype);
    }
    else if (tkn.id == 2){
        strcpy(s->rtype, tkn.rtype);
        s->args = tkn.args;
    }
    return s;
}

int insert(struct token tkn)
{
    int key = hash(tkn.token_name[0]);
    symbol *s = fillSymbol(tkn);
    symbol *t = table[key];
    if (t == NULL){
        table[key] = s;
        return 0;
    }
    while (t != NULL){
        if (t->next == NULL){
            t->next = s;
            return 0;
        }
        else{
            t = t->next;
        }
    }
    return -1;
}

int search(char *name)
{
    int key = hash(name[0]);
    symbol* s = table[key];
    if (s == NULL)
        return 0;
    if (strcmp(s->token_name, name) == 0)
        return s->sno;
    else{
        while ((s = s->next) && s != NULL)
            if (strcmp(s->token_name, name) == 0)
                return s->sno;
    }
    return 0;
}

void display()
{
    printf("_____________________________________________\nSNO\tNAME\tTKN\tRTYPE\tDTYPE\tNARGS\n");
    int i;
    symbol *s;
    for(i=0; i<10; i++){
        s = table[i];
        while (s != NULL){
            printf("%d\t%s\t%s\t%s\t%s\t%d\n", s->sno, s->token_name, s->ttype, s->rtype, s->dtype, s->args);
            s = s->next;
        }
    }
}

int main()
{
    int i;
    for(i=0; i<10; i++){
        table[i] = NULL;
    }
    FILE *fin = fopen("input.c", "r");
    FILE *fo = fopen("output.txt", "w+");
    if (!fin)
    {
        printf("Error! File cannot be opened!\n");
        return 0;
    }
    struct token tkn;
    char tokenId[30];
    int searchVal;
    while ((tkn = getNextToken(fin, tkn)).row != -1){
        searchVal = search(tkn.token_name);
        if (searchVal != 0){
            snprintf(tokenId, 30, "id%d", searchVal);
            strcpy(tkn.token_name, tokenId);
        }
        else{
            if (tkn.id != 0){
                tindex++;
                if(insert(tkn) == 0){
                    snprintf(tokenId, 30, "id%d", tindex);
                    strcpy(tkn.token_name, tokenId);
                }
                else{
                    printf("Unable to insert token %s into symbol table", tkn.token_name);
                }
            }
        }
        fprintf(fo, "<%s: %d, %d>\n", tkn.token_name, tkn.row, tkn.col);
        for(i=0; i<gotOpenBracket; i++)
            fprintf(fo, "\t");
    }
    fclose(fin);
    fclose(fo);
    display();
}
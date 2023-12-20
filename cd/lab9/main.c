#include <stdlib.h>
#include "la.c"

FILE *f1, *f2;
Token token;
char *tok;
int decl_flag = 1;
int se_flag = 1;
int sp_flag = 1;
int assgn_flag = 1;
int rel_flag = 1;
int stmt_flag = 1;
int in_block = 0;
int offset = 1;

void expression();
void statement_list();

void invalid(char str[]){
    printf("===========Parse Unsuccessful===============\n");
    printf("Error in function %s\n", str);
    exit(0);
}

void retract(){
    fseek(f2, -strlen(token.token_name), SEEK_CUR);
}

void data_type(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "int") == 0 || strcmp(token.token_name, "char") == 0){
        decl_flag = 1;
        return;
    }
    else{
        retract();
        decl_flag = 0;
    }
}

void id_list();

void listDoublePrime(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, ",") == 0)
        id_list();
    else
        retract();
}

void listPrime(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, ",") == 0)
        id_list();
    else if (strcmp(token.token_name, "[") == 0){
        token = getNextToken(f2);
        if (strcmp(token.type, "Numeric Literal") == 0){
            token = getNextToken(f2);
            if (strcmp(token.token_name, "]") == 0)
                listDoublePrime();
            else
                invalid("listPrime 1");
        }
        else
            invalid("listPrime 2");
    }
    else
        retract();
}

void id_list(){
    if (decl_flag){
        token = getNextToken(f2);
        if (strstr(token.type, "id") != NULL)
            listPrime();
    }
}

void declaration(){
    data_type();
    id_list();
    token = getNextToken(f2);
    if (decl_flag && strcmp(token.token_name, ";") == 0)
        declaration();
    else
        retract();
}

void assignment(){
    token = getNextToken(f2);
    if (strstr(token.type, "id") != NULL){
        assgn_flag = 1;
        if (strcmp(getNextToken(f2).token_name, "=") == 0)
            expression();
        else
            invalid("assignment");
    }
    else{
        retract();
        assgn_flag = 0;
    }
}

void decision_prime(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "else") == 0){
        if (strcmp(getNextToken(f2).token_name, "{") == 0){
            statement_list();
            if (strcmp(getNextToken(f2).token_name, "}") == 0)
                return;
            else
                invalid("decision prime }");
        }
        else
            invalid("decision prime {");
    }
    else
        retract();
}

void decision_stat(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "if") == 0){
        if (strcmp(getNextToken(f2).token_name, "(") == 0){
            expression();
            if (strcmp(getNextToken(f2).token_name, ")") == 0){
                if (strcmp(getNextToken(f2).token_name, "{") == 0){
                    statement_list();
                    if (strcmp(getNextToken(f2).token_name, "}") == 0){
                        printf("decision } parsed\n");
                        decision_prime();
                    }
                    else
                        invalid("decision }");
                }
                else
                    invalid("decision {");
            }
            else
                invalid("decision )");
        }
        else
            invalid("decision (");
    }
}

void looping_stat(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "while") == 0){
        if (strcmp(getNextToken(f2).token_name, "(") == 0){
            expression();
            if (strcmp(getNextToken(f2).token_name, ")") == 0){
                if (strcmp(getNextToken(f2).token_name, "{") == 0){
                    in_block++;
                    statement_list();
                    printf("in while : %s\n", token.token_name);
                    if (strcmp(getNextToken(f2).token_name, "}") == 0)
                        return;
                    else
                        invalid("loop }");
                }
                else
                    invalid("loop {");
            }
            else
                invalid("loop )");
        }
        else
            invalid("loop (");
    }
    else if (strcmp(token.token_name, "for") == 0){
        if (strcmp(getNextToken(f2).token_name, "(") == 0){
            assignment();
            if (strcmp(getNextToken(f2).token_name, ";") == 0){
                expression();
                if (strcmp(getNextToken(f2).token_name, ";") == 0){
                    assignment();
                    if (strcmp(getNextToken(f2).token_name, ")") == 0){
                        if (strcmp(getNextToken(f2).token_name, "{") == 0){
                            statement_list();
                            if (strcmp(getNextToken(f2).token_name, "}") == 0)
                                return;
                            else
                                invalid("loop }");
                        }
                        else
                            invalid("loop {");
                    }
                    else
                        invalid("floop )");
                }
                else
                    invalid("floop ; 2");
            }
            else
                invalid("floop ; 1");
        }
        else
            invalid("floop (");
    }
    else
        invalid("loop");
}

void case_stmt(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "case") == 0){
        token = getNextToken(f2);
        if (strcmp(token.type, "Numeric Literal") == 0){
            token = getNextToken(f2);
            if (strcmp(token.token_name, ":") == 0){
                statement_list();
                token = getNextToken(f2);
                if (strcmp(token.token_name, "break") == 0){
                    token = getNextToken(f2);
                    if (strcmp(token.token_name, ";") == 0){
                        case_stmt();
                        return;
                    }
                }
            }
        }
    }
    else if (strcmp(token.token_name, "default") == 0){
        token = getNextToken(f2);
        if (strcmp(token.token_name, ":") == 0){
            statement_list();
            return;
        }
    }
}

void switch_stmt(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "switch") == 0){
        token = getNextToken(f2);
        if (strcmp(token.token_name, "(") == 0){
            token = getNextToken(f2);
            if (strstr(token.type, "id") != NULL){
                token = getNextToken(f2);
                if (strcmp(token.token_name, ")") == 0){
                    token = getNextToken(f2);
                    if (strcmp(token.token_name, "{") == 0){
                        case_stmt();
                        if (strcmp(getNextToken(f2).token_name, "}") == 0)
                            return;
                    }
                }
            }
        }
    }
}

// single line functions
void statement(){
    token = getNextToken(f2);
    if (strstr(token.type, "id") != NULL){
        stmt_flag = 1;
        retract();
        assignment();
        token = getNextToken(f2);
        if (strcmp(token.token_name, ";") == 0)
            return;
        else
            retract();
    }
    else if (strcmp(token.token_name, "if") == 0){
        stmt_flag = 1;
        retract();
        decision_stat();
    }
    else if (strcmp(token.token_name, "for") == 0 || strcmp(token.token_name, "while") == 0){
        stmt_flag = 1;
        retract();
        looping_stat();
    }
    else if (strcmp(token.token_name, "switch") == 0){
        stmt_flag = 1;
        retract();
        switch_stmt();
    }
    else{
        stmt_flag = 0;
        retract();
    }
}

void statement_list(){
    token = getNextToken(f2);
    if (strstr(token.type, "id") != NULL || strcmp(token.token_name, "if") == 0 || strcmp(token.token_name, "for") == 0 || strcmp(token.token_name, "while") == 0 || strcmp(token.token_name, "switch") == 0){
        retract();
        statement();
        if (stmt_flag)
            statement_list();
    }
    else{
        retract();
        return;
    }
}

void relop(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "==") == 0 || strcmp(token.token_name, "!=") == 0 || strcmp(token.token_name, ">=") == 0 || strcmp(token.token_name, "<=") == 0 || strcmp(token.token_name, ">") == 0 || strcmp(token.token_name, "<") == 0){
        rel_flag = 1;
        return;
    }
    else{
        rel_flag = 0;
        retract();
    }
}

void mulop(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "*") == 0 || strcmp(token.token_name, "/") == 0 || strcmp(token.token_name, "%") == 0){
        se_flag = 1;
        return;
    }
    else{
        retract();
        se_flag = 0;
    }
}

void addop(){
    token = getNextToken(f2);
    if (strcmp(token.token_name, "+") == 0 || strcmp(token.token_name, "-") == 0){
        sp_flag = 1;
        return;
    }
    else{
        retract();
        sp_flag = 0;
    }
}

void factor(){
    token = getNextToken(f2);
    if (strstr(token.type, "id") != NULL)
        return;
    else if (strstr(token.type, "Numeric") != NULL)
        return;
    else
        retract();
}

void term_prime(){
    mulop();
    if (se_flag){
        factor();
        term_prime();
    }
}

void term(){
    factor();
    term_prime();
}

void simple_prime(){
    addop();
    if (sp_flag){
        term();
        simple_prime();
    }
}

void simple_expression(){
    term();
    simple_prime();
}

void expn_prime(){
    relop();
    if (rel_flag)
        simple_expression();
}

void expression(){
    simple_expression();
    expn_prime();
}

void program(){
    token = getNextToken(f2);
    token = getNextToken(f2);
    if (strcmp(token.token_name, "main") == 0){
        token = getNextToken(f2);
        if (strcmp(token.token_name, "(") == 0){
            token = getNextToken(f2);
            if (strcmp(token.token_name, ")") == 0){
                token = getNextToken(f2);
                if (strcmp(token.token_name, "{") == 0){
                    declaration();
                    statement_list();
                    if (strcmp(getNextToken(f2).token_name, "}") == 0)
                        printf("Parse Successful\n");
                    else{
                        invalid("program1");
                        printf("Missing parentheses '}'\n");
                    }
                }
                else{
                    invalid("program2");
                    printf("Missing parentheses '{'\n");
                }
            }
            else{
                invalid("program3");
                printf("Missing parentheses ')'\n");
            }
        }
        else{
            invalid("program4");
            printf("Missing parentheses '('\n");
        }
    }
    else
        invalid("program5");
}

void main(){
    char filename[100];
    // printf("Enter filename to generate tokens : ");
    // gets(filename);
    f1 = fopen("input.c", "r");
    f2 = fopen("output.txt", "w");
    if (f1 == NULL){
        printf("cannot open file");
        exit(1);
    }
    removePreprocessorDirectivesAndComments(f1, f2);
    fclose(f2);
    f2 = fopen("output.txt", "r");
    program();
}
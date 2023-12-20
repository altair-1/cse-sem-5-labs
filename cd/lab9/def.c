#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char token_name[100];
    int row_no;
    int column_no;
    char type[50];
} Token;

typedef struct{
    int index;
    char symbolName[100];
    char type[20];
    char returnType[20];
    char argNum[10];
} Symbol;

typedef struct node{
    Symbol data;
    struct node *next;
} TableElement;
// ==========================================
void displayTable(TableElement **list){
    TableElement *temp = *list;
    while (temp != NULL){
        printf("%d\t%s\t%s\t  %s\t\t\t%s\n", temp->data.index, temp->data.symbolName, temp->data.type, temp->data.returnType, temp->data.argNum);
        temp = temp->next;
    }
}

int search(char str[], TableElement **list){
    TableElement *temp = *list;
    if (temp == NULL){
        return 0;
    }
    while (temp != NULL){
        if (strcmp(temp->data.symbolName, str) == 0)
            return temp->data.index;
        temp = temp->next;
    }
    return 0;
}

TableElement* insert(Symbol symbol, TableElement *list){
    TableElement *newnode = (TableElement *)malloc(sizeof(TableElement));
    newnode->data = symbol;
    newnode->next = NULL;
    if (list == NULL){
        list = newnode; 
        return list;
    }
    TableElement *temp = list;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newnode;
    return list;
}
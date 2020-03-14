#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTSet.h"
#define LINELENGTH 200

char* create_string(char* string) {
    char* newstring;
    newstring = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(newstring, string);
    return newstring;
}

int main(void) {
    Set set = set_create(strcmp, free);
    char linebuf[LINELENGTH];
    SetNode node;
    while (fgets(linebuf, LINELENGTH, stdin) != NULL) {
        set_insert(set, create_string(linebuf));
        node = set_find_node(set, linebuf);
        if ((node = set_next(set, node)) != SET_EOF) {
            printf("%s", (char*)set_node_value(set, node));
        }
        else {
            printf("<none>\n");
        }
    }
    set_destroy(set);
    return 0;
}
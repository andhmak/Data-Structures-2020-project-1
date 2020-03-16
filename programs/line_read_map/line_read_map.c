#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTMap.h"
#define LINELENGTH 200

char* create_string(char* string) {
    char* newstring;
    newstring = malloc(sizeof(char) * (strlen(string) + 1));
    strcpy(newstring, string);
    return newstring;
}

int* create_int(int a) {
    int* p = malloc(sizeof(int));
	*p = a;
	return p;
}

int main(void) {
    Map map = map_create((CompareFunc) strcmp, free, free);
    char linebuf[LINELENGTH];
    int* times;
    while (fgets(linebuf, LINELENGTH, stdin) != NULL) {
        if ((times = (int*)map_find(map, linebuf)) == NULL) {
            printf("0\n");
            map_insert(map, create_string(linebuf), create_int(1));
        }
        else {
            printf("%d\n", *times);
            map_insert(map, create_string(linebuf), create_int(*times + 1));
        }
    }
    map_destroy(map);
    return 0;
}
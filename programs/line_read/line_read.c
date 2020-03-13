#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINELENGTH 200
#define LINEAMOUNT 100000

//use heap array later

struct line {
    char* text;
    int times;
};

int main(void) {
    struct line* lines;
    char linebuf[LINELENGTH];
    int i;
    if ((lines = malloc(LINEAMOUNT * sizeof(struct line))) == NULL) {
        fprintf(stderr, "Sorry, could not allocate memory\n");
    }
    for (i = 0 ; i < LINEAMOUNT ; i++) {
        lines[i].times = 0;
        lines[i].text = NULL;
    }
    while (fgets(linebuf, LINELENGTH, stdin) != NULL) {
        for (i = 0 ; i < LINEAMOUNT ; i++) {
            if (lines[i].text == NULL) {
                lines[i].text = malloc((strlen(linebuf) + 1) * sizeof(char));
                strcpy(lines[i].text, linebuf);
                lines[i].times++;
                printf("0\n");
                break;
            }
            else if (strcmp(linebuf, lines[i].text) == 0) {
                printf("%d\n", lines[i].times);
                lines[i].times++;
                break;
            }
        }
    }
    for (i = 0 ; i < LINEAMOUNT ; i++) {
        if (lines[i].text != NULL) {
            free(lines[i].text);
        }
    }
    free(lines);
    return 0;
}
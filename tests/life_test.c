//////////////////////////////////////////////////////////////////
//
// Test για το life module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ADTList.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "life.h"


void test_create(void) {
    LifeState state = life_create();
    TEST_CHECK(state != NULL);
    LifeCell cell;
    srand(time(NULL));
    for (int i = 0 ; i < 20 ; i++) {
        cell.x = rand();
        cell.y = rand();
        TEST_CHECK(life_get_cell(state, cell) == false);
    }
    life_destroy(state);
}

void test_get_set_cell(void) {
    LifeState state = life_create();
    LifeCell cell;
    srand(time(NULL));
    for (int i = 0 ; i < 20 ; i++) {
        cell.x = rand();
        cell.y = rand();
        life_set_cell(state, cell, true);
        TEST_CHECK(life_get_cell(state, cell) == true);
        life_set_cell(state, cell, false);
        TEST_CHECK(life_get_cell(state, cell) == false);
    }
    life_destroy(state);
}

void test_create_from_rle(void) {
    LifeState state = life_create_from_rle("glider.rle");
    LifeCell cell;
    cell.x = 0;
    cell.y = 0;
    TEST_CHECK(life_get_cell(state, cell) == false);
    cell.x = 1;
    cell.y = 0;
    TEST_CHECK(life_get_cell(state, cell) == true);
    cell.x = 2;
    cell.y = 0;
    TEST_CHECK(life_get_cell(state, cell) == false);
    cell.x = 0;
    cell.y = -1;
    TEST_CHECK(life_get_cell(state, cell) == false);
    cell.x = 1;
    cell.y = -1;
    TEST_CHECK(life_get_cell(state, cell) == false);
    cell.x = 2;
    cell.y = -1;
    TEST_CHECK(life_get_cell(state, cell) == true);
    cell.x = 0;
    cell.y = -2;
    TEST_CHECK(life_get_cell(state, cell) == true);
    cell.x = 1;
    cell.y = -2;
    TEST_CHECK(life_get_cell(state, cell) == true);
    cell.x = 2;
    cell.y = -2;
    TEST_CHECK(life_get_cell(state, cell) == true);
    life_destroy(state);
}

void test_save_to_rle(void) {
    LifeState state = life_create_from_rle("glider.rle");
    life_save_to_rle(state, "glidertest.rle");
    FILE *test = fopen("glidertest.rle", "rb");
    char string[100];
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "bo$2bo$3o!") == 0);
    fclose(test);
    remove("glidertest.rle");
    life_destroy(state);
    state = life_create_from_rle("gun.rle");
    life_save_to_rle(state, "guntest.rle");
    test = fopen("guntest.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "24bo$22bobo$12b2o6b2o12b2o$11bo3bo4b2o12b2o$2o8bo5bo3b2o$2o8bo3bob2o4bobo$10bo5bo7bo$11bo3bo$12b2o!") == 0);
    fclose(test);
    remove("guntest.rle");
    life_destroy(state);
}

void test_evolve(void) {  /* Testing a  full glider cycle */
    LifeState state = life_create_from_rle("glider.rle"), prevstate;
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    FILE *test = fopen("gliderevolved.rle", "rb");
    char string[20];
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "obo$b2o$bo!") == 0);
    fclose(test);
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    test = fopen("gliderevolved.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "2bo$obo$b2o!") == 0);
    fclose(test);
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    test = fopen("gliderevolved.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "o$b2o$2o!") == 0);
    fclose(test);
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    test = fopen("gliderevolved.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "bo$2bo$3o!") == 0);
    fclose(test);
    remove("gliderevolved.rle");
    life_destroy(state);
    ListNode loop = NULL;
    List list = NULL;
    state = life_create_from_rle("glider.rle");
    list = life_evolve_many(state, 8, &loop);
    LifeCell cell;
    for (ListNode node = list_first(list);  ; ) {
        state = list_node_value(list, node);
        printf("\n");
        for (int y = 0 ; y >= -5 ; y--) {
            for (int x = 0 ; x <= 5 ; x++) {
                cell.x = x;
                cell.y = y;
                printf("%d ", life_get_cell(state, cell));
            }
            printf("\n");
        }
        if (list_next(list, node) == LIST_EOF) {
            if (loop) {
                node = loop;
            }
            else {
                break;
            }
        }
        else if (list_next(list, node) != LIST_EOF){
            node = list_next(list, node);
        }
    }
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "life_create", test_create },
    { "life_get_set_cell", test_get_set_cell },
	{ "life_create_from_rle", test_create_from_rle },
    { "life_save_to_rle", test_save_to_rle },
    { "life_evolve", test_evolve },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
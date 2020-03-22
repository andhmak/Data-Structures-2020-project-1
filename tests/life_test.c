//////////////////////////////////////////////////////////////////
//
// Test για το life module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
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
}

void test_save_to_rle(void) {
    LifeState state = life_create_from_rle("glider.rle");
    life_save_to_rle(state, "glidertest.rle");
    state = life_create_from_rle("gun.rle");
    life_save_to_rle(state, "guntest.rle");
}

void test_evolve(void) {
    
}

void test_destroy(void) {
    
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "life_create", test_create },
	{ "life_create_from_rle", test_create_from_rle },
    { "life_save_to_rle", test_save_to_rle },
    { "life_get_set_cell", test_get_set_cell },
    { "life_evolve", test_evolve },
    { "life_destroy", test_destroy },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
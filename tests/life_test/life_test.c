//////////////////////////////////////////////////////////////////
//
// Test για το life module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ADTList.h"
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "life.h"

// Test για την life_create
void test_create(void) {
    // δημιουργία αρχικής κατάστασης
    LifeState state = life_create();
    // έλεγχος για το αν δημιουργήθηκε όντως κάποια κατάσταση
    TEST_CHECK(state != NULL);
    // έλεγχος σε 20 τυχαία κελιά ότι δεν είναι ζωντανά, εφόσον πρέπει να είναι όλα νεκρά
    LifeCell cell;
    srand(time(NULL));
    for (int i = 0 ; i < 20 ; i++) {
        cell.x = rand();
        cell.y = rand();
        TEST_CHECK(life_get_cell(state, cell) == false);
    }
    // απελευθέρωση μνήμης
    life_destroy(state);
}

// Test για τις life_get_cell και life_set_cell
void test_get_set_cell(void) {
    // δημιουργία αρχικής κατάστασης
    LifeState state = life_create();
    // "άναμμα"  20 τυχαίων κελιών και έλεγχος ότι φαίνονται ως
    // ζωντανά, σβήσιμο και έλεγχος ότι φαίνονται ως νεκρά
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
    // απελευθέρωση μνήμης
    life_destroy(state);
}

// Test για την life_create_from_rle
void test_create_from_rle(void) {
    // δημιουργία κατάστασης από γνωστό rle (glider)
    LifeState state = life_create_from_rle("glider.rle");
    // έλεγχος των κελιών της κατάστασης ότι είναι τα αναμενόμενα
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
    // απελευθέρωση μνήμης
    life_destroy(state);
}

// Test για την life_save_to_rle
void test_save_to_rle(void) {
    // Δημιουργία κενής κατάστασης
    LifeState state = life_create();
    // "σώσιμο" κατάστασης σε αρχείο rle
    life_save_to_rle(state, "emptytest.rle");
    // διάβασμα του αρχείου και έλεγχος ότι είναι το αναμενόμενο
    FILE *test = fopen("emptytest.rle", "rb");
    char string[100];
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "!") == 0);
    fclose(test);
    // διαγραφή παραγμένου αρχείου
    remove("emptytest.rle");
    // απελευθέρωση μνήμης κατάστασης
    life_destroy(state);
    // το ίδιο για κατάσταση από γνωστό rle (glider)
    state = life_create_from_rle("glider.rle");
    life_save_to_rle(state, "glidertest.rle");
    test = fopen("glidertest.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "bo$2bo$3o!") == 0);
    fclose(test);
    remove("glidertest.rle");
    life_destroy(state);
    // ξανά αλλά τώρα για gun
    state = life_create_from_rle("gun.rle");
    life_save_to_rle(state, "guntest.rle");
    test = fopen("guntest.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "24bo$22bobo$12b2o6b2o12b2o$11bo3bo4b2o12b2o$2o8bo5bo3b2o$2o8bo3bob2o4bobo$10bo5bo7bo$11bo3bo$12b2o!") == 0);
    fclose(test);
    remove("guntest.rle");
    life_destroy(state);
}

// Test για την life_evolve
void test_evolve(void) {  /* Έλεγχος ενός κύκλου ενός glider */
    // δημιουργία κατάστασης από rle glider
    LifeState state = life_create_from_rle("glider.rle"), prevstate;
    prevstate = state;
    // εξέλιξη
    state = life_evolve(state);
    // απελευθέρωση παλιάς κατάστασης
    life_destroy(prevstate);
    // έλεγχος ότι το αποτέλεσμα είναι το αναμενόμενο
    life_save_to_rle(state, "gliderevolved.rle");
    FILE *test = fopen("gliderevolved.rle", "rb");
    char string[20];
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "obo$b2o$bo!") == 0);
    fclose(test);
    // πάλι για την επόμενη κατάσταση
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    test = fopen("gliderevolved.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "2bo$obo$b2o!") == 0);
    fclose(test);
    // και την επόμενη
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    test = fopen("gliderevolved.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "o$b2o$2o!") == 0);
    fclose(test);
    // μέχρι να κλείσει ένας κύκλος
    prevstate = state;
    state = life_evolve(state);
    life_destroy(prevstate);
    life_save_to_rle(state, "gliderevolved.rle");
    test = fopen("gliderevolved.rle", "rb");
    fscanf(test, "%s", string);
    TEST_CHECK(strcmp(string, "bo$2bo$3o!") == 0);
    fclose(test);
    // διαγραφή παραγμένου αρχείου και απελευθέρωση μνήμης
    remove("gliderevolved.rle");
    life_destroy(state);
}

// Οι test_evolve_many και test_iteration δεν ζητόντουσαν αλλά τις έφτιαξα
// για να με βοηθήσουν. Δεν χρειάζονται βαθμολόγηση αλλά τις αφήνω εδώ
// (με commented τις εντολές που τις καλούν) αφού έτσι κι αλλιώς έχουν υλοποιηθεί
// μήπως βοηθήσουν με κάποιον τρόπο την διαδικασία της βαθμολόγησης 

void test_evolve_many(void) {
    ListNode loop = NULL;
    List list = NULL;
    LifeCell cell;
    LifeState state = life_create_from_rle("glider.rle");
    list = life_evolve_many(state, 7, &loop);
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
    list_destroy(list);
}

void test_iteration(void) {
    LifeCell cell;
    LifeState state = life_create_from_rle("glider.rle");
    for (StateNode node = state_first(state) ; node != STATE_EOF ; node = state_next(state, node)) {
        cell = state_node_cell(state, node);
        printf("\nx: %d, y: %d\n", cell.x, cell.y);
    }
    life_destroy(state);
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "life_create", test_create },
    { "life_get_set_cell", test_get_set_cell },
	{ "life_create_from_rle", test_create_from_rle },
    { "life_save_to_rle", test_save_to_rle },
    { "life_evolve", test_evolve },
    { "life_evolve_many", test_evolve_many },
    { "life_iteration", test_iteration },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
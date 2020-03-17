#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ADTSet.h"

typedef struct {
	int x, y;
} LifeCell;

int compare_cells(LifeCell a, LifeCell b) {
	if (a.x < b.x) {
		return -1;
	}
	else if (a.x > b.x) {
		return 1;
	}
	else {
		if (a.y > b.y) {
			return -1;
		}
		else if (a.y == b.y) {
			return 0;
		}
		else {
			return 1;
		}
	}
}

//typdef struct life_state* LifeState;

struct life_state {
	Set set;
};

// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά.
struct life_state* life_create() {
	struct life_state* state = malloc(sizeof(struct life_state));
	state->set = set_create(compare_cells, free);
	return state;
}

// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format)
struct life_state* life_create_from_rle(char* file) {
	FILE *stream = fopen(file, "rb");
	LifeCell cell;
	struct life_state* state = life_create();
	int x = 0, y = 0, num;
	char ch;
	for (ch = getc(stream) ; ch != '!' ; ch = getc(stream)) {
		if (ch == '#') {
			while (getc(stream) != '\n');
			continue;
		}
		if ((ch >= '0') && (ch <= '9')) {
			for (num = 0 ; (ch >= '0') && (ch <= '9') ; ch = getc(stream)) {
				num *= 10;
				num += ch - 48;
			}
		}
		else {
			num = 1;
		}
		if (ch == 'o') {
			num += x;
			cell.y = y;
			for ( ; x < num ; x++) {
				cell.x = x;
				life_set_cell(state, cell, true);
			}
		}
		else if (ch == 'b') {
			x += num;
		}
		else if (ch == '$') {
			x = 0;
			y--;
		}
		else if (ch == '!') {
			break;
		}
//		else {
//			fpritnf(stderr, "Invalid rle format");
//			exit(1);
//		}
	}
	fclose(stream);
	return state;
}

// Αποθηκεύει την κατάσταση state στο αρχείο file (RLE format)
void life_save_to_rle(struct life_state* state, char* file) {
	FILE *stream = fopen(file, "wb");

}

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό)
bool life_get_cell(struct life_state* state, LifeCell cell) {

}

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state
void life_set_cell(struct life_state* state, LifeCell cell, bool value) {

}

// Παράγει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state
struct life_state* life_evolve(struct life_state* state) {

}

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί
void life_destroy(struct life_state* state) {

}
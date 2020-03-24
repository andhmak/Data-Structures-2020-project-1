#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ADTSet.h"
#include "ADTMap.h"
#include "ADTList.h"

//could hold "top left" cell in state struct, or most left, and the amount of them

typedef struct {
	int x, y;
} LifeCell;

int compare_cells(LifeCell *a, LifeCell *b) {
	if (a->y > b->y) {
		return -1;
	}
	else if (a->y < b->y) {
		return 1;
	}
	else {
		if (a->x < b->x) {
			return -1;
		}
		else if (a->x == b->x) {
			return 0;
		}
		else {
			return 1;
		}
	}
}

LifeCell *create_cell(LifeCell cell) {
	LifeCell *pcell = malloc(sizeof(LifeCell));
	*pcell = cell;
	return pcell;
}

typedef struct life_state* LifeState;

struct life_state {
	Set set;
};

int compare_states(LifeState a, LifeState b) {
	if (set_size(a->set) < set_size(b->set)) {
		return -1;
	}
	else if (set_size(a->set) > set_size(b->set)) {
		return 1;
	}
	else {
		int i;
		for (SetNode node1 = set_first(a->set), node2 = set_first(b->set) ; node1 != SET_EOF ; node1 = set_next(a->set, node1), node2 = set_next(b->set, node1)) {
			if ((i = compare_cells(set_node_value(a->set, node1), set_node_value(b->set, node2))) != 0) {
				return i;
			}
		}
		return 0;
	}
}

// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά.
LifeState life_create() {
	LifeState state = malloc(sizeof(struct life_state));
	state->set = set_create((CompareFunc)compare_cells, free);
	return state;
}

void life_set_cell(LifeState state, LifeCell cell, bool value);

// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format)
LifeState life_create_from_rle(char* file) {
	FILE *stream = fopen(file, "rb");
	LifeCell cell;
	LifeState state = life_create();
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
void life_save_to_rle(LifeState state, char* file) {
	FILE *stream = fopen(file, "wb");
	int leftmost = __INT_MAX__, num = 0;
	SetNode node, prevnode;
	for (node = set_first(state->set) ; node != SET_EOF ; node = set_next(state->set, node)) {
		if (((LifeCell*)set_node_value(state->set, node))->x < leftmost) {
			leftmost = ((LifeCell*)set_node_value(state->set, node))->x;
		}
	}
	if (set_size(state->set)) {
		node = set_first(state->set);
		if (((LifeCell*)set_node_value(state->set, node))->x - leftmost == 1) {
			fprintf(stream, "b");
		}
		else if (((LifeCell*)set_node_value(state->set, node))->x - leftmost) {
			fprintf(stream, "%db", ((LifeCell*)set_node_value(state->set, node))->x - leftmost);
		}
		num++;
	}
	else {
		fprintf(stream, "!");
		return;
	}
	for (prevnode = node, node = set_next(state->set, node) ; node != SET_EOF ; prevnode = node, node = set_next(state->set, node)) {
		if (((LifeCell*)set_node_value(state->set, node))->y - ((LifeCell*)set_node_value(state->set, prevnode))->y) {
			if (num != 1) {
				fprintf(stream, "%do", num);
			}
			else {
				fprintf(stream, "o");
			}
			if (((LifeCell*)set_node_value(state->set, prevnode))->y - ((LifeCell*)set_node_value(state->set, node))->y != 1) {
				fprintf(stream, "%d$", ((LifeCell*)set_node_value(state->set, prevnode))->y - ((LifeCell*)set_node_value(state->set, node))->y);
			}
			else {
				fprintf(stream, "$");
			}
			if (((LifeCell*)set_node_value(state->set, node))->x - leftmost == 1) {
				fprintf(stream, "b");
			}
			else if (((LifeCell*)set_node_value(state->set, node))->x - leftmost) {
				fprintf(stream, "%db", ((LifeCell*)set_node_value(state->set, node))->x - leftmost);
			}
			num = 1;
		}
		else if (((LifeCell*)set_node_value(state->set, node))->x - ((LifeCell*)set_node_value(state->set, prevnode))->x == 1) {
			num++;
		}
		else {
			if (num != 1) {
				fprintf(stream, "%do", num);
			}
			else {
				fprintf(stream, "o");
			}
			num = 1;
			if (((LifeCell*)set_node_value(state->set, node))->x - ((LifeCell*)set_node_value(state->set, prevnode))->x - 1 != 1) {
				fprintf(stream, "%db", ((LifeCell*)set_node_value(state->set, node))->x - ((LifeCell*)set_node_value(state->set, prevnode))->x - 1);
			}
			else {
				fprintf(stream, "b");
			}
		}
	}
	if (num != 1) {
		fprintf(stream, "%do", num);
	}
	else {
		fprintf(stream, "o");
	}
	fprintf(stream, "!");
	fclose(stream);
}

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό)
bool life_get_cell(LifeState state, LifeCell cell) {
	return set_find(state->set, &cell);
}

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state
void life_set_cell(LifeState state, LifeCell cell, bool value) {
	if (value) {
		set_insert(state->set, create_cell(cell));
	}
	else {
		set_remove(state->set, &cell);
	}
}

// Παράγει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state
LifeState life_evolve(LifeState state) {
	LifeState newstate = life_create();
	LifeCell cell, tmpcell;
	int num, i, j, k, l;
	for (SetNode node = set_first(state->set) ; node != SET_EOF ; node = set_next(state->set, node)) {
		for (i = -1 ; i <= 1 ; i++) {
			for (j = -1 ; j <= 1 ; j++) {
				cell = *(LifeCell *)set_node_value(state->set, node);
				cell.x += i;
				cell.y += j;
				num = 0;
				for (k = -1 ; k <= 1 ; k++) {
					for (l = -1 ; l <= 1 ; l++) {
						if ((k == 0) && (l == 0)) {
							continue;
						}
						tmpcell = cell;
						tmpcell.x += k;
						tmpcell.y += l;
						if (life_get_cell(state, tmpcell)) {
							num++;
						}
					}
				}
				if ((num == 3) || ((num == 2) && life_get_cell(state, cell))) {
					life_set_cell(newstate, cell, true);
				}
			}
		}
	}
	return newstate;
}

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί
void life_destroy(LifeState state) {
	set_destroy(state->set);
	free(state);
}

typedef struct {
	ListNode node;
	LifeState state;
} statenode;

int compare_statenodes(statenode *a, statenode *b) {
	return compare_states(a->state, b->state);
}

statenode *create_statenode(statenode a) {
	statenode* p = malloc(sizeof(statenode));
	*p = a;
	return p;
}

// Επιστρέφει μία λίστα από το πολύ steps εξελίξεις, ξεκινώνας από την κατάσταση
// state. Αν βρεθεί επανάληψη τότε στο *loop αποθηκεύεται ο κόμβος στον οποίο
// συνεχίζει η εξέλιξη μετά τον τελευταίο κόμβο της λίστας, διαφορετικά NULL
List life_evolve_many(LifeState state, int steps, ListNode* loop) {
	Map map = map_create((CompareFunc)compare_states, NULL, NULL);
	List list = list_create(free);
	list_insert_next(list, LIST_BOF, state);
	map_insert(map, state, list_last(list));
	for (int i = 0 ; i < steps - 1 ; i++) {
		state = life_evolve(state);
		if ((*loop = (ListNode)map_find(map, state))) {  //<--This always segfaults when it's supposed to find it and it's driving me mad
			life_destroy(state);
			break;
		}
		else {
			list_insert_next(list, list_last(list), state);
			map_insert(map, state, list_last(list));
		}
	}
	map_destroy(map);
	return list;
}
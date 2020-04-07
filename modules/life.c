#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ADTSet.h"
#include "ADTMap.h"
#include "ADTList.h"

//could hold "top left" cell in state struct, or most left, and the amount of them

// Τύπος "κελί"
typedef struct {
	int x, y;
} LifeCell;

// Συγκρίνει κελιά, από το πιο "πάνω" στο πιο "κάτω", και αν είναι
// στην ίδια "σειρά", τότε από το πιο "αριστερό" στο πιο "δεξιό"
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

// Δεσμεύει μνήμη για ένα κελί με τις συντεταγμένες του cell και επιστρέφει έναν δείκτη σε αυτό
LifeCell *create_cell(LifeCell cell) {
	LifeCell *pcell = malloc(sizeof(LifeCell));
	*pcell = cell;
	return pcell;
}

typedef struct life_state* LifeState;

// Τύπος "κατάσταση παιχνιδιού", που υλοποιήται με την χρήση ADTSet
struct life_state {
	Set set;
};

//faster without comparing size
// Συγκρίνει καταστάσεις του παιχνιδιού, σαν δυαδικούς αριθμούς όπου κάθε κελί αναπαριστά ένα ψηφίο.
// Το πρώτο κελί (με την διάταξη compare_cells) που θα βρεθεί διαφορετικό καθορίζει την διάταξη των καταστάσεων.
// Η κατάσταση όπου αυτό το κελί είναι ζωντανό είναι η μεγαλύτερη.
// Αν δεν βρεθεί τέτοιο κελί τότε οι καταστάσεις έιναι ίσες.
int compare_states(LifeState a, LifeState b) {
	int i;
	for (SetNode node1 = set_first(a->set), node2 = set_first(b->set) ; node1 != SET_EOF ; node1 = set_next(a->set, node1), node2 = set_next(b->set, node2)) {
		if ((i = compare_cells(set_node_value(a->set, node1), set_node_value(b->set, node2))) != 0) {
			return i;
		}
	}
	return 0;
}

// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά.
// Δεμεύεται μνήμη για το struct και δημιουργείται το set που περιέχεται σε αυτό.
// Τα κελία στο set συγκρόνται σύμφωνα με την compare_cells και κατασρέφονται με free.
LifeState life_create() {
	LifeState state = malloc(sizeof(struct life_state));
	state->set = set_create((CompareFunc)compare_cells, free);
	return state;
}

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state.
// Αν value == true το προσθέτει στο set (που αν υπάρχει δεν
// αλλάζει κάτι) αλλιώς το αφαιρεί (που αν δεν υπάρχει δεν αλλάζει κάτι).
void life_set_cell(LifeState state, LifeCell cell, bool value) {
	if (value) {
		set_insert(state->set, create_cell(cell));
	}
	else {
		set_remove(state->set, &cell);
	}
}

// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format)
LifeState life_create_from_rle(char* file) {
	// Άνοιγμα του αρχείου
	FILE *stream;
	if ((stream = fopen(file, "rb")) == NULL) {
		fprintf(stderr, "Invalid input file\n");
		exit(-1);
	}
	LifeCell cell;
	// δημιουργία κενής κατάστασης
	LifeState state = life_create();
	// αρχικοοίηση συντεταγμένων στο κέντρο
	// num είναι ένας αριθμός όπως περιέχεται στο rle
	int x = 0, y = 0, num;
	// διάβασμα χαρακτήρα-χαρακτήρα, μέχρι το "!"
	char ch;
	for (ch = getc(stream) ; ch != '!' ; ch = getc(stream)) {
		// αγνόησε τις γραμμές που ξεκονούν με "#"
		if (ch == '#') {
			while (getc(stream) != '\n');
			continue;
		}
		// αν βρεθεί αριθμός, μετατροπή τους από συμβολοσειρά σε int και αποθήκευσή του στο num
		if ((ch >= '0') && (ch <= '9')) {
			for (num = 0 ; (ch >= '0') && (ch <= '9') ; ch = getc(stream)) {
				num *= 10;
				num += ch - 48;
			}
		}
		// αλλιώς εννοείται το 1
		else {
			num = 1;
		}
		// αν βρεθεί "o"
		// προσθήκη num ζωντανών κελιών από την τρέχουσα θέση προς τα δεξιά
		if (ch == 'o') {
			for (num += x, cell.y = y ; x < num ; x++) {
				cell.x = x;
				life_set_cell(state, cell, true);
			}
		}
		// αν βρεθεί "b"
		// κίνηση num θέσεων προς τα δεξιά
		else if (ch == 'b') {
			x += num;
		}
		// αν βρεθεί "$"
		// επιστροφή στην αρχή (0) οριζόντια και κίνηση μια γραμμή κάτω
		else if (ch == '$') {
			x = 0;
			y -= num;
		}
//		else {
//			fpritnf(stderr, "Invalid rle format");
//			exit(1);
//		}
	}
	// κλείσιμο ρεύματος
	fclose(stream);
	return state;
}

// Αποθηκεύει την κατάσταση state στο αρχείο file (RLE format)
void life_save_to_rle(LifeState state, char* file) {
	// Άνοιγμα του αρχείου
	FILE *stream;
	if ((stream = fopen(file, "wb")) == NULL) {
		fprintf(stderr, "Could not create/modify destination file\n");
		exit(-1);
	}
	// Εύρεση της αριστερότερης συντεταγμένης ζωντανού κελιού της δοσμένης κατάστασης
	// (pending θα έιναι ο αριθμός ζωντανών κελιών που είναι εκκρεμότητα να γραφθούν)
	int leftmost = __INT_MAX__, pending = 0;
	SetNode node, prevnode;
	for (node = set_first(state->set) ; node != SET_EOF ; node = set_next(state->set, node)) {
		if (((LifeCell*)set_node_value(state->set, node))->x < leftmost) {
			leftmost = ((LifeCell*)set_node_value(state->set, node))->x;
		}
	}
	// Αν η κατάσταση δεν είναι κενή πάρε τον κόμβο του πρώτου κελιού
	// (τα κελιά είναι κατατεταγμένα από πάνω προς τα κάτω και από δεξια προς τα αριστερά)
	// και γράψε τόσα "b" όσα η απόσταση του κελιού από από το αριστερότερο
	// πρόσθεσε ένα ζωντανό κελί που είναι εκκρεμότητα να γραφθεί
	if (set_size(state->set)) {
		node = set_first(state->set);
		if (((LifeCell*)set_node_value(state->set, node))->x - leftmost == 1) {
			fprintf(stream, "b");
		}
		else if (((LifeCell*)set_node_value(state->set, node))->x - leftmost) {
			fprintf(stream, "%db", ((LifeCell*)set_node_value(state->set, node))->x - leftmost);
		}
		pending++;
	}
	// Αν είναι γράψε στο αρχείο "!" και επίστρεψε
	else {
		fprintf(stream, "!");
		fclose(stream);
		return;
	}
	// Για κάθε άλλο ζωντανό κελί με την σειρά
	for (prevnode = node, node = set_next(state->set, node) ; node != SET_EOF ; prevnode = node, node = set_next(state->set, node)) {
		// Αν αλλάζει γραμμή (συντεταγμένη y) από το προηγούμενο
		if (((LifeCell*)set_node_value(state->set, node))->y - ((LifeCell*)set_node_value(state->set, prevnode))->y) {
			// γράψε όσα ζωντανά κελιά ("o") εκκρεμούν στην γραμμή
			if (pending != 1) {
				fprintf(stream, "%do", pending);
			}
			else {
				fprintf(stream, "o");
			}
			// γράψε όσες αλλαγές γραμμής ("$") χρειάζονται
			if (((LifeCell*)set_node_value(state->set, prevnode))->y - ((LifeCell*)set_node_value(state->set, node))->y != 1) {
				fprintf(stream, "%d$", ((LifeCell*)set_node_value(state->set, prevnode))->y - ((LifeCell*)set_node_value(state->set, node))->y);
			}
			else {
				fprintf(stream, "$");
			}
			// γράψε τα νεκρά κελιά ("b") στην αρχή της νέας γραμμής (απόσταση του πρώτου ζωντανού της γραμμής (του τωρινού) από το αριστερότερο)
			if (((LifeCell*)set_node_value(state->set, node))->x - leftmost == 1) {
				fprintf(stream, "b");
			}
			else if (((LifeCell*)set_node_value(state->set, node))->x - leftmost) {
				fprintf(stream, "%db", ((LifeCell*)set_node_value(state->set, node))->x - leftmost);
			}
			// θέσε το πλήθος των ζωντανά κελιά που είναι εκκρεμότητα να γραφθούν σε 1
			pending = 1;
		}
		// Αλλιώς αν είναι ακριβώς δίπλα
		else if (((LifeCell*)set_node_value(state->set, node))->x - ((LifeCell*)set_node_value(state->set, prevnode))->x == 1) {
			// πρόσθεσε ένα ζωντανό κελί που είναι εκκρεμότητα να γραφθεί
			pending++;
		}
		// Αλλιώς
		else {
			// γράψε όσα ζωντανά κελιά εκκρεμούν
			if (pending != 1) {
				fprintf(stream, "%do", pending);
			}
			else {
				fprintf(stream, "o");
			}
			// θέσε το πλήθος των ζωντανά κελιά που είναι εκκρεμότητα να γραφθούν σε 1
			pending = 1;
			// γράψε τα νεκρά κελιά ανάμεσα σε αυτό το ζωντανό κελί και το προηγούμενο
			if (((LifeCell*)set_node_value(state->set, node))->x - ((LifeCell*)set_node_value(state->set, prevnode))->x - 1 != 1) {
				fprintf(stream, "%db", ((LifeCell*)set_node_value(state->set, node))->x - ((LifeCell*)set_node_value(state->set, prevnode))->x - 1);
			}
			else {
				fprintf(stream, "b");
			}
		}
	}
	// γράψε όσα ζωντανά κελιά εκκρεμούν
	if (pending != 1) {
		fprintf(stream, "%do", pending);
	}
	else {
		fprintf(stream, "o");
	}
	// γράψε "!"
	fprintf(stream, "!");
	// κλείσιμο του ρεύματος
	fclose(stream);
}

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό)
bool life_get_cell(LifeState state, LifeCell cell) {
	return set_find(state->set, &cell);
}

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί
void life_destroy(LifeState state) {
	set_destroy(state->set);
	free(state);
}

// Παράγει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state
LifeState life_evolve(LifeState state) {
	// η εξελιγμένη κατάσταση
	LifeState newstate = life_create();
	// κατάσταση που περιέχει τα σίγουρα νεκρά κελιά της εξελιγμένης κατάστασης
	LifeState deadcells = life_create();
	LifeCell cell, tmpcell;
	int alive, i, j, k, l;
	// για κάθε ζωντανό κελί της παλιάς κατάστασης
	for (SetNode node = set_first(state->set) ; node != SET_EOF ; node = set_next(state->set, node)) {
		// ελέγχεται αυτό και όλα τα γειτονικά του
		for (i = -1 ; i <= 1 ; i++) {
			for (j = -1 ; j <= 1 ; j++) {
				cell = *(LifeCell *)set_node_value(state->set, node);
				cell.x += i;
				cell.y += j;
				// αν δεν έχει τεθεί ως ζωντανό ή νεκρό στην εξελιγμένη κατάσταση
				if ((!life_get_cell(newstate, cell)) && (!life_get_cell(deadcells, cell))) {
					// μετρώνται οι ζωνανοί γείτονες
					alive = 0;
					for (k = -1 ; k <= 1 ; k++) {
						for (l = -1 ; l <= 1 ; l++) {
							if ((k == 0) && (l == 0)) {
								continue;
							}
							tmpcell = cell;
							tmpcell.x += k;
							tmpcell.y += l;
							if (life_get_cell(state, tmpcell)) {
								alive++;
							}
						}
					}
					// αν είναι 3 ή είναι 2 και το κελί είναι ζωντανό
					// θα είναι ζωντανό στην εξελιγμένη κατάσταση
					if ((alive == 3) || ((alive == 2) && life_get_cell(state, cell))) {
						life_set_cell(newstate, cell, true);
					}
					// αλλιώς θα είναι νεκρό
					else {
						life_set_cell(deadcells, cell, true);
					}
				}
			}
		}
	}
	// απελευθέρωση μνήμης της βοηθητικής κατάστασης των νεκρών κελιών
	life_destroy(deadcells);
	// επιστροφή της εξελιγμένης κατάστασης
	return newstate;
}

//typedef struct {
//	ListNode node;
//	LifeState state;
//} statenode;

//int compare_statenodes(statenode *a, statenode *b) {
//	return compare_states(a->state, b->state);
//}

//statenode *create_statenode(statenode a) {
//	statenode* p = malloc(sizeof(statenode));
//	*p = a;
//	return p;
//}

// Επιστρέφει μία λίστα από το πολύ steps εξελίξεις, ξεκινώνας από την κατάσταση
// state. Αν βρεθεί επανάληψη τότε στο *loop αποθηκεύεται ο κόμβος στον οποίο
// συνεχίζει η εξέλιξη μετά τον τελευταίο κόμβο της λίστας, διαφορετικά NULL
List life_evolve_many(LifeState state, int steps, ListNode* loop) {
	// δημιουργία map με κλειδιά καταστάσεις, που θα κατατάσσονται 
	// σύμφωνα με την compare_states, και τιμές κομβους λίστας
	Map map = map_create((CompareFunc)compare_states, NULL, NULL);
	// δημιουργία λίστας με τιμές καταστάσεις και DestroyFunc life_destroy
	List list = list_create((DestroyFunc)life_destroy);
	// εισαγωγή της δοσμένης ατάστασης ως πρώτου στοιχείου λίστας
	list_insert_next(list, LIST_BOF, state);
	//εισαγωγή της πρώτης κατάστασης και του πρώτου στοιχείου της λίστας στο map
	map_insert(map, state, list_last(list));
	// μέχρι να έχουν γίνει steps εξελίξεις συνολικά
	for (int i = 0 ; i < steps - 1 ; i++) {
		// εξελίσσεται η κατάσταη
		state = life_evolve(state);
		// αν αυτή η κατάσταση υπήρξε πριν, τίθεται το *loop ίσο με τον κόμβο
		// της λίστας που περιέχει αυσή η κατάσταση και λήγει η επανάληψη
		if ((*loop = (ListNode)map_find(map, state))) {
			life_destroy(state);
			break;
		}
		// αλλιώς μπαίνει η κατάσταση στην λίστα και η κατάσταση μαζί με τον κόμβο της λίστας στο map
		else {
			list_insert_next(list, list_last(list), state);
			map_insert(map, state, list_last(list));
		}
	}
	// απελευθέρωση του map
	map_destroy(map);
	// επιστροφή της λίστας
	return list;
}

//struct set {
//	SetNode root;				// η ρίζα, NULL αν είναι κενό δέντρο
//	int size;					// μέγεθος, ώστε η set_size να είναι Ο(1)
//	CompareFunc compare;		// η διάταξη
//	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του set
//};

//maybe if it doesn't allow that then pointer to SetNode

// Τα παρακάτω επιτρέπουν επαναληπτικές μεθόδους, διασχίζοντας τα ζωντανά κελιά μιας κατάστασης

typedef SetNode StateNode;

#define STATE_BOF (StateNode)0
#define STATE_EOF (StateNode)0

// Επιστρέφουν το πρώτο και το τελευταίο cell του state, ή STATE_BOF / STATE_EOF αντίστοιχα αν το state είναι κενό

StateNode state_first(LifeState state) {
	return set_first(state->set);
}

StateNode state_last(LifeState state) {
	return set_last(state->set);
}

// Επιστρέφουν τον επόμενο και τον προηγούμενο κομβο του node, ή SET_EOF / SET_BOF
// αντίστοιχα αν ο node δεν έχει επόμενο / προηγούμενο.

StateNode state_next(LifeState state, StateNode node) {
	return set_next(state->set, node);
}

StateNode state_previous(LifeState state, StateNode node) {
	return set_previous(state->set, node);
}

// Επιστρέφει το περιεχόμενο του κόμβου node
LifeCell state_node_cell(LifeState state, StateNode node) {
	return *(LifeCell*)set_node_value(state->set, node);
}
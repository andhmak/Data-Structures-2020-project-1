#include <stdbool.h>
#include "ADTSet.h"
#include "ADTList.h"

typedef struct {
	int x, y;
} LifeCell;

typedef struct life_state* LifeState;

// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά.
LifeState life_create();

// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format)
LifeState life_create_from_rle(char* file);

// Αποθηκεύει την κατάσταση state στο αρχείο file (RLE format)
void life_save_to_rle(LifeState state, char* file);

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό)
bool life_get_cell(LifeState state, LifeCell cell);

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state
void life_set_cell(LifeState state, LifeCell cell, bool value);

// Παράγει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state
LifeState life_evolve(LifeState state);

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί
void life_destroy(LifeState state);

// Επιστρέφει μία λίστα από το πολύ steps εξελίξεις, ξεκινώνας από την κατάσταση
// state. Αν βρεθεί επανάληψη τότε στο *loop αποθηκεύεται ο κόμβος στον οποίο
// συνεχίζει η εξέλιξη μετά τον τελευταίο κόμβο της λίστας, διαφορετικά NULL
List life_evolve_many(LifeState state, int steps, ListNode* loop);

typedef SetNode StateNode;

#define STATE_BOF (StateNode)0
#define STATE_EOF (StateNode)0

// Επιστρέφουν το πρώτο και το τελευταίο cell του state, ή STATE_BOF / STATE_EOF αντίστοιχα αν το set είναι κενό

StateNode state_first(LifeState state);
StateNode state_last(LifeState state);

// Επιστρέφουν τον επόμενο και τον προηγούμενο κομβο του node, ή SET_EOF / SET_BOF
// αντίστοιχα αν ο node δεν έχει επόμενο / προηγούμενο.

StateNode state_next(LifeState state, StateNode node);
StateNode state_previous(LifeState state, StateNode node);

// Επιστρέφει το περιεχόμενο του κόμβου node

LifeCell state_node_cell(LifeState state, StateNode node);
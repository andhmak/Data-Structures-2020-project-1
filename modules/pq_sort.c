////////////////////////////////
//
// Υλοποίηση του pq_sort module
//
////////////////////////////////

#include <stdlib.h>

#include "pq_sort.h"
#include "ADTPriorityQueue.h"


void pq_sort_vector(Vector vec, CompareFunc compare) {
	// Προσοχή! Μέσα στη συνάρτηση αυτή θα χρειαστεί να αντικαταστήσουμε τα περιεχόμενα του vector. Δε
	// θέλουμε όμως όσο το κάνουμε αυτό το vector να καλέσει τη destroy! Οπότε αλλάζουμε προσωρινά τη
	// συνάρτηση destroy σε NULL (αποθηκεύοντας την παλιά τιμή).
	DestroyFunc old_destroy = vector_set_destroy_value(vec, NULL);


	// κυρίως λειτουργία της συνάρτησης
	// ...

	// επαναφορά της destroy
	vector_set_destroy_value(vec, old_destroy);
}

void pq_sort_list(List list, CompareFunc compare) {

	// ...

}
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
	PriorityQueue pqueue = pqueue_create(compare, NULL, vec);
	for (int i = vector_size(vec) - 1 ; i >= 0 ; i--) {
		vector_set_at(vec, i, pqueue_max(pqueue));
		pqueue_remove_max(pqueue);
	}
	pqueue_destroy(pqueue);
	// επαναφορά της destroy
	vector_set_destroy_value(vec, old_destroy);
}

void pq_sort_list(List list, CompareFunc compare) {
	DestroyFunc old_destroy = list_set_destroy_value(list, NULL);

	PriorityQueue pqueue = pqueue_create(compare, NULL, NULL);
	for (ListNode node = list_first(list) ; node != LIST_EOF ; node = list_next(list, node)) {
		pqueue_insert(pqueue, list_node_value(list, node));
	}
	list_destroy(list);
	list = list_create(old_destroy);
	while (pqueue_size(pqueue) > 0) {
		list_insert_next(list, LIST_BOF, pqueue_max(pqueue));
		pqueue_remove_max(pqueue);
	}
	pqueue_destroy(pqueue);

}
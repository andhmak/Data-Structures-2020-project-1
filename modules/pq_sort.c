////////////////////////////////
//
// Υλοποίηση του pq_sort module
//
////////////////////////////////

#include <stdlib.h>
#include "pq_sort.h"
#include "ADTPriorityQueue.h"

void pq_sort_vector(Vector vec, CompareFunc compare) {
	// Μέσα στη συνάρτηση αυτή θα χρειαστεί να αντικαταστήσουμε τα περιεχόμενα του vector. Δε θέλουμε
	// όμως όσο το κάνουμε αυτό το vector να καλέσει τη destroy! Οπότε αλλάζουμε προσωρινά τη
	// συνάρτηση destroy σε NULL (αποθηκεύοντας την παλιά τιμή).
	DestroyFunc old_destroy = vector_set_destroy_value(vec, NULL);

	// δημιουργία pqueue με στοιχεία από το vector
	PriorityQueue pqueue = pqueue_create(compare, NULL, vec);

	// Από το τέλος προς την αρχή του vector αντικαθίσταται κάθε τιμή του με το μέγιστο της pqueue,
	// και αφαιρείται από την pqueue. Άρα κατατάσσονται με αύξουσα σειρά. 
	for (int i = vector_size(vec) - 1 ; i >= 0 ; i--) {
		vector_set_at(vec, i, pqueue_max(pqueue));
		pqueue_remove_max(pqueue);
	}

	// καταστροφή pqueue
	pqueue_destroy(pqueue);

	// επαναφορά της destroy
	vector_set_destroy_value(vec, old_destroy);
}

void pq_sort_list(List list, CompareFunc compare) {
	// Αλλαγή της destroy σε NULL ώστε να μην ελευθερωθούν τα δεδομένα και αποθήκευση της παλιάς destroy
	DestroyFunc old_destroy = list_set_destroy_value(list, NULL);

	// δημιουργία pqueue με στοιχεία από το vector
	PriorityQueue pqueue = pqueue_create(compare, NULL, NULL);
	for (ListNode node = list_first(list) ; node != LIST_EOF ; node = list_next(list, node)) {
		pqueue_insert(pqueue, list_node_value(list, node));
	}

	// άδειασμα της λίστας
	while(list_size(list)) {
		list_remove_next(list, LIST_BOF);
	}

	// Γέμισμα της λίστας με τα στοιχεία της pqueue από το μεγαλύτερο προς το μικρότερο, βάζοντας
	// το καινούργιο πάντα στην αρχή, άρα τελικά η λίστα έχει τα στοιχεία της είναι κατατεταγμένα με αύξουσα σειρά.
	while (pqueue_size(pqueue) > 0) {
		list_insert_next(list, LIST_BOF, pqueue_max(pqueue));
		pqueue_remove_max(pqueue);
	}

	// καταστροφή pqueue
	pqueue_destroy(pqueue);

	// επαναφορά της destroy
	list_set_destroy_value(list, old_destroy);
}
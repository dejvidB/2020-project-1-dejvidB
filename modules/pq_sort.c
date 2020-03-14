////////////////////////////////
//
// Υλοποίηση του pq_sort module
//
////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "pq_sort.h"
#include "ADTPriorityQueue.h"

int *create_ints(int value){
	int *pointer = malloc(sizeof(int)); // δέσμευση μνήμης
	*pointer = value;					// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void pq_sort_vector(Vector vec, CompareFunc compare){
	// Προσοχή! Μέσα στη συνάρτηση αυτή θα χρειαστεί να αντικαταστήσουμε τα περιεχόμενα του vector. Δε
	// θέλουμε όμως όσο το κάνουμε αυτό το vector να καλέσει τη destroy! Οπότε αλλάζουμε προσωρινά τη
	// συνάρτηση destroy σε NULL (αποθηκεύοντας την παλιά τιμή).
	DestroyFunc old_destroy = vector_set_destroy_value(vec, NULL);

	// κυρίως λειτουργία της συνάρτησης
	PriorityQueue pq = pqueue_create(compare, free, vec);
	for (int i = vector_size(vec) - 1; i >= 0; i--){
		vector_set_at(vec, i, create_ints(*((int *)pqueue_max(pq))));
		pqueue_remove_max(pq);
	}
	// επαναφορά της destroy
	pqueue_destroy(pq);
	vector_set_destroy_value(vec, old_destroy);
}

void pq_sort_list(List list, CompareFunc compare){
	DestroyFunc old_destroy = list_set_destroy_value(list, NULL);
	// κυρίως λειτουργία της συνάρτησης
	PriorityQueue pq = pqueue_create(compare, free, NULL);
	for(ListNode node = list_first(list); node != LIST_EOF;){
		int *current = create_ints(*(int*)list_node_value(list, node));
		pqueue_insert(pq, current);
	}
	
	list_destroy(list);
	list = list_create(old_destroy);

	while(pqueue_size(pq) >= 0){
		list_insert_next(list, LIST_BOF, *((int*)create_ints(*((int *)pqueue_max(pq)))));
		pqueue_remove_max(pq);
	}
	pqueue_destroy(pq);
}

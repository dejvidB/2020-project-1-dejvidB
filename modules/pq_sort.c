////////////////////////////////
//
// Υλοποίηση του pq_sort module
//
////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "pq_sort.h"
#include "ADTPriorityQueue.h"

int* create_ints(int value){
	int* pointer = malloc(sizeof(int)); // δέσμευση μνήμης
	*pointer = value;					// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void pq_sort_vector(Vector vec, CompareFunc compare){
	// Προσοχή! Μέσα στη συνάρτηση αυτή θα χρειαστεί να αντικαταστήσουμε τα περιεχόμενα του vector. Δε
	// θέλουμε όμως όσο το κάνουμε αυτό το vector να καλέσει τη destroy! Οπότε αλλάζουμε προσωρινά τη
	// συνάρτηση destroy σε NULL (αποθηκεύοντας την παλιά τιμή).
	DestroyFunc old_destroy = vector_set_destroy_value(vec, NULL);

	// κυρίως λειτουργία της συνάρτησης
	PriorityQueue pq = pqueue_create(compare, free, vec);	  //Δημιουργία pq με βάση τον vec και συνάρτηση compare
	for (int i = vector_size(vec) - 1; i >= 0; i--){	   	  //Αφαίρεση κάθε του μεγαλύτερου στοιχείου του pq, και προσθήκη του στο vec
		vector_set_at(vec, i, create_ints(*((int *)pqueue_max(pq))));	//Προσθήκη των στοιχείων με την αντίστροφη σειρά
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
	for(ListNode node = list_first(list); node != LIST_EOF; node = list_next(list, node)){	//Αντιγραφή των στοιχείων της λίστας στο pq
		int *current = create_ints(*(int*)list_node_value(list, node));
		pqueue_insert(pq, current);
	}
	list_destroy(list);  //"Καταστροφή" της λίστας
	list = list_create(old_destroy);

	while(pqueue_size(pq) > 0){
		list_insert_next(list, LIST_BOF, create_ints(*((int *)pqueue_max(pq))));	//Δημιουργία νέα λίστας, προσθέτοντας το μεγαλύτερο στοιχείο στην αρχή της
		pqueue_remove_max(pq);
	}
	pqueue_destroy(pq);
}
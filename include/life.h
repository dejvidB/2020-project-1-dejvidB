#include "ADTMap.h"
#include "ADTSet.h"
#include "ADTList.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

typedef struct {
	int x, y;
} LifeCell;

typedef Map LifeState;

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

// Μετατρέπει την κατάσταση state σε μορφή RLE και την επιστρέφει ως String
char* RLE_to_String(LifeState state);

// Επιστρέφει μία λίστα από το πολύ steps εξελίξεις, ξεκινώνας από την κατάσταση
// state. Αν βρεθεί επανάληψη τότε στο *loop αποθηκεύεται ο κόμβος στον οποίο
// συνεχίζει η εξέλιξη μετά τον τελευταίο κόμβο της λίστας, διαφορετικά NULL
List life_evolve_many(LifeState state, int steps, ListNode* loop);

// Επιστρέφει μία λίστα από το πολύ steps εξελίξεις, ξεκινώνας από την κατάσταση
// state. Αν βρεθεί _μετατόπιση ή επανάληψη _ τότε στο *loop αποθηκεύεται ο κόμβος
// στον οποίο συνεχίζει η εξέλιξη μετά τον τελευταίο κόμβο της λίστας, διαφορετικά NULL
// και στις μεταβλητές displacement_x, displacement_y αποθηκεύεται η μετατόπιση των x, y
List life_evolve_many_with_displacement(LifeState state, int steps, ListNode* loop);
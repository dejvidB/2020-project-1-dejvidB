//////////////////////////////////////////////////////////////////
//
// Test για το life.c module
//
//////////////////////////////////////////////////////////////////

#include <limits.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "life.h"

void test_life_create(void) {

    LifeState state = NULL;
    state = life_create();

    TEST_CHECK(state != NULL);
}

void test_life_create_from_rle(void){
    LifeState state = NULL;
    state = life_create_from_rle("test.RLE");

    TEST_CHECK(state != NULL);
}

void test_life_evolve_and_write(void){
    LifeState state = life_create_from_rle("test.RLE");
    ListNode* rec = NULL;
    List states = life_evolve_many(state, 500, rec);
    printf("IS LISTNODE NULL? %d\n", rec == NULL);
    state = list_node_value(states, list_last(states));
    printf("%s\n", RLE_to_String(state));
    //life_save_to_rle(state, "result.RLE");
    //life_destroy(state);
    TEST_CHECK(state != NULL);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	//{ "test_life_create", test_life_create },
	//{ "test_life_create_from_rle", test_life_create_from_rle},
	{ "test_life_evovle_and_write", test_life_evolve_and_write},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};

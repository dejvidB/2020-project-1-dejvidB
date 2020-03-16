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
    state = life_evolve(life_evolve(state));
    life_save_to_rle(state, "result.RLE");
    TEST_CHECK(state != NULL);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	//{ "test_life_create", test_life_create },
	//{ "test_life_create_from_rle", test_life_create_from_rle},
	{ "test_life_evovle_and_write", test_life_evolve_and_write},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};

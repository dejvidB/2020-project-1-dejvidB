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


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_life_create", test_life_create },
	{ "test_life_create_from_rle", test_life_create_from_rle},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};

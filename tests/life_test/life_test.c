//////////////////////////////////////////////////////////////////
//
// Test για το life.c module
//
//////////////////////////////////////////////////////////////////
#include <limits.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "life.h"

void test_life_create(void){
    LifeState state = NULL;
    state = life_create();
    TEST_CHECK(state != NULL);
    life_destroy(state);
}

void test_life_evolve(void){
    //Create test file with a glider
    FILE *fp = fopen("live_evolve_test_file.RLE", "w");
    fprintf(fp, "%s", "bo$2bo$3o!");
    fclose(fp);

    LifeState state = life_create_from_rle("live_evolve_test_file.RLE");
    TEST_CHECK(state != NULL);

    state = life_evolve(state);

    //Check if life evolved to the next state successfully
    char* evolved_rle = RLE_to_String(state);
    TEST_CHECK(strcmp(evolved_rle, "obo$b2o$bob!") == 0);
    free(evolved_rle);
    life_destroy(state);

    remove("live_evolve_test_file.RLE");
}

void test_life_get_cell(void){
    LifeCell cell = {INT_MIN, 0};
    LifeCell cell1 = {0, INT_MAX};
    LifeCell cell2 = {5, 6};
    LifeCell cell3 = {15246, 5515};
    LifeCell dead = {0, 0};

    LifeState test_state = life_create();

    life_set_cell(test_state, cell, true);
    life_set_cell(test_state, cell1, true);
    life_set_cell(test_state, cell2, true);
    life_set_cell(test_state, cell3, true);
    life_set_cell(test_state, dead, false);

    TEST_CHECK(life_get_cell(test_state, cell));
    TEST_CHECK(life_get_cell(test_state, cell1));
    TEST_CHECK(life_get_cell(test_state, cell2));
    TEST_CHECK(life_get_cell(test_state, cell3));
    TEST_CHECK(life_get_cell(test_state, dead) == 0);

    life_destroy(test_state);
}

void test_life_evolve_and_write(void){
    LifeState state = life_create_from_rle("test.RLE");
    ListNode loop = NULL;
    List states = life_evolve_many_with_displacement(state, 500, &loop);
    //printf("List size is: %d\n", list_size(states));
    //printf("IS LISTNODE NULL? %p\n", rec);
    state = list_node_value(states, list_last(states));
    //printf("%s\n", RLE_to_String(state));
    //life_save_to_rle(state, "result.RLE");
    //life_destroy(state);
    TEST_CHECK(state != NULL);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_life_create", test_life_create},
    { "test_life_evolve", test_life_evolve},
	{ "test_life_get_cell", test_life_get_cell},
	{ "test_life_evovle_and_write", test_life_evolve_and_write},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};

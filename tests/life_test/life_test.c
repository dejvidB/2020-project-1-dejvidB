////////////////////////////////////////////////////////////////// //
// Test για το life.c module //
//////////////////////////////////////////////////////////////////
#include <limits.h>
#include "acutest.h"

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

    LifeState state1 = life_evolve(state);
    //Check if life evolved successfully to the next state
    char* evolved_rle = RLE_to_String(state1);
    TEST_CHECK(strcmp(evolved_rle, "obo$b2o$bob") == 0);
    free(evolved_rle);
    life_destroy(state);
    life_destroy(state1);

    //Remove test file
    remove("live_evolve_test_file.RLE");
}

void test_life_get_set_cell(void){
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

    life_set_cell(test_state, cell, false);
    life_set_cell(test_state, cell1, false);
    life_set_cell(test_state, cell2, false);
    life_set_cell(test_state, cell3, false);


    TEST_CHECK(!life_get_cell(test_state, cell));
    TEST_CHECK(!life_get_cell(test_state, cell1));
    TEST_CHECK(!life_get_cell(test_state, cell2));
    TEST_CHECK(!life_get_cell(test_state, cell3));

    life_destroy(test_state);
}

void test_life_evolve_many(void){
    FILE* fp = fopen("live_evolve_test_file.RLE", "w");
    fprintf(fp, "%s", "4b2o6b2o4b$3bobo6bobo3b$3bo10bo3b$2obo10bob2o$2obobo2b2o2bobob2o$3bobobo2bobobo3b$3bobobo2bobobo3b$2obobo2b2o2bobob2o$2obo10bob2o$3bo10bo3b$3bobo6bobo3b$4b2o6b2o!");
    fclose(fp);
    ListNode loop = NULL;
    List states = life_evolve_many(life_create_from_rle("live_evolve_test_file.RLE"), 5000, &loop);
    TEST_CHECK(loop != NULL);
    TEST_CHECK(list_size(states) == 5);
    list_destroy(states);

    //Remove test file
    remove("live_evolve_test_file.RLE");
}

void test_life_evolve_many_with_displacement(void){
    //Create test file with a glider
    FILE *fp = fopen("live_evolve_test_file.RLE", "w");
    fprintf(fp, "%s", "bo$2bo$3o!");
    fclose(fp);

    ListNode loop = NULL;
    List states = life_evolve_many_with_displacement(life_create_from_rle("live_evolve_test_file.RLE"), 5000, &loop);

    TEST_CHECK(loop != NULL);
    TEST_CHECK(list_size(states) == 4);
    list_destroy(states);

    //Remove test file
    remove("live_evolve_test_file.RLE");
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
    {"life_create() and life_destroy()", test_life_create},
    {"life_evolve()", test_life_evolve},
    {"life_set_cell() and life_get_cell()", test_life_get_set_cell},
    {"life_evolve_many()", test_life_evolve_many},
    {"life_evolve_many_with_displacement()", test_life_evolve_many_with_displacement},
    { NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
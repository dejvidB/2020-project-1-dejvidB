#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
int x = 0, y = 0, min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;

int compare(int *a, int *b){
    return *(int*)a - *(int*)b;
}

int set_compare(LifeCell *a, LifeCell *b){
    return a->y - b->y;
}

int *create_int(int value){
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = value;
    return p;
}

LifeState life_create(){
    return map_create((CompareFunc)compare, NULL, NULL);
}

LifeState life_create_from_rle(char* file){
    LifeState state = life_create();
    FILE *fp = fopen(file, "rb");
    assert(fp != NULL);
    char c;
    int times = 0;
    while((c = fgetc(fp)) != '!'){
        if(c == '#' || c == 'x'){
            while(c != '\n')                        //Skip unnecessary lines by fgetting till \n
                c = fgetc(fp);
        }else if(c >= '0' && c <= '9'){             //Character is a number if its ASCII code lies between '0'-'9'
            times *= 10;
            times += (c - '0');
        }else if(c == 'o'){
            if(times == 0)
                times = 1;
            while(times--){
                LifeCell cell = {x, y};
                life_set_cell(state, cell, true);   //IMPORTANT: SAVE ONLY LIVING CELLS
                y++;                                //Increase y coordinate for future use
            }
            times = 0;                              //Reset times
        }else if(c == 'b'){
            if(times == 0)
                times = 1;
            while(times--)
                y++;                                //Skip dead cells by increasing y
            times = 0;                              //Reset times
        }else if(c == '$'){
            if(times == 0)
                times = 1;
            while(times--)
                x++;                                //Increase x, cause line changed ($)
            y = 0;
            times = 0;
        }
    }
    fclose(fp);
    return state;
}

void life_save_to_rle(LifeState state, char* file){
    min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;

    //Find state limits
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
        Set line = map_node_value(state, map_node);
        for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
            LifeCell cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
            if(cell.x < min_x)
                min_x = cell.x;
            if(cell.x > max_x)
                max_x = cell.x;
            if(cell.y < min_y)
                min_y = cell.y;
            if(cell.y > max_y)
                max_y = cell.y;
        }
    }

    FILE *fp = fopen(file, "w");
    assert(fp != NULL);
    for(int x = min_x; x <= max_x; x++){
        int times = 0;
        char last = 0, newc = 0;                    //Keep track of old and new charater
        for(int y = min_y; y <= max_y; y++){
            LifeCell cell;
            cell.x = x;
            cell.y = y;
            if(life_get_cell(state, cell)){
                newc  = 'o';
            }else{
                newc = 'b';
            }
            if(last == 0){
                last = newc;
                times = 1;
            }else if(last == newc){                 //Count consecutive alive or dead cells
                times++;
            }else{
                if(times > 1)
                    fprintf(fp, "%d", times);
                fprintf(fp, "%c", last);
                last = newc;
                times = 1;
            }
        }

        if(times > 1)
            fprintf(fp, "%d", times);
        fprintf(fp, "%c", last);
        if(x != max_x) fprintf(fp, "%c", '$');      //Change line, only if we are not on the last line
    }
    fprintf(fp, "%s", "!\n");
    fclose(fp);
    life_destroy(state);
}

bool life_get_cell(LifeState state, LifeCell cell){
    Set line;
    if((line = map_find(state, &cell.x)) != NULL)
        if(set_find(line, &cell))
            return true;
    return false;
}

void life_set_cell(LifeState state, LifeCell cell, bool value){
    Set line = map_find(state, &cell.x);
    if(value){
        if(line == NULL){                                       //If x line does not exist
            LifeCell* new_cell = malloc(sizeof(LifeCell));      //Allocate memory for new LifeCell and copy cell coordinates
            new_cell->x = cell.x;
            new_cell->y = cell.y;
            line = set_create((CompareFunc)set_compare, NULL);  //Create new line, insert LifeCell in it and insert line in map
            set_insert(line, new_cell);
            map_insert(state, create_int(cell.x), line);
        }else{
            LifeCell* new_cell = malloc(sizeof(LifeCell));
            new_cell->x = cell.x;
            new_cell->y = cell.y;
            set_insert(line, new_cell);
        }
    }else{
        if(line != NULL){
            free(set_find(line, &cell));
            if(set_size(line) == 0){
                set_destroy(line);
                int* key  = map_node_key(state, map_find_node(state, &(cell.y)));
                free(key);
            }
        }
    }
}

LifeState life_evolve(LifeState state){
    LifeState new_state = life_create();
    
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
        Set line = map_node_value(state, map_node);
        for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
            LifeCell current_cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
            if(current_cell.x < min_x)
                min_x = current_cell.x;
            if(current_cell.x > max_x)
                max_x = current_cell.x;
            if(current_cell.y < min_y)
                min_y = current_cell.y;
            if(current_cell.y > max_y)
                max_y = current_cell.y;
            for(int x = current_cell.x - 1; x <= current_cell.x + 1; x++){
                for(int y = current_cell.y - 1; y <= current_cell.y + 1; y++){
                    LifeCell cell = {x, y};
                    char neighbours = 0;
                    for(int i = x - 1; i <= x + 1; i++){
                        for(int j = y - 1; j <= y + 1; j++){
                            if(i != x || j != y){
                                LifeCell neighbour = {i, j};
                                neighbours += life_get_cell(state, neighbour);  //If life_get_cell returns 1(true), then neighbour is alive
                            }
                            if(neighbours == 4)
                                break;
                        }
                        if(neighbours == 4)
                            break;
                    }
                    if(neighbours < 2 || neighbours > 3)             //There can't be any living cell with neighbours < 2 || neighbours > 3
                        continue;
                    if(cell.x == current_cell.x && cell.y == current_cell.y){               //Since cell = current cell, we know it's alive
                        life_set_cell(new_state, cell, true);
                        continue;
                    }
                    if(life_get_cell(state, cell)){
                        life_set_cell(new_state, cell, true);
                    }else{
                        if(neighbours == 3){                      //If cell is dead, but has 3 neighbours, it should be alive in next state
                            life_set_cell(new_state, cell, true);
                        }
                    }
                }
            }
        }
    }
    return new_state;
}

void life_destroy(LifeState state){
    // for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
    //     Set line = map_node_value(state, map_node);
    //     for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
        
    //     }
    // }
    // map_destroy(state);
}

char* RLE_to_String(LifeState state){
    int size = 50;
    char* result = malloc(size);
    int i = 0;  //Count 
    for(int x = min_x; x <= max_x; x++){
        int times = 0;
        char last = 0, newc = 0;                    //Keep track of old and new charater
        for(int y = min_y; y <= max_y; y++){
            LifeCell cell;
            cell.x = x;
            cell.y = y;
            if(life_get_cell(state, cell)){
                newc  = 'o';
            }else{
                newc = 'b';
            }
            if(last == 0){
                last = newc;
                times = 1;
            }else if(last == newc){                 //Count consecutive alive or dead cells
                times++;
            }else{
                if(times > 1){
                    int temp = times, length = 0;
                    while(temp){
                        temp/=10;
                        length++;
                    }
                    char times_str[length];
                    sprintf(times_str, "%d", times);
                    if(length + i > size){
                        char temp[i + 1];
                        strcpy(temp, result);
                        free(result);
                        size *= 2;
                        result = malloc(size);
                        strcpy(result, temp);
                    }
                    for(int k = 0; k < length; k++){
                        result[i] = times_str[k] + '0';
                        i++;
                    }
                }
                if(i + 1 > size){
                    char temp[i + 1];
                    strcpy(temp, result);
                    free(result);
                    size *= 2;
                    result = malloc(size);
                    strcpy(result, temp);
                }
                result[i] = last;
                i++;
                last = newc;
                times = 1;
            }
        }
        if(times > 1){
            int temp = times, length = 0;
            while(temp){
                temp/=10;
                length++;
            }
            char times_str[length];
            sprintf(times_str, "%d", times);
            if(length + i > size){
                char temp[i + 1];
                strcpy(temp, result);
                free(result);
                size *= 2;
                result = malloc(size);
                strcpy(result, temp);
            }
            for(int k = 0; k < length; k++){
                result[i] = times_str[k] + '0';
                i++;
            }
        }
        if(i + 1 > size){
            char temp[i + 1];
            strcpy(temp, result);
            free(result);
            size *= 2;
            result = malloc(size);
            strcpy(result, temp);
        }
        result[i] = last;
        i++;
        if(x != max_x){
            if(i + 1 > size){
                char temp[i + 1];
                strcpy(temp, result);
                free(result);
                size *= 2;
                result = malloc(size);
                strcpy(result, temp);
            }
            result[i] = '$';
            i++;
        }
    }
    if(i + 1 > size){
        char temp[i + 1];
        strcpy(temp, result);
        free(result);
        size += 1;
        result = malloc(size);
        strcpy(result, temp);
    }
    result[i] = '\0';
    return result;
}

List life_evolve_many(LifeState state, int steps, ListNode* loop){
    List list_with_states = list_create(NULL);  //List containing all states
    loop = NULL;    //Set loop to NULL

    Map rles = map_create((CompareFunc)strcmp, free, NULL); //Create map RLE => ListNode

    list_insert_next(list_with_states, list_last(list_with_states), state); //Insert the first state in list
    
    LifeState new_state = life_evolve(state); //Evolve to next state
    char continue_checking = 1;
    for(int i = 1; i < steps; i++){
        if(continue_checking){
            char* rle = strdup(RLE_to_String(new_state));   //Get RLE of this state
            MapNode similar_node;
            if((similar_node = map_find_node(rles, rle)) == MAP_EOF){   //Check if there is a ListNode with the same RLE in the map
                list_insert_next(list_with_states, list_last(list_with_states), new_state); //Insert LifeState in list with states
                map_insert(rles, rle, list_last(list_with_states)); //Insert ListNode with its RLE as id
            }else{
                ListNode similar_list_node = map_node_value(rles, similar_node);    //Map contains list_nodes
                LifeState similar_life_state = list_node_value(list_with_states, similar_list_node);      //Convert list_node to LifeState

                Set first_line_in_evolved = map_node_value(new_state, map_first(new_state));
                LifeCell first_in_evolved = *((LifeCell*)set_node_value(first_line_in_evolved, set_first(first_line_in_evolved)));

                Set first_line_in_similar = map_node_value(similar_life_state, map_first(similar_life_state));
                LifeCell first_in_similar = *((LifeCell*)set_node_value(first_line_in_similar, set_first(first_line_in_similar)));

                if(first_in_evolved.x == first_in_similar.x && first_in_evolved.y == first_in_similar.y){
                    //WE HAVE THE SAME STATE!
                    *loop = list_next(list_with_states,  similar_list_node);
                    return list_with_states;
                }else{
                    continue_checking = 0;
                }
            }
        }else{
            list_insert_next(list_with_states, list_last(list_with_states), new_state);
        }
        new_state = life_evolve(new_state);
    }
    return list_with_states;
}

//List life_evolve_many_with_displacement(LifeState state, int steps, ListNode* loop){

//}

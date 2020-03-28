#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

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
    min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    LifeState state = life_create();
    FILE *fp = fopen(file, "rb");
    assert(fp != NULL);
    char c;
    int times = 0;
    while((c = fgetc(fp)) != '!'){
        //Skip unnecessary lines
        if(c == '#' || c == 'x'){
            while(c != '\n')
                c = fgetc(fp);
            continue;
        }else if(c >= '0' && c <= '9'){ //Check if input is a number
            times *= 10;                       //Convert char to int
            times += (c - '0');
        }else if(c == 'o'){         //Check if input is a live cell
            //INSERT IN MAP WHILE MOVING $Y, BUT KEEPING X THE SAME
	    if(times == 0)
		times = 1;
	    //printf("times = %d\n", times);
            while(times--){
                //printf("%d, %d\n", x, y);
                LifeCell cell = {x, y};
                life_set_cell(state, cell, true);
                y++;
            }
            times = 0;   //Reset times to be used in next iterations
        }else if(c == 'b'){
            if(times == 0)
		times = 1;
            while(times--)
                y++;               //Move $Y when reading dead cells
            times = 0;   //Reset times to be used in next iterations
        }else if(c == '$'){        //Check if we have to change line
            if(times == 0)
		times = 1;
            while(times--)
                x++;                       //and prepare coordinates
            max_y = y > max_y ? y : max_y;
            y = 0;
            times = 0;
        }
    }
    max_x = x;
    fclose(fp);
    return state;
}

void life_save_to_rle(LifeState state, char* file){
    min_x = INT_MAX, min_y = INT_MAX, max_x = INT_MIN, max_y = INT_MIN;
    int i = 0;
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node), i++){
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
    FILE *fp;
    fp = fopen(file, "w");
    assert(fp != NULL);
    for(int i = min_x; i <= max_x; i++){
        int times = 0;
        char last = 0, newc = 0;
        for(int j = min_y; j <= max_y; j++){
            LifeCell cell;
            cell.x = i;
            cell.y = j;
            if(life_get_cell(state, cell)){
                newc  = 'o';
            }else{
                newc = 'b';
            }
            if(last == 0){
                last = newc;
                times = 1;
            }else if(last == newc){
                times++;
            }else if(last == newc){
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
        if(i != max_x) fprintf(fp, "%c", '$');             //Change line
    }
    fprintf(fp, "%s", "!\n");                 //EOF
    fclose(fp);
    //life_destroy(state);
}

bool life_get_cell(LifeState state, LifeCell cell){
    Set line;
    if((line = map_find(state, &cell.x)) != NULL)
        if(set_find(line, &cell))
            return true;
    return false;
}

void life_set_cell(LifeState state, LifeCell cell, bool value){
    if(value){
        Set line = map_find(state, &cell.x);
        if(line == NULL){  //If x line does not exist
            LifeCell* new_cell = malloc(sizeof(LifeCell));
            new_cell->x = cell.x;
            new_cell->y = cell.y;
            line = set_create((CompareFunc)set_compare, free);
            set_insert(line, new_cell);
            map_insert(state, create_int(cell.x), line);
        }else{                                          //If x line exists
            //if(set_find(line, &cell) == NULL){
                LifeCell* new_cell = malloc(sizeof(LifeCell));
                new_cell->x = cell.x;
                new_cell->y = cell.y;
                set_insert(line, new_cell);
            //}
        }
    }
}

LifeState life_evolve(LifeState state){
    LifeState new_state = life_create();
    //printf("Called life_evolve\n");
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
        Set line = map_node_value(state, map_node);
        for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
            LifeCell current_cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
	    //printf("%d, %d\n", current_cell.x, current_cell.y);
            for(int x = current_cell.x - 1; x <= current_cell.x + 1; x++){
                for(int y = current_cell.y - 1; y <= current_cell.y + 1; y++){
                    LifeCell cell = {x, y};
                    char neighbours = 0;
                    for(int i = x - 1; i <= x + 1; i++){
                        for(int j = y - 1; j <= y + 1; j++){
                            if(i != x || j != y){
                                LifeCell neigh = {i, j};
                                neighbours += life_get_cell(state, neigh);
                            }
                           if(neighbours == 4)
                                break;
                        }
                        if(neighbours == 4){
                            break;
                        }
                    }
                    if(neighbours < 2 || neighbours > 3)
                        continue;
                    if(life_get_cell(state, cell)){            //the cell (i, j) is alive
                        //if(neighbours >= 2 && neighbours <= 3){
                            life_set_cell(new_state, cell, true);
                        //
                    }else{
                        if(neighbours == 3){
                            life_set_cell(new_state, cell, true);
                        }
                    }
                }
            }
        }
    }
    //life_destroy(state);
    return new_state;
}

void life_destroy(LifeState state){
    //for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
    //    Set line = map_node_value(state, map_node);
    //    set_destroy(line);
    //}
    //map_destroy(state);
}


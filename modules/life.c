#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

int x = 0, y = 0, min_x_left = INT_MAX, min_y_left = INT_MAX, max_x_right = INT_MIN, max_y_right = INT_MIN;

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
    //REMINDER: SET DESTROY VALUE
}

LifeState life_create_from_rle(char* file){
    LifeState state = life_create();
    FILE *fp;
    fp = fopen(file, "rb");
    assert(fp != NULL);
    char c;
    int times = 0;
    while((c = fgetc(fp)) != '!'){
        //Skip unnecessary lines
        if(c == '#' || c == 'x'){
            while(c != '\n')
                c = fgetc(fp);
            continue;
        }

        if(c >= '1' && c <= '9'){       //Check if input is a number
            times *= 10;                       //Convert char to int
            times += (c - '0');
        }else if(c == 'o'){          //Check if input is a live cell
            if(times == 0)
                times = 1;
            //INSERT IN MAP WHILE MOVING $Y, BUT KEEPING X THE SAME
            while(times--){
                LifeCell cell = {x, y};
		        life_set_cell(state, cell, true);
                y++;
            }
            times = 1;   //Reset times to be used in next iterations
        }else if(c == 'b'){
	    if(times == 0)
		times = 1;
            while(times--)
                y++;               //Move $Y when reading dead cells
            times = 1;   //Reset times to be used in next iterations
        }else if(c == '$'){        //Check if we have to change line
            x++;                           //and prepare coordinates
            y = 0;
	    times = 0;
        }
    }

    fclose(fp);
    return state;
}

void life_save_to_rle(LifeState state, char* file){
    //printf("%d - %d\n%d - %d\n", min_x_left, max_x_right, min_y_left, max_y_right);
    int i = 0;
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node), i++){
        Set line = map_node_value(state, map_node);
        for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
            LifeCell cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
            if(cell.x < min_x_left)
                min_x_left = cell.x;
            if(cell.x > max_x_right)
                max_x_right = cell.x;
            if(cell.y < min_y_left)
                min_y_left = cell.y;
            if(cell.y > max_y_right)
                max_y_right = cell.y;
        }
    }
    FILE *fp;
    fp = fopen(file, "w");
    assert(fp != NULL);
    for(int i = min_x_left; i <= max_x_right; i++){
        for(int j = min_y_left; j <= max_y_right; j++){
            LifeCell cell;
            cell.x = i;
            cell.y = j;
            if(life_get_cell(state, cell)){
                fprintf(fp, "%c", 'o');     //Alive cell
            }else{
                fprintf(fp, "%c", 'b');     //Dead cell
            }
        }
        fprintf(fp, "%c", '$');             //Change line
    }
    fprintf(fp, "%s", "!\n");                 //EOF
    fclose(fp);
}

bool life_get_cell(LifeState state, LifeCell cell){
    Set line;
    if((line = map_find(state, &cell.x)) != NULL)
        if(set_find(line, &cell))
            return 1;
    return 0;
}

void life_set_cell(LifeState state, LifeCell cell, bool value){
    Set line;
    if(value){
        if((line = map_find(state, &cell.x)) == NULL){  //If x line does not exist
            line = set_create((CompareFunc)set_compare, free);
            LifeCell* new_cell = malloc(sizeof(LifeCell));
            new_cell->x = cell.x;
            new_cell->y = cell.y;
            set_insert(line, new_cell);
            map_insert(state, create_int(cell.x), line);
        }else{                                          //If x line exists
            LifeCell* new_cell = malloc(sizeof(LifeCell));
            new_cell->x = cell.x;
            new_cell->y = cell.y;
            set_insert(line, new_cell);
        }
    }else{
	    if((line = map_find(state, &cell.x)) != NULL){
            set_remove(line, &cell);
        }
        if(set_size(line) == 0){
            set_destroy(line);
            map_remove(state, line);
        }
    }
}

LifeState life_evolve(LifeState state){
    LifeState new_state = life_create();
    //Copy old state to new state
    int i = 0;
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node), i++){
        Set temp_line = set_create((CompareFunc)set_compare, free), line = map_node_value(state, map_node);
        for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
            LifeCell *cell = malloc(sizeof(LifeCell));
            cell->x = ((LifeCell*)set_node_value(line, node))->x;
            cell->y = ((LifeCell*)set_node_value(line, node))->y;
            set_insert(temp_line, cell);
            if(cell->x < min_x_left)
                min_x_left = cell->x;
            if(cell->x > max_x_right)
                max_x_right = cell->x;
            if(cell->y < min_y_left)
                min_y_left = cell->y;
            if(cell->y > max_y_right)
                max_y_right = cell->y;
        }
        map_insert(new_state, create_int(i), temp_line);
    }

    for(int i = min_x_left - 1; i <= max_x_right + 1; i++){
        for(int j = min_y_left - 1; j <= max_y_right + 1; j++){
            LifeCell cell = {i, j};
            LifeCell cell_upper_left = {i - 1, j - 1};
            LifeCell cell_upper = {i - 1, j};
            LifeCell cell_upper_right = {i - 1, j + 1};

            LifeCell left = {i, j - 1};
            LifeCell right = {i, j + 1};

            LifeCell cell_lower_left = {i + 1, j - 1};
            LifeCell cell_lower = {i + 1, j};
            LifeCell cell_lower_right = {i + 1, j + 1};
            int neighnours =  life_get_cell(state, cell_upper_left) 
                            + life_get_cell(state, cell_upper)
                            + life_get_cell(state, cell_upper_right)
                            + life_get_cell(state, left)
                            + life_get_cell(state, right)
                            + life_get_cell(state, cell_lower_left)
                            + life_get_cell(state, cell_lower)
                            + life_get_cell(state, cell_lower_right);
            if(life_get_cell(state, cell)){
                //the cell (i, j) is alive
                if(neighnours < 2 || neighnours > 3){
                    life_set_cell(new_state, cell, false);
                }
            }else{
                if(neighnours == 3){
                    life_set_cell(new_state, cell, true);
                }
            }
        }
    }
   // life_destroy(state);
    return new_state;
}

void life_destroy(LifeState state){
    map_destroy(state);
}

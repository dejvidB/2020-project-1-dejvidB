#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
int x = 0, y = 0;

int compare_x(LifeCell *a, LifeCell *b){
    return a->x - b->x;
}

int compare_y(LifeCell *a, LifeCell *b){
   return a->y - b->y;
}

LifeState life_create(){
    return set_create((CompareFunc)compare_x, NULL);
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
                LifeCell *cell = malloc(sizeof(LifeCell));
                cell->x = x;
                cell->y = y;
		set_insert(state, cell);
               // life_set_cell(state, *cell, true);
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
    for(SetNode node = set_last(state); node != SET_BOF; node = set_previous(state, node)){
	printf("I\n");
        //set_insert(new_state, set_node_value(state, node));
        //life_set_cell(new_state, node, true);
        LifeCell *cell = (LifeCell*)set_node_value(state, node);
        int x = 0, y = 0;
        //LifeCell cell = (LifeCell)node;
        x = cell->x;
        y = cell->y;
        printf("Iterate %d,%d\n", x, y);
    }

    fclose(fp);
    return state;
}

void life_save_to_rle(LifeState state, char* file){
    printf("\n%d,%d\n", x, y);
    FILE *fp;
    fp = fopen(file, "w");
    assert(fp != NULL);
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            LifeCell cell;
            cell.x = i;
            cell.y = j;
            if(set_find(state, &cell) == NULL){
                fprintf(fp, "%c", 'b');     //Dead cell
            }else{
                fprintf(fp, "%c", 'o');     //Alive cell
            }
        }
        fprintf(fp, "%c", '$');             //Change line
    }
    fprintf(fp, "%s", "!\n");                 //EOF
    fclose(fp);
}

bool life_get_cell(LifeState state, LifeCell cell){
    return set_find(state, &cell) == NULL ? 0 : 1;
}

void life_set_cell(LifeState state, LifeCell cell, bool value){
    if(value){
	set_insert(state, &cell);
    }else{
	set_remove(state, &cell);
    }
}

LifeState life_evolve(LifeState state){
    LifeState new_state = life_create();
    //Copy old state to new state
   // for(SetNode node = set_first(state); node != SET_EOF; node = set_next(state, node)){
  //     set_insert(new_state, set_node_value(state, node));
        //life_set_cell(new_state, node, true);
//        LifeCell cell = *((LifeCell*)set_node_value(state, node));
//	int x = 0, y = 0;
	//LifeCell cell = (LifeCell)node;
//	x = cell.x;
	//y = cell.y;
	//printf("-- %d,%d\n", x, y);
//    }

    for(int i = 0; i <= x; i++){
        for(int j = 0; j <= y; j++){
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
            if(set_find(state, &cell) != NULL){
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
    set_destroy(state);
}

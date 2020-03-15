#include "life.h"
#include <stdio.h>
#include <stdlib.h>

int x = 0, y = 0;

int compare(LifeCell a, LifeCell b){
    return (int) sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

LifeState life_create(){
    return set_create(compare, NULL);
}

LifeState life_create_from_rle(char* file){
    LifeState state = life_create();
    FILE *fp;
    fp = fopen(file, "rb");
    assert(fp != NULL);
    char c;
    int line = 0, times = 0;
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
                LifeCell cell;
                cell.x = x;
                cell.y = y;
                life_set_cell(state, cell, true);
                y++;
            }
            times = 1;   //Reset times to be used in next iterations
        }else if(c == 'b'){
            while(times--)
                y++;               //Move $Y when reading dead cells
            times = 1;   //Reset times to be used in next iterations
        }else if(c == '$'){        //Check if we have to change line
            x++;                           //and prepare coordinates
            y = 0;
        }
    }
    fclose(fp);
    return state;
}

void life_save_to_rle(LifeState state, char* file){
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
    fprintf(fp, "%c", "!");                 //EOF
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
    for(SetNode node = set_first(state); node != SET_EOF; node = set_next(state, node)){
        set_insert(new_state, set_node_value(state, node));
        //life_set_cell(new_state, node, true);
    }

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            LifeCell cell = {i, j};
            LifeCell cell_upper_left = {i - 1, j - 1};
            LifeCell cell_upper = {i - 1, j};
            LifeCell cell_upper_right = {i + 1, j + 1};

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
    life_destroy(state);
    return new_state;
}

void life_destroy(LifeState state){
    set_destroy(state);
}
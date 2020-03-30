#include "bmp.h"
#include "gif.h"
#include "ADTVector.h"
#include <string.h>
#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef char* String;
extern int max_x, max_y, min_x, min_y;

#define TOP 0
#define LEFT 1
#define BOTTOM 2
#define RIGHT 3

int main(int argc, char *argv[]) {
    int frames = 1;
    if(atoi(argv[6]) > 1)
        frames = atoi(argv[6]);

    // float zoom = 1;
    // if(atof(argv[7]) > 0)
    //     zoom = atof(argv[7]);

    int speed = 1;
    if(atoi(argv[8]) >= 1)
        speed = atoi(argv[8]);

    int delay = atoi(argv[9]);

    String state_file = strdup(argv[1]);

    int limits[4];
    limits[TOP] = limits[LEFT] = INT_MIN;
    limits[BOTTOM] = limits[RIGHT] = INT_MAX;

    limits[TOP] = atoi(argv[2]);

    if(atoi(argv[4]) < limits[TOP])   //If bottom is indeed lower than top
        limits[BOTTOM] = atoi(argv[4]);

    limits[LEFT] = atoi(argv[3]);

    if(atoi(argv[5]) > limits[LEFT])   //If right is indeed right
        limits[RIGHT] = atoi(argv[5]);
    
    String target_gif = strdup(argv[10]);
    LifeState state = life_create_from_rle(state_file);

	// Μεγέθη
	int size = 300;
	//int cell_size = 50;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif = gif_create(size, size);
	Bitmap* bitmap = bm_create(size, size);

	// Default καθυστέρηση μεταξύ των frames, σε milliseconds
	gif->default_delay = delay;

    for(int i = 0; i < frames; i++){
        bm_set_color(bitmap, bm_atoi("white"));
        bm_clear(bitmap);
        for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
            Set line = map_node_value(state, map_node);
            for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
                LifeCell cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
                if(cell.x >= limits[TOP] && cell.x <= limits[BOTTOM] && cell.y >= limits[LEFT] && cell.y <= limits[RIGHT]){
                    bm_set_color(bitmap, bm_atoi("black"));
                    bm_putpixel(bitmap, cell.y + size/2, cell.x + size/2);
                }
            }
        }
        gif_add_frame(gif, bitmap);

        //Evlove speed φορές
        for(int j = 0; j < speed; j++){
            state = life_evolve(state);
        }
    }

	// Αποθήκευση σε αρχείο
	gif_save(gif, target_gif);

	// Αποδέσμευση μνήμης
	bm_free(bitmap);
	gif_free(gif);
}

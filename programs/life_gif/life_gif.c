#include "bmp.h"
#include "gif.h"
#include "ADTVector.h"
#include <string.h>
#include "life.h"
#include <stdio.h>
#include <stdlib.h>

typedef char* String;
extern int max_x, max_y, min_x, min_y;

int main(int argc, char *argv[]) {
    int frames = 1;
    if(atoi(argv[6]) >= 1)
        frames = atoi(argv[6]);

    // float zoom = 1;
    // if(atof(argv[7]) > 0)
    //     zoom = atof(argv[7]);

    int speed = 1;
    if(atoi(argv[8]) >= 1)
        speed = atoi(argv[8]);

    // int delay = 10;
    // if(atoi(argv[9]) > 0)
    //     delay = atoi(argv[9]);

    String state_file = strdup(argv[1]);
    //int top = atoi(argv[2]), left = atoi(argv[3]), bottom = atoi(argv[4]), right = atoi(argv[5]);
    
    String target_gif = strdup(argv[10]);
    LifeState state = life_create_from_rle(state_file);

	// Μεγέθη
	//int size = 300;
	//int cell_size = 50;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif = gif_create(300, 300);
	Bitmap* bitmap = bm_create(300, 300);

	// Default καθυστέρηση μεταξύ των frames, σε εκατοστά του δευτερολέπτου
	gif->default_delay = 1;
    //gif->default_delay = delay;

    for(int i = 0; i < frames; i++){
	//printf("new frame\n");
        bm_set_color(bitmap, bm_atoi("white"));
        bm_clear(bitmap);
        for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
            Set line = map_node_value(state, map_node);
	    //printf("---------\n");
            for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
                LifeCell cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
		//printf("%d, %d\n", cell.x, cell.y);
                bm_set_color(bitmap, bm_atoi("black"));
                bm_putpixel(bitmap, cell.y + 130, cell.x + 130);
            }
        }
        gif_add_frame(gif, bitmap);
        //Evlove speed φορές
        for(int j = 0; j < speed; j++){
            state = life_evolve(state);
        }
     }

    	life_save_to_rle(state, "result");
	// Αποθήκευση σε αρχείο
	gif_save(gif, target_gif);

	// Αποδέσμευση μνήμης
	bm_free(bitmap);
	gif_free(gif);
}

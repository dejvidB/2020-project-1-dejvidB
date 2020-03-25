#include "bmp.h"
#include "gif.h"
#include "ADTVector.h"
#include <string.h>
#include "life.h"
#include <stdio.h>
#include <stdlib.h>

typedef char* String;

int main(int argc, char *argv[]) {
    int frames = 1;
    if((frames = atoi(argv[6])) < 1) //frames >= 1
        return 1;

    float zoom = 0;
    if((zoom = atof(argv[7]) < 0))  //zoom
        return 1;

    int speed = 1;
    if((speed = atoi(argv[8])) < 1)
        return 1;

    int delay = 0;
    if((delay = atoi(argv[9])) < 0)
        return 1;

    String state_file = strdup(argv[1]);
   // int top = atoi(argv[2]), left = atoi(argv[3]), bottom = atoi(argv[4]), right = atoi(argv[5]);
    
    String target_gif = strdup(argv[10]);

    LifeState state = life_create_from_rle(state_file);

	// Μεγέθη
	int size = 256;
	//int cell_size = 50;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif = gif_create(size, size);
	Bitmap* bitmap = bm_create(size, size);

	// Default καθυστέρηση μεταξύ των frames, σε εκατοστά του δευτερολέπτου
	gif->default_delay = 0;

	// Δημιουργούμε ενα animation με ένα "cell" το οποίο μετακινείται από τη δεξιά-πάνω
	// γωνία προς την κάτω-αριστερά. Το cell μετακινείται ένα pixel τη φορά, οπότε το animation
	// θα έχει τόσα frames όσα το μέθεθος της εικόνας.

    //Απεικόνιση αρχικής κατάστασης
    //πρώτα μαυρίζουμε ολόκληρο το bitmap
    bm_set_color(bitmap, bm_atoi("black"));
    bm_clear(bitmap);
    for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
        Set line = map_node_value(state, map_node);
        for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
            LifeCell cell;
            cell.x = ((LifeCell*)set_node_value(line, node))->x;
            cell.y = ((LifeCell*)set_node_value(line, node))->y;
	    //printf("%d, %d\n", cell.x, cell.y);
            // Και μετά ζωγραφίζουμε ένα άσπρο τετράγωνο με αρχή το
            // σημείο (i,i) και τέλος το (i+cell_size, i+cell_size)
            bm_set_color(bitmap, bm_atoi("white"));
           //bm_fillrect(bitmap, cell.x + size/2, cell.y + size/2, cell.x + size/2 + cell_size, cell.y + size/2 + cell_size);
	    bm_putpixel(bitmap, cell.x + size/2, cell.y + size/2);
            // Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
        }
    }
    gif_add_frame(gif, bitmap);

    for(int i = 1; i < frames; i++){
        //Evlove speed φορές
        for(int j = 0; j < speed; j++){
            state = life_evolve(state);
        }
        bm_set_color(bitmap, bm_atoi("black"));
	bm_clear(bitmap);
        //bm_fillrect(bitmap, cell.x + size/2, cell.y + size/2, cell.x + size/2 + cell_size, cell.y + size/2 + cell_size);
        //bm_putpixel(bitmap, cell.y + size/2, cell.x + size/2);

        for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
            Set line = map_node_value(state, map_node);
            for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
                LifeCell cell;
                cell.x = ((LifeCell*)set_node_value(line, node))->x;
                cell.y = ((LifeCell*)set_node_value(line, node))->y;
                // Και μετά ζωγραφίζουμε ένα άσπρο τετράγωνο με αρχή το
                // σημείο (i,i) και τέλος το (i+cell_size, i+cell_size)
                bm_set_color(bitmap, bm_atoi("white"));
                bm_putpixel(bitmap, cell.x + size/2, cell.y + size/2);
                // Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
                //gif_add_frame(gif, bitmap);
            }
        }
        gif_add_frame(gif, bitmap);
    }

    life_save_to_rle(state, "result");
	// Αποθήκευση σε αρχείο
	gif_save(gif, target_gif);

	// Αποδέσμευση μνήμης
	bm_free(bitmap);
	gif_free(gif);
}

#include "bmp.h"
#include "gif.h"
#include "life.h"

typedef char* String;
extern int displacement_x, displacement_y;

#define TOP 0
#define LEFT 1
#define BOTTOM 2
#define RIGHT 3

int main(int argc, char *argv[]) {
    if(argc < 11){
        printf("Wrong usage.\n life_gif <state> <top> <left> <bottom> <right> <frames> <zoom> <speed> <delay> <gif>\n");
        return 1;
    }

    int frames = 1;
    if(atoi(argv[6]) > 1)
        frames = atoi(argv[6]);

    int speed = 1;
    if(atoi(argv[8]) > 1)
        speed = atoi(argv[8]);

    int delay = atoi(argv[9]);

    String state_file = argv[1];

    int limits[4];

    limits[TOP] = atoi(argv[2]);

    if(atoi(argv[4]) > limits[TOP])   //If bottom is indeed lower than top
        limits[BOTTOM] = atoi(argv[4]);
    else
        limits[BOTTOM] = limits[TOP] + 300;

    limits[LEFT] = atoi(argv[3]);

    if(atoi(argv[5]) > limits[LEFT])   //If right is indeed right
        limits[RIGHT] = atoi(argv[5]);
    else
        limits[RIGHT] = limits[LEFT] + 300;

    String target_gif = argv[10];

    int x = limits[BOTTOM] - limits[TOP];
    int y = limits[RIGHT] - limits[LEFT];

    // Δημιουργία ενός GIF και ενός bitmap στη μνήμη
    GIF* gif = gif_create(y, x);
    Bitmap* bitmap = bm_create(y, x);

    // Default καθυστέρηση μεταξύ των frames, σε milliseconds
    gif->default_delay = delay;

    ListNode loop;
    List states = life_evolve_many_with_displacement(life_create_from_rle(state_file), frames * speed, &loop);
    ListNode temp = list_first(states);
    LifeState state;

    int plus_x = 0, plus_y = 0;
    for(int i = 0; i < frames; i++){
        state = list_node_value(states, temp);
        bm_set_color(bitmap, bm_atoi("white"));
        bm_clear(bitmap);
        bm_set_color(bitmap, bm_atoi("black"));
        for(MapNode map_node = map_first(state); map_node != MAP_EOF; map_node = map_next(state, map_node)){
            Set line = map_node_value(state, map_node);
            for(SetNode node = set_first(line); node != SET_EOF; node = set_next(line, node)){
                LifeCell cell = {((LifeCell*)set_node_value(line, node))->x, ((LifeCell*)set_node_value(line, node))->y};
                x = cell.x + plus_x - limits[TOP];
                y = cell.y + plus_y - limits[LEFT]; 

                if(cell.x + plus_x >= limits[TOP] && cell.x + plus_x <= limits[BOTTOM] && cell.y + plus_y >= limits[LEFT] && cell.y + plus_y <= limits[RIGHT]){
                    bm_putpixel(bitmap, y, x);
                }
            }
        }
        gif_add_frame(gif, bitmap);
        //Evlove speed φορές, μεταξύ δύο frames
        for(int j = 0; j < speed; j++){
            if(list_next(states, temp) != LIST_EOF){
                temp = list_next(states, temp);
            }else{
                if(loop != NULL){
                    temp = loop;
                    plus_x += displacement_x;
                    plus_y += displacement_y;
                }else{
                    break;
                }
            }
        }
    }

    list_destroy(states);

    // Αποθήκευση σε αρχείο
    gif_save(gif, target_gif);

    // Αποδέσμευση μνήμης
    bm_free(bitmap);
    gif_free(gif);
    return 0;
}
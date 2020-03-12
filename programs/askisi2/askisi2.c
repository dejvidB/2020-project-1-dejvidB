#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTMap.h"

#define MAX_LENGTH 200

int* create_int(int value) {
    int* pointer = malloc(sizeof(int)); // δέσμευση μνήμης
    *pointer = value;                   // αντιγραφή του value στον νέο ακέραιο
    return pointer;
}

char* create_str(char* value){
	char* pointer = malloc((sizeof(strlen(value) + 1) * sizeof(char)));
	strcpy(pointer, value);
	return pointer;
}

int main(void){
	Map map = map_create((CompareFunc)strcmp, free, free);
	char *temp = malloc(sizeof(char) * (MAX_LENGTH + 1));
	if (temp == NULL)
		return 1; //Could not allocate memory
	while (fgets(temp, MAX_LENGTH, stdin) != NULL){
		int counter = 0;
		MapNode key = MAP_EOF;
		if ((key = map_find_node(map, temp)) == MAP_EOF){
			map_insert(map, create_str(temp), create_int(1));
		}else{
			int *node_value = (int *)map_node_value(map, key);
			counter = *node_value;
			(*node_value)++;
		}
		printf("%d\n", counter);
	}

	map_destroy(map);
	return 0;
}

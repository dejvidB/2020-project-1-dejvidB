#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTMap.h"

#define MAX_LENGTH 200

int* create_int(int value){
    int* pointer = malloc(sizeof(int));
    *pointer = value;
    return pointer;
}

int main(void){
	Map map = map_create((CompareFunc)strcmp, free, free);
	char *temp = malloc(sizeof(char) * (MAX_LENGTH + 1));
	if (temp == NULL)
		return 1; //Could not allocate memory
	while (fgets(temp, MAX_LENGTH, stdin) != NULL){	//Read line and put it in `temp` variable
		int counter = 0;
		MapNode key;
		if ((key = map_find_node(map, temp)) == MAP_EOF){
			map_insert(map, strdup(temp), create_int(1));	//Insert line as a key in map, and value = 1, because it's new
		}else{
			int *node_value = (int *)map_node_value(map, key);
			counter = *node_value;
			(*node_value)++;
		}
		printf("%d\n", counter);
	}
	free(temp);
	map_destroy(map);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTSet.h"

#define MAX_LENGTH 200

int main(void){
	Set set = set_create((CompareFunc)strcmp, free);
	char *temp = malloc(sizeof(char) * (MAX_LENGTH + 1));
	if (temp == NULL)
		return 1; //Could not allocate memory

	while (fgets(temp, MAX_LENGTH, stdin) != NULL){
		SetNode node = set_first(set);
		while (node != SET_EOF && strcmp((char *)set_node_value(set, node), temp) < 0){ //While each line in map is "smaller" than new line
			node = set_next(set, node);	//Go to the next line
		}
		if (node == SET_EOF){
			printf("<none>\n");
		}else{
			printf("%s\n", (char*)set_node_value(set, node));
		}
		set_insert(set, strdup(temp));
	}

	free(temp);
	set_destroy(set);
	return 0;
}
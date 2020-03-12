#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ADTSet.h"

#define MAX_LENGTH 200

char *create_str(char *value){
	char *pointer = malloc((sizeof(strlen(value) + 1) * sizeof(char)));
	strcpy(pointer, value);
	return pointer;
}

int main(void){
	Set set = set_create((CompareFunc)strcmp, free);
	char *temp = malloc(sizeof(char) * (MAX_LENGTH + 1));
	if (temp == NULL)
		return 1; //Could not allocate memory
	while (fgets(temp, MAX_LENGTH, stdin) != NULL){
		SetNode node = set_first(set);
		while (node != SET_EOF && strcmp((char *)set_node_value(set, node), temp) < 0){
			node = set_next(set, node);
		}
		if (node == SET_EOF){
			printf("<none>\n");
		}else{
			printf("%s\n", (char*)set_node_value(set, node));
		}
		set_insert(set, create_str(temp));
	}
	free(temp);
	set_destroy(set);
	return 0;
}

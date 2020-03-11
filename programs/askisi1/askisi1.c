#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 100000
#define MAX_LENGTH 20

int main(void){
	int size = 0;
	char **buf = malloc(sizeof(char*) * CAPACITY);
	if(buf == NULL)
		return 1; //Could not allocate memory

	for(int i = 0; i < CAPACITY; i++){
		buf[i] = malloc(sizeof(char) * MAX_LENGTH);
		if(buf[i] == NULL)
			return 1; //Could not allocate memory
	}

	while (fgets(buf[size], MAX_LENGTH, stdin) != NULL){
		int counter = 0;
		for(int i = 0; i < size; i++)
			if(strcmp(buf[i], buf[size]) == 0)
				counter++;
		printf("%d\n", counter);
		size++;	
	}

	for(int i = 0; i < CAPACITY; i++){
		free(buf[i]);
	}
	free(buf);
	return 0;
}

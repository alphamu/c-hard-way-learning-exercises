#include <stdio.h>

int main(int argc, char *argv[]) {

	int i=0;
	while(i < argc) {
		printf("arg %d: %s\n", i, argv[i]);
		i++;
	}

	char *states[] = {
		"Victoria", "New South Whales",
		"Queensland", "Nothern Territory"
	};

	int num_states = 4;
	i = 0;
	while(i < num_states) {
		printf("State %d: %s\n", i, states[i]);
		i++;
	}

	return 0;
}

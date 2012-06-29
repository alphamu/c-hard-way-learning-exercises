#include <stdio.h>

int main(int argc, char *argv[]) {
	int i=0;
	for(i=1; i < argc; i++) {
		printf("arg %d: %s\n", i, argv[i]);
	}

	char *states[]={
					"New South Wales",
					"Victoria",
					"Western Australia",
					"Northern Territories"
				};

	char test[] = {'a', 'b', 'c', '4', '5'};
	printf("char[] is: %c %c %c %c %c\n", test[0], test[1], test[2], test[3], test[4]);
	printf("Size of char[%d]: %ld\n", 1, sizeof(test[1]));
	printf("Size of char: %ld\n", sizeof(char));
	printf("Size of char [sizeof('a')]: %ld\n", sizeof('a'));
	printf("Size of array: %ld\n", sizeof(states)/sizeof(*states));

	for(i = 0; i < sizeof(states)/sizeof(*states); i++) {
		printf("State %d: %s\n", i, states[i]);
	}

	return 0;
}

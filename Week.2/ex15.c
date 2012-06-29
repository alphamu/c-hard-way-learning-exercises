#include <stdio.h>

int main(int argc, char *argv[])
{
	int ages[] = {13, 19, 27, 32, 49};
	char *names[] = { "Anthony", "Betty", "Carmelo", "Desmond", "Evans" };
	//char *names = "Anthony";

	//safely get the size of ages
	int count = sizeof(ages) / sizeof(int);
	int i = 0;

	//first way of using indexing
	for(i = 0; i < count; i++) {
		printf("%s has %d years alive.\n", names[i], ages[i]);
	}

	printf("---\n");

	//setup the pointers to the start of the arrays
	int *cur_age = ages + count - 1;
	char **cur_name = names + count - 1;

	//second way using pointers
	for(i = 0; i < count; i++) {
		printf("%s is %d years old.\n", *(cur_name-i), *(cur_age-i));
	}

	printf("---\n");

	//third way, pointers are just arrays
	for(i = 0; i < count; i++) {
		printf("%s is %d years old again.\n", cur_name[-i], cur_age[-i]);
	}

	printf("---\n");

	//fourth way with pointers ina  stupid complex way
	for(cur_name = names + count - 1, cur_age = ages + count - 1; (cur_age - ages) >= 0; cur_name--, cur_age--) {
		printf("%s lived %d years so far.\n", *cur_name, *cur_age);
	}
	
	return 0;
}

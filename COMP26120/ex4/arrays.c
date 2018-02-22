#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* these arrays are just used to give the parameters to 'insert',
   to create the 'people' array
*/

#define HOW_MANY 7
char *names[HOW_MANY]= {"Simon", "Suzie", "Alfred", "Chip", "John", "Tim",
		      "Harriet"};
int ages[HOW_MANY]= {22, 24, 106, 6, 18, 32, 24};

/* declare your struct for a person here */
typedef struct Persons
{
  char fullName[50];
  int currentAge;
} person;

static void insert(struct Persons *inputArray[], char *name, int age,
																																int *nextInsert)
{
  /* put name and age into the next free place in the array parameter here */
	struct Persons *new_person = (struct Persons*) malloc(sizeof(struct Persons));

	if (new_person != NULL)
	{
		new_person->currentAge = age;
 		strcpy(new_person->fullName,name);
	}

	inputArray[*nextInsert] = new_person;

  /* modify nextfreeplace here */
	(*nextInsert)++;
}

int main(int argc, char **argv)
{
  /* declare the people array here */
  struct Persons *people[HOW_MANY];
	int nextFreeIndex = 0;

  for (int i = 0; i < HOW_MANY; i++)
  {
    insert (people, names[i], ages[i], &nextFreeIndex);
  }

  // print the people array here
	for (int i = 0; i < HOW_MANY; i++)
  {
		printf("Index: %d , Name: %s, Age: %d\n", i, people[i]->fullName,
																												 people[i]->currentAge);
  }

	// For loop to free memory of all the struct variables.
	for (int i = 0; i < HOW_MANY; i++)
  {
		free(people[i]);
  }

  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumerate type variable to distinguish between staff or student.
typedef enum {staff, student, neither} staff_or_student;

/* these arrays are just used to give the parameters to 'insert',
   to create the 'people' array
*/
#define HOW_MANY 7
char *names[HOW_MANY]= {"Simon", "Suzie", "Alfred", "Chip", "John", "Tim",
		                                                                 "Harriet"};
int ages[HOW_MANY]= {22, 24, 106, 6, 18, 32, 24};


// 2 new arrays for inserting staff or students values.
staff_or_student peopleType[HOW_MANY] = {staff, student, staff, neither, student, 
                                                             neither , student};

char *extraInformation[HOW_MANY]= {"LF31", "A.I", "Kilburn 2.72", 
"No Further Information", "Computer Science", "No Further Information", 
"Computer Science"};


// Struct of a Person with union of extra information based on it's type.
typedef struct Person
{
  char fullName[50];
  int currentAge;

  staff_or_student status;
  union 
  {
    char extraInformation[50];
  } type;

  // Pointer to the struct of the next person.
  struct Person *nextPerson;
};

// Method to insert new struct to the current struct at the start.
struct Person *insert_start(struct Person *input, char *name, int age)
{
  struct Person *curr_ptr = (struct Person*) malloc(sizeof(struct Person));

  curr_ptr->currentAge = age;
  strcpy(curr_ptr->fullName,name);

  curr_ptr->nextPerson = input;
  return curr_ptr;
}

// Method to insert new struct to the current struct at the end.
struct Person *insert_end(struct Person *input, char *name, int age)
{
  struct Person *curr_ptr = (struct Person*) malloc(sizeof(struct Person));

  curr_ptr->currentAge = age;
  strcpy(curr_ptr->fullName,name);

  if (input == NULL)
  {  
    curr_ptr->nextPerson = input;
    return curr_ptr;
  }
  else
  {
    struct Person *root_ptr	= input;

    while(input->nextPerson != NULL)
    {      
      input = input->nextPerson;
    }

    input->nextPerson = curr_ptr;
    return root_ptr;
  }
}

// Method to insert new struct to current struct in a particular order based on 
// it's compare_people argument parameter. 
struct Person *insert_sorted(struct Person *input, char *name, int age,
int compare_people(struct Person *,struct Person *), staff_or_student status,  
char *information)
{
  struct Person *curr_ptr = (struct Person*) malloc(sizeof(struct Person));

  curr_ptr->currentAge = age;
  strcpy(curr_ptr->fullName,name);
  curr_ptr->status = status;
  strcpy(curr_ptr->type.extraInformation, information);

  if (input == NULL)
  {  
    curr_ptr->nextPerson = input;
    return curr_ptr;
  }
  else
  {
    struct Person *root_ptr	= input;
    if (compare_people(input,curr_ptr) > 0)
    {
      curr_ptr->nextPerson = input;
      return curr_ptr;
    } 

    // While loop to find the index that should precede the new person.
    while (input->nextPerson != NULL && 
                                 compare_people(input->nextPerson,curr_ptr) < 0)
    {      
      input = input->nextPerson;
    }

    curr_ptr->nextPerson = input->nextPerson;
    input->nextPerson = curr_ptr;
    return root_ptr;
  }
}

// Method to sort people by alphabetical order of the people's names.
int compare_people_by_name(struct Person *personOne, struct Person *personTwo)
{
  return strcmp(personOne->fullName,personTwo->fullName);
}

// Method to sort struct in age order.
int compare_people_by_age(struct Person *personOne, struct Person *personTwo)
{
  int answer;
  if (personOne->currentAge > personTwo->currentAge)
    answer = 1;
  else if (personOne->currentAge < personTwo->currentAge)
    answer = -1;
  else
    answer = 0;
  return answer;
}

int main(int argc, char **argv)
{
  /* Declare the people array here */
  struct Person *people = NULL;

  for (int i = 0; i < HOW_MANY; i++)
  {
    people = insert_sorted(people, names[i], ages[i], compare_people_by_age,
                                            peopleType[i], extraInformation[i]);
  }

  // Print the people array here and free all the structs.
  while(people != NULL)
  {
    struct Person *temp_ptr = people;
		printf("Name: %s, Age: %d, Extra information: %s \n", people->fullName, 
                             people->currentAge, people->type.extraInformation);
    people = people->nextPerson;
    free(temp_ptr);
  }

  return 0;
}

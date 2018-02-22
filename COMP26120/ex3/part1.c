#include <stdlib.h>
#include <stdio.h>

// Function to return the largest value in an int array.
int largest(int requiredIntArray[], int requiredArraySize) 
{   
  int largestIndex = 0;

  for (int i = 0; i < requiredArraySize; i++) 
  {
    if (requiredIntArray[i] > largestIndex)
      largestIndex = requiredIntArray[i];
  } 

  return largestIndex;
}

// Function to print out the elements in an int array.
void printIntArray(int requiredIntArray[], int requiredArraySize)
{
  for (int i = 0; i < requiredArraySize; i++)
    printf("%d ", requiredIntArray[i]);
}

// Main method to test the implemented functions.
int main (int argc, char *argv[])
{
  int test1[5] = {1000, 2, 3, 7, 50};
  printf("For array ");
  printIntArray(test1, sizeof(test1)/sizeof(*test1));
  printf("the largest value will be %d.\n\n", largest(test1, sizeof(test1)/sizeof(*test1)));
  
  int test2[8] = {7, 5, 9, 4, 17, 10, 6, 7};
  printf("For array ");
  printIntArray(test2, sizeof(test2)/sizeof(*test2));
  printf("the largest value will be %d.\n\n", largest(test2, sizeof(test2)/sizeof(*test2)));

  int test3[8] = {1, 79, 100, 69, 3, 78, 1, 1};
  printf("For array ");
  printIntArray(test3, sizeof(test3)/sizeof(*test3));
  printf("the largest value will be %d.\n\n", largest(test3, sizeof(test3)/sizeof(*test3)));
  
  return 0;
}

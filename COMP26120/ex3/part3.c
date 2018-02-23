#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
  // Int array to store the length of all the parameter.
  int longestArgumentIndex[argc];
  int largest = 0;
  int index = 0;     

  if (argc > 1)    
  {
    for (int i = 1; i < argc; i++)
    {
      longestArgumentIndex[i-1] = strlen(argv[i]);
    }    
 
    for (int i = 0; i < argc - 1; i++)
    {
      if (largest < longestArgumentIndex[i])
      {
        largest = longestArgumentIndex[i];
        index = i + 1;
      }
    }

    printf("The largest argument string is \"%s\" with length %d.\n",argv[index]
                                                ,longestArgumentIndex[index-1]);
  }
  else
    printf("No argument detected.\n");

  return 0;  
}

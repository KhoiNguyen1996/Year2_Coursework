#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
  // Check if the input parameter is correct.
  if (argc != 2)
  {
    printf("Please only enter 1 argument as filename.\n");
    exit(-1);
  }

  // Array to store the frequency of characters of the input file.
  int charArray[256];

  // Integer value to store the current reach character of the input file. 
  int currentChar;

  // For loop to initialise the value of the char array to 0.
  for (int i = 0; i < sizeof(charArray)/sizeof(int); i++)
  {
    charArray[i] = 0;
  }

  FILE *inputstream= fopen(argv[1], "r");

  if (!inputstream) 
  {
    fprintf(stderr, "can't open file %s for reading\n", argv[1]);
    exit(-1);
  }
	
  // While loop to calculate the frequency of characters.
  while ((currentChar = fgetc(inputstream)) != EOF)
  {
    charArray[currentChar]++;
  }

  fclose(inputstream);
		
  // For loop to print out the frequency of characters.
  for (int i = 0; i < sizeof(charArray)/sizeof(int); i++)
  {
    if (charArray[i] != 0)
    printf("%d instances of character 0x%02x (%c)\n", charArray[i], i,(char)i);
  }

  return 0;		
}

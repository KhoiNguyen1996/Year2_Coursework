#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> 
#include <math.h>   

int main(int argc, char **argv)
{
  int currentChar, charCount, charToUpper, charToLower = 0;
  charToUpper = 0;

  // Char arrays of length 256 characters to store the input/output file names.
  char inputfile[256], outputfile[256];

  printf("Please enter your input file: ");
  scanf("%s", inputfile);
  printf("Your input file is %s.\n", inputfile);

  printf("Please enter your output file: ");
  scanf("%s", outputfile);
  printf("Your output file is %s.\n", outputfile);

  // Inputstream and outputstream for getting data from input file and store it 
  // into output file.
  FILE *inputstream= fopen(inputfile, "r");
  FILE *outputstream= fopen(outputfile, "w");
  
  if (!inputstream) 
  {
    fprintf(stderr, "can't open %s for reading\n", inputfile);
    exit(-1);
  }
  else if (!outputstream)
  {
    fprintf(stderr, "can't open %s for writing\n", outputfile);
    exit(-1);
  }

  currentChar = fgetc(inputstream);

  // While loop that goes through all the characters and convert to upper and 
  // lower case accordingly.
  while(!feof(inputstream))
  {
    if (isupper(currentChar))
    {
      fputc(tolower(currentChar), outputstream);
      charToLower++;
    }
    else if (islower(currentChar))
    {
      fputc(toupper(currentChar), outputstream);
      charToUpper++;
    }
    else
      fputc(currentChar, outputstream);

    charCount++;
    currentChar = fgetc(inputstream);
  }

  fprintf(outputstream,"\nTotal number of character: %d\n", charCount);
  fprintf(outputstream,"Total number of character converted to upper-case: %d\n", charToUpper);
  fprintf(outputstream,"Total number of character converted to lower-case: %d\n", charToLower);

  // Close the input and output streams.
  fclose(inputstream);
  fclose(outputstream);
}

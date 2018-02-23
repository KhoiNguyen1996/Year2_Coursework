#include <stdlib.h>
#include <stdio.h>

// Enum variable type to print out the square box.
typedef enum{ black,white }boardSquare;

// Function to print out the chess board with respect to it's width and length.
void printBlackWhite(int width, int length, boardSquare requiredArray[width][length])
{
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < length; j++)
    {      
      if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
        requiredArray[i][j] = black;
      else
        requiredArray[i][j] = white;  
    }
  }

  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < length; j++)
    {
      if (requiredArray[i][j] == 0)
        printf("black ");
      else
        printf("white ");
    }
    printf("\n");
  }

}

// Main method to test for the implemented function
int main (int argc, char *argv[])
{
  printf("For board size of dimension 6 rows, 9 columns. We have:\n");  
  boardSquare boardSize1[6][9];
  printBlackWhite(6,9,boardSize1);

  printf("\nFor board size of dimension 8 rows, 6 columns. We have:\n");  
  boardSquare boardSize2[8][6];
  printBlackWhite(8,6,boardSize2);

  printf("\nFor board size of dimension 4 rows, 4 columns. We have:\n");  
  boardSquare boardSize3[4][4];
  printBlackWhite(4,4,boardSize3);

  printf("\nFor board size of dimension 9 rows, 9 columns. We have:\n");  
  boardSquare boardSize4[9][9];
  printBlackWhite(9,9,boardSize4);

  return 0;
}

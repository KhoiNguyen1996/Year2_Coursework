#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

unsigned long fme(unsigned long g, unsigned long x,
                  unsigned long p)
{
  long d = g;
  long e = x;
  long s = 1;
  while (e != 0)
  {
    if (e % 2 == 1)
      s = (s*d) % p;
    d = (d*d) % p;
    e = e / 2;
  }
  return s;
}

/*--------------------------------------------------------------------------
 Part 3.
 The running time of my program will be O(p) this is because the function will
 run the for loop p times for any input of y,g or p. Thus the upper bound will
 be the number of iteration of the loop which will be p for the worst case.
 This function is a one-way function since it's easy to compute y but hard to
 invert the function to computed x. Therefore a^x mod p can be easily computed.
 But not x from a given values of a^x mod p, a and p.
*/
unsigned long dl(unsigned long y, unsigned long g, unsigned long p)
{
  unsigned long x = 1;
  for (int i = 1; i < p; i++)
  {
    if (fme(g,i,p) == y)
    {
      x = i;
      return x;
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  printf("--------------------------Part 3-----------------------\n");
  long part3_answer1 = dl(3, 7, 10);
  printf("3 = 7^%lu mod 10\n", part3_answer1);
  long part3_answer2 = dl(2, 2, 6);
  printf("2 = 2^%lu mod 6\n", part3_answer2);
  long part3_answer3 = dl(484, 670, 678);
  printf("484 = 670^%lu mod 678\n", part3_answer3);
}


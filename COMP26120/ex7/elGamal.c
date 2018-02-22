#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*--------------------------------------------------------------------------
 Part 1.
 2 long arrays to store the test values for a and b.
*/
long a[10] = {90,98,70,89,1200,20,80,10,69,120};
long b[10] = {19,10,8,15,9,600,89,90,100,69};
long gcd;

/*
Function to calculate the highest common factor of 2 integers using recursion.
It will have a run time complexity that is linear to the size of the input a.
Which is either 2 or 2log(a) depeding on the bigger one for input a, as we want
to find the worst case scenario. Therefore the complexity is O(max(2,2log(a))).

NOTE: This is the algorithm that was provided in the lecture. 
*/
unsigned long hcf(unsigned long a, unsigned long b)
{
  // Find the larger number between a and b if any.
  if (b > a)
  {
    long temp = a;
    a = b;
    b = temp;
  }

  if (b == 0)
    return a;
  else
  {
    int r = a % b;
    return hcf(b,r);
  }
}

/*--------------------------------------------------------------------------
Part 2.
The running time of the funtion is linear to the size of input x. This is
because the while loop will iterate log2(x) times for any g,x,p inputs,
Thus the running time of the program will be O(log2(x)) and independent of the
rest of the inputs; g or p.

NOTE: This is the algorithm that was provided in the lecture. 
*/
unsigned long fme(unsigned long g, unsigned long x,
                  unsigned long p)
{
  // Store values into temporary variables.
  long d = g;
  long e = x;
  long s = 1;

  // While loop to calculate the answer of g^x mod p
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
 Part 4.
 For this function I used the concept of Fermat’s Little theorem.
 Where (a^(m-2)*a) mod m = 1, if m is a prime. This is to reduce complexity for
 really big value of p. The running time of my function will be O(log(p)), this
 is because it will have the same complexity as function fme
*/
unsigned long imp(unsigned long y, unsigned long p)
{
  return fme(y, p-2, p);
}

/*--------------------------------------------------------------------------
 Part 5.
 Program to encrypt a given integer with respect to the hard coded values of
 prime number = 65537 and it's root = 3. The program will then print out the
 encrypted value in the form (a,b) based on the user's public key. If you input
 the form (a,b) and your private key, the program will decrypt the message.
*/
void encryption()
{
  char userInput[100];
  bool endGame = false;
  bool wrongInput = false;

  unsigned long a,b,k;
  unsigned long decryptPrivate, inverseAToX;
  unsigned long decryptA, decryptB, decryptSecretNumber;
  unsigned long primeModulus = 65537;
  unsigned long primitiveRoot = 3;
  unsigned long privateKey = 1;
  unsigned long publicKey = 69;
  unsigned long secretNumber;

  printf("Prime modulus is %d\nPrimitive root wrt %d is %d\n",
                                     primeModulus, primeModulus, primitiveRoot);

  printf("Choose: e (encrypt) | d (decrypt) |k (get public key) | x (exit)? ");
  fgets(userInput, 100, stdin);
  sscanf(userInput, "%[^\n]", userInput);

  while (!endGame)
  {
    if (strcmp(userInput, "x") == 0 && !endGame)
      endGame = true;
    else if (strcmp(userInput, "e") == 0 && !endGame)
    {
      printf("Type secret number to send: ");
      fgets(userInput, 100, stdin);
      sscanf(userInput, "%lu\n", &secretNumber);

      printf("Type recipient's public key: ");
      fgets(userInput, 100, stdin);
      sscanf(userInput, "%lu\n", &publicKey);

      // Generating random value of k within the correct range.
      srand(time(NULL));
      k = (unsigned long) (rand() % (65536 + 1 - 1)) + 1;

      // Calculating the values of a and b.
      a = fme(primitiveRoot, k, primeModulus);
      b = (fme(secretNumber, 1, primeModulus) *
                            fme(publicKey, k, primeModulus)) % primeModulus;
      printf("The encrypted secret is: (%lu,%lu)\n", a, b);
    }
    else if (strcmp(userInput, "d") == 0 && !endGame)
    {
      // Get user's a and b inputs.
      printf("Type in received message in form (a,b): ");
      fgets(userInput, 100, stdin);
      sscanf(userInput, "(%lu,%lu)", &decryptA, &decryptB);

      // Get user's private key.
      printf("Type in your private key: ");
      fgets(userInput, 100, stdin);
      sscanf(userInput, "%lu\n", &decryptPrivate);

      // Calculate the inverse modulo of a^x.
      inverseAToX = imp(fme(decryptA, decryptPrivate, primeModulus), primeModulus);

      // Calculate the secret message = (b * a^-x) mod p.
      decryptSecretNumber = (decryptB * inverseAToX) % primeModulus;
      printf("The secret number is: %lu\n", decryptSecretNumber);
    }
    else if (strcmp(userInput, "k") == 0 && !endGame)
    {
      printf("Type private key: ");
      fgets(userInput, 100, stdin);
      sscanf(userInput, "%lu\n", &privateKey);

      // Generate public key from user's private key.
      publicKey = fme(primitiveRoot, privateKey, primeModulus);
      printf("Public key is: %lu\n", publicKey);
    }
    // If the user input invalid selection.
    else
    {
      printf("Please only enter e, d, k or x as input.\n");
    }

    // Statement to get user input and go to the function accordingly.
    if (!endGame)
    {
      printf("Choose: e (encrypt) | d (decrypt) |k (get public key) | x (exit)? ");
      fgets(userInput, 100, stdin);
      sscanf(userInput, "%[^\n]", userInput);
    }
  }
}

int main(int argc, char **argv)
{
  // Test for part 1.
  printf("--------------------------Part 1-----------------------\n");
  for (int i = 0; i < 10; i++)
  {
    gcd = hcf(a[i],b[i]);
    printf("The greatest command divisor of %lu and %lu is %lu.\n",
           a[i], b[i], gcd);
  }

  // Test for part 2.
  printf("--------------------------Part 2-----------------------\n");
  long part2_answer1 = fme(7,11,10);
  long part2_answer2 = fme(2,5,6);
  printf("7^11 mod 10 is: %lu\n", part2_answer1);
  printf("2^5 mod 6 is: %lu\n", part2_answer2);

  // Test for part 4.
  printf("--------------------------Part 4-----------------------\n");
  long part4_answer1 = imp(9,80);
  printf("(%lu * %lu) mod 81\n",part4_answer1,9);

  // Test for part 5.
  printf("--------------------------Part 5-----------------------\n");
  encryption();
}


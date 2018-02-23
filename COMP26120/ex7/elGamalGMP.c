#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*--------------------------------------------------------------------------
 Part 1.
 Function to calculate the highest common factor of 2 integers using recursion.
*/
void hcf(mpz_t result, mpz_t a, mpz_t b)
{
  mpz_t tempValue, firstNumber, secondNumber;
  mpz_inits(tempValue, firstNumber, secondNumber, NULL);
  mpz_set(firstNumber, a);
  mpz_set(secondNumber, b);

  // Find the larger number between a and b if any.
  if (mpz_cmp(firstNumber, secondNumber) < 0)
  {
    mpz_set(firstNumber, b);
    mpz_set(secondNumber, a);
  }

  if (mpz_cmp_d(secondNumber,0) == 0)
  {
    mpz_set(result, firstNumber);
  }
  else
  {
    mpz_mod(tempValue, firstNumber, secondNumber);
    while (mpz_cmp_d(tempValue, 0) != 0)
    {
      mpz_set(tempValue, firstNumber);
      mpz_set(firstNumber, secondNumber);
      mpz_set(secondNumber, tempValue);
      mpz_mod(secondNumber, secondNumber, firstNumber);
      mpz_mod(tempValue, firstNumber, secondNumber);
    }

    mpz_set(result,secondNumber);
  }
}

//--------------------------------------------------------------------------
// Part 2.
void fme(mpz_t result, mpz_t g, mpz_t x, mpz_t p)
{
  // Store values into temporary variables.
  mpz_t divide,d,e,s,returnResult,tempValue;
  mpz_inits(divide,d,e,s,returnResult,tempValue, NULL);

  mpz_set(d, g);
  mpz_set(e, x);
  mpz_set_d(s, 1);

  mpz_set_d(divide, 2);

  // While loop to calculate the answer of g^x mod p
  while (mpz_cmp_d(e, 0) != 0)
  {
    mpz_mod_ui(tempValue, e, 2);
    if (mpz_cmp_d(tempValue, 1) == 0)
    {
      mpz_mul(tempValue, s, d);
      mpz_mod(s, tempValue, p);
    }
    mpz_mul(d, d, d);
    mpz_mod(d, d, p);
    mpz_div_ui(e, e, 2);
  }
  // Return the result
  mpz_set(result,s);
}


/*--------------------------------------------------------------------------
 Part 4.
 Using Fermat’s Little theorem. Where (a^(m-2)*a) mod m = 1, if m is a prime.
 this is to reduce complexity for really big p.
*/
void imp(mpz_t result, mpz_t y, mpz_t p)
{
  mpz_t tempValue, m;
  mpz_inits(tempValue, m, NULL);
  mpz_set(m,p);
  mpz_sub_ui(m,m,2);

  fme(tempValue, y, m, p);
  mpz_set(result, tempValue);
}

/*
--------------------------------------------------------------------------
 Part 5.
 Program to encrypt a given integer with respect to the hard coded values of
 prime number = 170141183460469231731687303715884105727 and it's root = 43.
 The program will then print out the encrypted value in the form (a,b) based on
 the user's public key. If you input the form (a,b) and your private key,
 the program will decrypt the message.
*/
void encryption()
{
  // Initialising all the requqired variables.
  char userInput[1000];
  bool endGame = false;

  // Initialse all the random number k with 696969 different values.
  gmp_randstate_t randomState;
  gmp_randinit_default(randomState);
  gmp_randseed_ui(randomState, 696969);

  // Setting up all the requqired gmp variables.
  mpz_t a,b,k,decryptPrivate,inverseAToX,decryptA,decryptB,decryptSecretNumber;
  mpz_t primeModulus,primitiveRoot,privateKey,publicKey,secretNumber,tempValue;
  mpz_inits(a,b,k,decryptPrivate,inverseAToX,decryptA,decryptB,tempValue,
            decryptSecretNumber, NULL);
  mpz_inits(primeModulus, primitiveRoot, privateKey,publicKey,secretNumber,NULL);

  mpz_init_set_str(primeModulus, "170141183460469231731687303715884105727", 10);
  mpz_set_d(primitiveRoot, 43);

  gmp_printf("Prime modulus is %Zd\nPrimitive root wrt %Zd is %Zd\n",
                                     primeModulus, primeModulus, primitiveRoot);

  printf("Choose: e (encrypt) | d (decrypt) |k (get public key) | x (exit)? ");
  fgets(userInput, 1000, stdin);
  sscanf(userInput, "%[^\n]", userInput);

  while (!endGame)
  {
    if (strcmp(userInput, "x") == 0 && !endGame)
      endGame = true;
    else if (strcmp(userInput, "e") == 0 && !endGame)
    {
      printf("Type secret number to send: ");
      fgets(userInput, 1000, stdin);
      gmp_sscanf(userInput, "%Zd\n", secretNumber);

      printf("Type recipient's public key: ");
      fgets(userInput, 1000, stdin);
      gmp_sscanf(userInput, "%Zd\n", publicKey);

      //Generating random value of k within the correct range.
      mpz_urandomm(k, randomState, primeModulus);
      gmp_printf("Test: %Zd\n",k);

      // Calculate the value of a.
      fme(a, primitiveRoot, k, primeModulus);

      // Calculate secretNumber %= primeModulus.
      mpz_mod(secretNumber, secretNumber, primeModulus);

      fme(tempValue, publicKey, k, primeModulus);
      mpz_mul(tempValue, secretNumber, tempValue);

      // Calculate the value of b
      mpz_mod(b, tempValue, primeModulus);

      gmp_printf("The encrypted secret is: (%Zd,%Zd)\n", a, b);
    }
    else if (strcmp(userInput, "d") == 0 && !endGame)
    {
      // Get user's a and b inputs.
      printf("Type in received message in form (a,b): ");
      fgets(userInput, 1000, stdin);
      gmp_sscanf(userInput, "(%Zd,%Zd)", decryptA, decryptB);  mpz_t a,b,k;

      // Get user's private key.
      printf("Type in your private key: ");
      fgets(userInput, 1000, stdin);
      gmp_sscanf(userInput, "%Zd\n", decryptPrivate);

      // Calculate the inverse modulo of decryptPrivate^x mod primeModulus.
      mpz_sub_ui(tempValue, primeModulus, 2);
      mpz_mul(decryptPrivate, decryptPrivate, tempValue);
      fme(inverseAToX, decryptA, decryptPrivate, primeModulus);

      // Calculate the secret message = (b * a^-x) mod p.
      mpz_mul(tempValue, decryptB, inverseAToX);
      mpz_mod(decryptSecretNumber, tempValue, primeModulus);
      gmp_printf("The secret number is: %Zd\n", decryptSecretNumber);

    }
    else if (strcmp(userInput, "k") == 0 && !endGame)
    {
      printf("Type private key: ");
      fgets(userInput, 1000, stdin);
      gmp_sscanf(userInput, "%Zd\n", privateKey);

      // Generate public key from user's private key.
      fme(publicKey, primitiveRoot, privateKey, primeModulus);
      gmp_printf("Public key is: %Zd\n", publicKey);
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
      fgets(userInput, 1000, stdin);
      sscanf(userInput, "%[^\n]", userInput);
    }
  }
}

int main(int argc, char **argv)
{
  // Test for part 6.
  printf("--------------------------Part 6-----------------------\n");
  encryption();
  return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>
#include <math.h>
#include "speller.h"
#include "dict.h"

typedef struct
{ // hash-table entry
  Key_Type element; // only data is the key itself
  enum {empty, in_use, deleted} state;
} cell;

typedef unsigned int Table_size; // type for size-of or index-into hash table

struct table
{
  cell *cells;
  Table_size table_size; // cell cells [table_size];
  Table_size num_entries; // number of cells in_use

  double totalCollision;
  double totalInsert;
  // add anything else that you need
};

// Function to initialise values in the table.
Table initialize_table(Table_size dimension)
{
  Table newTable = (Table) malloc(sizeof(Table));
  newTable->num_entries = 0;
  if (!isPrime(dimension) || dimension == 1)
    dimension = nextPrime(dimension);
  newTable->table_size = dimension;
  newTable->cells = malloc(sizeof(cell) * dimension);

  newTable->totalCollision = 0;
  return newTable;
}

//=============================================================================
// Functions to calculate the hash code of a key of type string.
int summation_hash(const char* key)
{
  int k = strlen(key);
  char charArray[k];
  strncpy(charArray, key, k);

  int answer = 0;

  for (int i = 0; i < k; i++)
  {
    answer += charArray[i];
  }

  return answer;
}

int polynomial_hash(const char* key)
{
  int k = strlen(key);
  char charArray[k];
  strncpy(charArray, key, k);

  int a = 37;
  int answer = 0;

  for (int i = 0; i < k; i++)
  {
    answer += (int) charArray[i] * pow(a,k-i-1);
  }

  return answer;
}

// Non zero hash function with formula f(k) = q - (k mod q)
// q = 7.
int nonZero_hash(int key)
{
  return 17 - (key % 17);
}

// Compression function that map the key to range 0 to N.
int compression(int hashKey, Table_size maxRange)
{
  int a = rand() % maxRange + 1;
  int b = rand() % maxRange + 1;
  return (int) (hashKey) % maxRange;
}

//=============================================================================
// Probing modes. Use open addressing.

// Function to insert a value into the table and use linear probing in order to
// deal with collisions.
Table linear_probing(Table table, int index, Key_Type value)
{
  int current = table->totalInsert + 1;
  // For loop to find the index to input the new value into.
  for (int i = index; i < table->table_size; i++)
  {
    if (table->cells[i].state == in_use &&
        strcmp(table->cells[i].element, value) == 0)
      return table;

    else if(table->cells[i].state == empty)
    {
      table->cells[i].state = in_use;
      table->cells[i].element = strdup(value);
      table->num_entries++;
      return table;
    }
    else
    {
      table->totalCollision++;
      table->totalInsert = current;
    }
  }
  return table;
}

Table quadratic_probing(Table table, int index, Key_Type value)
{
  int position;
  int current = table->totalInsert + 1;
  // For loop to find the index to input the new value into.
  for (int i = 0; i < table->table_size; i++)
  {
    position = (index + i * i) % table->table_size;
    if (table->cells[position].state == in_use &&
        strcmp(table->cells[position].element, value) == 0)
      return table;

    else if(table->cells[position].state == empty)
    {
      table->cells[position].state = in_use;
      table->cells[position].element = strdup(value);
      table->num_entries++;
      return table;
    }
    else
    {
      table->totalInsert = current;
      table->totalCollision++;
    }
  }
  return table;
}

Table double_hashing(Table table, int index, Key_Type value)
{
  int current = table->totalInsert + 1;
  int position;

  // For loop to find the index to input the new value into.
  for (int i = 0; i < table->table_size; i++)
  {
    position = (index + i * nonZero_hash(value)) % table->table_size;
    if (table->cells[position].state == in_use &&
        strcmp(table->cells[position].element, value) == 0)
      return table;

    else if(table->cells[position].state == empty)
    {
      table->cells[position].state = in_use;
      table->cells[position].element = strdup(value);
      table->num_entries++;
      return table;
    }
    else
    {
      table->totalInsert = current;
      table->totalCollision++;
    }
  }
  return table;
}

// Get the key of a given value in a table that used linear probing.
Boolean find_linear(Table table, int index, Key_Type value)
{
  // For loop to find the index to input the new value into.
  for (int i = index; i < table->table_size; i++)
  {
    if (table->cells[i].state == in_use &&
        strcmp(table->cells[i].element, value) == 0)
      return TRUE;
  }
  return FALSE;
}

// Get the key of a given value in a table that used quadratic probing.
Boolean find_quadratic(Table table, int index, Key_Type value)
{
  int position;

  // For loop to find the index to input the new value into.
  for (int i = 0; i < table->table_size; i++)
  {
    position = (index + i * i) % table->table_size;
    if (table->cells[position].state == in_use &&
        strcmp(table->cells[position].element, value) == 0)
      return TRUE;
  }
  return FALSE;
}

// Get the key of a given value in a table that used double hashing.
Boolean find_double(Table table, int index, Key_Type value)
{
  int position;

  // For loop to find the index to input the new value into.
  for (int i = 0; i < table->table_size; i++)
  {
    position = (index + i * nonZero_hash(value)) % table->table_size;
    if (table->cells[position].state == in_use &&
        strcmp(table->cells[position].element, value) == 0)
      return TRUE;
  }
  return FALSE;
}

//=============================================================================
// Get table hash ratio.
double get_ratio(Table table)
{
  return (double) table->num_entries / table->table_size;
}

// Rehash to a larger table of size N is prime.
Table rehashTable(Table table, int mode)
{
  // Setup a new table, then copy the data from the old table.
  Table newTable = (Table) malloc(sizeof(Table));

  newTable->table_size = nextPrime(table->table_size);
  newTable->cells = malloc(sizeof(cell) * nextPrime(table->table_size));

  printf("\n=============================================================\n");
  printf("Rehashing the table into size of: %d\n", newTable->table_size);

  for (int i = 0; i < table->table_size; i++)
  {
    if (table->cells[i].state == in_use)
    {
      newTable = insert(table->cells[i].element, newTable);
    }
  }

  newTable->num_entries = table->num_entries;
  newTable->totalCollision = table->totalCollision;
  newTable->totalInsert = table->totalInsert;

  return newTable;
}

// Get the next prime of a given number.
int nextPrime(int n)
{
    int prime = n*2;
    while (!isPrime(prime)) prime += 1;
    return prime;
}

// Check if a number is a prime.
int isPrime(int n)
{
    int i, startIndex;

    // Check if the number is divisible by 2 or 3.
    if (n % 2 == 0 || n % 3 == 0)
      return FALSE;

    startIndex = (int) sqrt(n);

    for (i = 5; i <= startIndex; i += 6)
    {
        if (n % i == 0)
           return FALSE;
    }

    for (i = 7; i <= startIndex; i += 6)
    {
        if (n % i == 0)
           return FALSE;
    }

    // If not divisible by anything return true.
    return TRUE;
}
//=============================================================================
// Function to insert key, value pair into a table.
Table insert(Key_Type insertKey, Table insertTable)
{
  // Initialise the required variables.
  int hashID, hashCode;

  // Check if the size of the table is too small, if so rehash.
  if ((insertTable->num_entries != 0 && get_ratio(insertTable) > 0.5)
      || get_ratio(insertTable) == 1)
  {
    insertTable = rehashTable(insertTable, mode);
  }

  switch(mode)
  {
    // Mode 1: Used only summation hash function and use linear probing.
    case 1:
      hashCode = summation_hash(insertKey);
      hashID = compression(hashCode, insertTable->table_size);
      return linear_probing(insertTable, hashID, insertKey);
      break;

    // Mode 2: Used summation hash function and use quadratic probing.
    case 2:
      hashCode = summation_hash(insertKey);
      hashID = compression(hashCode, insertTable->table_size);
      return quadratic_probing(insertTable, hashID, insertKey);
      break;

    // Mode 3: Used polynomial hash function and use linear probing.
    case 3:
      hashCode = polynomial_hash(insertKey);
      hashID = compression(hashCode, insertTable->table_size);
      return linear_probing(insertTable, hashID, insertKey);
      break;

    // Mode 4: Used polynomial hash function and use quadratic probing.
    case 4:
      hashCode = polynomial_hash(insertKey);
      hashID = compression(hashCode, insertTable->table_size);
      return quadratic_probing(insertTable, hashID, insertKey);
      break;

    // Mode default: Used polynomial hash and double hashing.
    default:
      hashCode = polynomial_hash(insertKey);
      hashID = compression(hashCode, insertTable->table_size);
      return double_hashing(insertTable, hashID, insertKey);
      break;
  }
}

// Find a key in the table, if in use then true, else false.
// This function has a time complexity of O(N) where N is the number of elements
// in the bucket.
Boolean find(Key_Type findKey, Table findTable)
{
  // Initialise the required variables.
  int hashID, hashCode;

  switch(mode)
  {
    // Mode 1: Used only summation hash function and use linear probing.
    case 1:
      hashCode = summation_hash(findKey);
      hashID = compression(hashCode, findTable->table_size);
      return find_linear(findTable, hashID, findKey);
      break;

    // Mode 2: Used summation hash function and use quadratic probing.
    case 2:
      hashCode = summation_hash(findKey);
      hashID = compression(hashCode, findTable->table_size);
      return find_quadratic(findTable, hashID, findKey);
      break;

    // Mode 3: Used polynomial hash function and use linear probing.
    case 3:
      hashCode = polynomial_hash(findKey);
      hashID = compression(hashCode, findTable->table_size);
      return find_linear(findTable, hashID, findKey);
      break;

    // Mode 4: Used polynomial hash function and use quadratic probing.
    case 4:
      hashCode = polynomial_hash(findKey);
      hashID = compression(hashCode, findTable->table_size);
      return find_quadratic(findTable, hashID, findKey);
      break;

    // Mode default: Used polynomial hash and double hashing.
    default:
      hashCode = polynomial_hash(findKey);
      hashID = compression(hashCode, findTable->table_size);
      return find_double(findTable, hashID, findKey);
      break;
  }
}

// Find all the in use cells in the table.
void print_table(Table printTable)
{
  for (int i = 0; i < printTable->table_size; i++)
  {
    if (printTable->cells[i].state == in_use)
    {
      printf("Hash ID: %d, Value: %s\n", i ,printTable->cells[i].element);
    }
  }
}

// Function to print out any stat related to the table.
void print_stats(Table statsTable)
{
  printf("Total number of collisions: %.0lf\n", statsTable->totalCollision);
  printf("Total number of inserts with collisions: %.0lf\n", statsTable->totalInsert);
  double collisionsRate = statsTable->totalCollision / statsTable->totalInsert;
  printf("Collisions per insert: %lf\n\n", collisionsRate);

  printf("Total number of entries: %d\n", statsTable->num_entries);
  printf("Current bucket load: %f\n", get_ratio(statsTable));
}

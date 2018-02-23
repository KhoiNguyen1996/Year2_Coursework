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
  // add anything else that you need
};

// Function to initialise values in the table.
Table initialize_table(Table_size dimension)
{
  Table newTable = malloc(sizeof(Table));
  newTable->num_entries = 0;
  newTable->table_size = dimension;
  newTable->cells = (cell*) malloc(sizeof(cell) * dimension);

  return newTable;
}

// Function to calculate the hash code of a key of type string.
static int polynomial_hash(const char* key)
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

static int compression(int hashKey, Table_size maxRange)
{
  return (int) abs(hashKey * 9 + 3) % maxRange;
}

static int quadratic_probing(int i, int j, int N)
{
  return (i + j * j) % N;
}

Table insert(Key_Type insertKey, Table insertTable)
{
  int hashCode = polynomial_hash(insertKey);
  int hashID = compression(hashCode, insertTable->table_size);

  for (int i = 0; i < insertTable->table_size; i++)
  {
    if (insertTable->cells[quadratic_probing(hashID, i, insertTable->table_size)].state == in_use &&
        strcmp(insertTable->cells[quadratic_probing(hashID, i, insertTable->table_size)].element, insertKey) == 0)
      return insertTable;

    else if(insertTable->cells[quadratic_probing(hashID, i, insertTable->table_size)].state == empty)
    {
      insertTable->cells[quadratic_probing(hashID, i, insertTable->table_size)].state = in_use;
      insertTable->cells[quadratic_probing(hashID, i, insertTable->table_size)].element = strdup(insertKey);
      return insertTable;
    }
  }
  return insertTable;
}

// Find a key in the table, if in use then true, else false.
Boolean find(Key_Type findKey, Table findTable)
{
  int hashCode = polynomial_hash(findKey);
  int hashID = compression(hashCode, findTable->table_size);
  for (int i = 0; i < findTable->table_size; i++)
  {
    if(findTable->cells[quadratic_probing(hashID, i, findTable->table_size)].state == in_use &&
       strcmp(findTable->cells[quadratic_probing(hashID, i, findTable->table_size)].element, findKey) == 0)
       return 1;
  }
  return 0;
}

void print_table(Table printTable)
{
  for (int i = 0; i < printTable->table_size; i++)
  {
    if (printTable->cells[i].element != NULL && printTable->cells[i].state == in_use)
    {
      printf("Hash ID: %d, Value: %s\n", i, printTable->cells[i].element);
    }
  }
}

void print_stats(Table statsTable)
{
}

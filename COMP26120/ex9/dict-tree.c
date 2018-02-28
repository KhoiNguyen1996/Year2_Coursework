#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>

#include "speller.h"
#include "dict.h"

typedef struct node *tree_ptr;
struct node
{
  Key_Type element; // only data is the key itself
  tree_ptr left, right;
  // add anything else that you need
};

struct table
{
  tree_ptr head; // points to the head of the tree

  // Values to store the stats of the table.
  int maxDepth;
  int noOfNode;
  int noOfCompare_insert;
  int noOfCompare_find;
  int noOfInsert;
  int noOfFind;
};

// Function to initialise a new table with NULL head pointer.
Table initialize_table(/*ignore parameter*/)
{
  Table newTable = (Table) malloc(sizeof(Table));

  // Initialise the required values in the table.
  newTable->head = NULL;
  newTable->maxDepth = 0;
  newTable->noOfNode = 0;
  newTable->noOfCompare_insert = 0;
  newTable->noOfCompare_find = 0;
  newTable->noOfInsert = 0;
  newTable->noOfFind = 0;

  return newTable;
}

// Function to insert a value into the table,
// with respect to the order of the tree.
Table insert(Key_Type insertKey, Table insertTable)
{
  // Increase no of method call.
  insertTable->noOfInsert++;

  // A new node to insert into the tree structure, if doesn't exist.
  tree_ptr insertNode = (tree_ptr) malloc(sizeof(tree_ptr));
  insertNode->element = strdup(insertKey);
  insertNode->left = NULL;
  insertNode->right = NULL;

  // Check if the current table is empty.
  if (insertTable->head == NULL)
  {
    // If it's empty, then add a new node.
    insertTable->head = insertNode;
    insertTable->noOfNode++;
    return insertTable;
  }
  else
  {
    // Get the root node of the table for the root node.
    tree_ptr rootNode = insertTable->head;

    // Make the node for the current traversal node.
    tree_ptr traversalNode = rootNode;

    // Check if the key already existed.
    Boolean isExisted = find(insertKey, insertTable);

    // Insert new node into the tree using preorder traversal.
    while (!isExisted)
    {
      rootNode = traversalNode;
      // Left insert.
      if (strcmp(insertKey, traversalNode->element) < 0)
      {
        insertTable->noOfCompare_insert++;
        traversalNode = traversalNode->left;

        if (traversalNode == NULL)
        {
          rootNode->left = insertNode;
          insertTable->noOfNode++;

          return insertTable;
        }
      }
      // Right insert.
      else if (strcmp(insertKey, traversalNode->element) > 0)
      {
        insertTable->noOfCompare_insert++;
        traversalNode = traversalNode->right;

        if (traversalNode == NULL)
        {
          rootNode->right = insertNode;
          insertTable->noOfNode++;

          return insertTable;
        }
      }
      // No insert if value already existed.
      else
      {
        insertTable->noOfCompare_insert++;
        return insertTable;
      }
    }
  }
  return insertTable;
}

// This function has a time complexity of O(N) for non self-balance tree.
// It's has a time complexity of O(log N) for self-balance tree.
// N is the total number of node currently in the table.
Boolean find(Key_Type findKey, Table findTable)
{
  // Increase no of method call.
  findTable->noOfFind++;

  tree_ptr headNode = findTable->head;

  // Keep searching for a node with equivalence value.
  while (strcmp(headNode->element, findKey) != 0)
  {
    // Increase the number of compare.
    findTable->noOfCompare_find++;

    if (strcmp(headNode->element, findKey) > 0)
    {
      headNode = headNode->left;
      findTable->noOfCompare_find++;
    }
    else if (strcmp(headNode->element, findKey) < 0)
    {
      headNode = headNode->right;
      findTable->noOfCompare_find++;
    }
    // If the head reach NULL then the value doesn't exist.
    if (headNode == NULL)
      return FALSE;
  }
  // If the value exists.
  return TRUE;
}

// Print the table using preorder traversal using recursion.
void print_table(Table printTable)
{
  tree_ptr rootNode = (tree_ptr) malloc(sizeof(tree_ptr));

  // Get root node of the table if not null.
  if (printTable->head != NULL)
  {
    rootNode = printTable->head;
  }

  // Preorder recursive calls.
  if (rootNode->element != NULL)
  {
    printf("%s\n", rootNode->element);

    // Print the left tree first.
    Table leftTable = (Table) malloc(sizeof(Table));
    leftTable->head = rootNode->left;
    print_table(leftTable);

    // Then print the right tree.
    Table rightTable = (Table) malloc(sizeof(Table));
    rightTable->head = rootNode->right;
    print_table(rightTable);
  }
}

// Function to find the maximum depth of a table.
int maxDepth(Table depthTable)
{
  tree_ptr head = depthTable->head;

  // If there is no root node, height is zero.
  if (head == NULL)
    return 0;
  else
  {
    // Calculate the max height of the left side.
    Table leftTable = (Table) malloc(sizeof(Table));
    leftTable->head = head->left;
    int leftDepth = maxDepth(leftTable);

    // Calculate the max height of the right side.
    Table rightTable = (Table) malloc(sizeof(Table));
    rightTable->head = head->right;
    int rightDepth = maxDepth(rightTable);

    // Pick the maximum value of left or right.
    if (leftDepth > rightDepth)
      return leftDepth + 1;
    else
      return rightDepth + 1;
  }
}

// Function to print performance stat of the table.
void print_stats(Table statsTable)
{
  // Print find stats
  printf("=======================================================\n");
  printf("Total number of find calls: %d\n", statsTable->noOfFind);
  printf("Total number of compares in find call: %d\n",
          statsTable->noOfCompare_find);

  double findRatio = (double) statsTable->noOfCompare_find / statsTable->noOfFind;
  printf("Number of compare per find: %lf\n", findRatio);

  // Print insert stats.
  printf("=======================================================\n");
  printf("Total number of insert calls: %d\n", statsTable->noOfInsert);
  printf("Total number of compare in insert call: %d\n",
         statsTable->noOfCompare_insert);

  double insertRatio = (double) statsTable->noOfCompare_insert / statsTable->noOfInsert;
  printf("Number of insert per find: %lf\n", insertRatio);

  // Print other stats
  printf("=======================================================\n");
  printf("Total number of node: %d\n", statsTable->noOfNode);
  printf("Max depth: %d\n", maxDepth(statsTable));
}

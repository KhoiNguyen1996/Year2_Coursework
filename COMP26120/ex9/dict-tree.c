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
  int maxDepth;
  int noOfNode;
  int noOfCompare_insert;
  int noOfCompare_find;
  int noOfInsert;
  int noOfFind;
  // add anything else that you need
};

// Function to initialise a new table with NULL head pointer.
Table initialize_table(/*ignore parameter*/)
{
  Table newTable = (Table) malloc(sizeof(Table));
  newTable->head = NULL;
  newTable->maxDepth = 0;
  return newTable;
}

// Function to insert a value into the table,
// with respect to the order of the tree.
Table insert(Key_Type insertKey, Table insertTable)
{
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
    return insertTable;
  }
  else
  {
    // Get the root node of the table for the root node.
    tree_ptr rootNode = insertTable->head;

    // Make the node for the current traversal node.
    tree_ptr traversalNode = rootNode;

    // Insert new node into the tree using preorder traversal.
    while (1)
    {
      rootNode = traversalNode;
      // Left insert.
      if (strcmp(insertKey, traversalNode->element) < 0)
      {
        traversalNode = traversalNode->left;
        if (traversalNode == NULL)
        {
          rootNode->left = insertNode;
          return insertTable;
        }
      }
      // Right insert.
      else if (strcmp(insertKey, traversalNode->element) > 0)
      {
        traversalNode = traversalNode->right;

        if (traversalNode == NULL)
        {
          rootNode->right = insertNode;
          return insertTable;
        }
      }
      // No insert if value already existed.
      else
        return insertTable;
    }
  }
  return insertTable;
}

Boolean find(Key_Type findKey, Table findTable)
{
  tree_ptr headNode = findTable->head;

  while (strcmp(headNode->element, findKey) != 0)
  {
    if (strcmp(headNode->element, findKey) > 0)
      headNode = headNode->left;
    else if (strcmp(headNode->element, findKey) < 0)
      headNode = headNode->right;

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

/*
you might calculate the height and/or the average number of string compares per
call to insert or find, so you can compare them with the theoretical minimum.
You should add fields to the data structures in the code you are given to
collect the information you need.
*/
void print_stats(Table statsTable)
{
}

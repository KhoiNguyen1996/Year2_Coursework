#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Node to contain the data structure of the game as a tree.
typedef struct Node
{
  char *objectName;
  char *questionData;

  struct Node *yes_ptr;
  struct Node *no_ptr;
} node;

// Function to print out the current node from a given data structure node.
void nodePrint(node *currentNode)
{
  if (currentNode != NULL)
  {
    if (currentNode->objectName == NULL)
    {
      printf("Object: [NOTHING]\n");
      printf("Question: %s\n", currentNode->questionData);

      if (currentNode->yes_ptr->objectName == NULL)
        printf("Yes: %s\n", currentNode->yes_ptr->questionData);
      else
        printf("Yes: %s\n", currentNode->yes_ptr->objectName);

      if (currentNode->no_ptr->objectName == NULL)
        printf("No: %s\n", currentNode->no_ptr->questionData);
      else
        printf("No: %s\n", currentNode->no_ptr->objectName);
    }
    else
    {
      printf("Object: %s\n", currentNode->objectName);
      printf("Question: [NOTHING]\n");
    }
  }
}

// Function to print out the whole tree data structure of the game.
void treePrint(node *currentNode)
{
  if (currentNode == NULL);
  else
  {
    if (currentNode->objectName == NULL)
    {
      printf("Question: %s\n", currentNode->questionData);
      treePrint(currentNode->yes_ptr);
      treePrint(currentNode->no_ptr);
    }
    else
    {
      printf("Object: %s\n", currentNode->objectName);
    }
  }
}

// Function to make starting tree for new game.
struct node* makeStartingTree()
{
  node *curr_ptr = (node*) malloc(sizeof(node));
  char *startingObject = "a pangolin";
  curr_ptr->objectName = startingObject;
  return curr_ptr;
}

// Check for yes no response.
void yesNoAnswer(char userResponse[])
{
  while (strcmp(userResponse,"yes") != 0 && strcmp(userResponse,"no") != 0)
  {
    printf("Please only enter as yes or no only.\n");
    fgets(userResponse, 5, stdin);
    sscanf(userResponse, "%s", userResponse);
  }
}

// Check for new insert question from the user.
void checkNewQuestion(char userResponse[])
{
  bool hasQuestion = (userResponse[strlen(userResponse) - 1] == '?');
  while (!hasQuestion)
  {
    printf("Please only enter with ? at the end.\n");
    fgets(userResponse, 100, stdin);
    sscanf(userResponse, "%[^\n]", userResponse);
    hasQuestion = (userResponse[strlen(userResponse) - 1] == '?');
  }
}

// Check for new insert object from the user.
void checkNewObject(char userResponse[])
{
  bool isA = (userResponse[0] == 'a' && userResponse[1] == ' ');
  bool isAn = (userResponse[0] == 'a' && userResponse[1] == 'n'
               && userResponse[2] == ' ');
  bool isUpper = isupper(userResponse[0]);
  while (!isA && !isAn && !isUpper)
  {
    printf("Please only enter with format a/an object or Name.\n");
    fgets(userResponse, 100, stdin);
    sscanf(userResponse, "%[^\n]", userResponse);
    isA = (userResponse[0] == 'a' && userResponse[1] == ' ');
    isAn = (userResponse[0] == 'a' && userResponse[1] == 'n'
                 && userResponse[2] == ' ');
    isUpper = isupper(userResponse[0]);

  }
}

// Load the tree structure from a save file.
node* treeLoad(FILE *openFile)
{
  char line[200];
  char objectQuestion[100];

  fgets(line, 200, openFile);
  if (line == NULL)
  {
    return NULL;
  }
  else
  {
    sscanf(line, "%s %[^\n]", objectQuestion, line);
    node *new_ptr = (node*) malloc(sizeof(node));

    if (strcmp(objectQuestion, "Question:") == 0)
    {
      char *question = (char*) malloc(strlen(line));
      strcpy(question, line);
      new_ptr->questionData = question;
      new_ptr->yes_ptr = treeLoad(openFile);
      new_ptr->no_ptr = treeLoad(openFile);
    }
    else
    {
      char *object = (char*) malloc(strlen(line));
      strcpy(object, line);
      new_ptr->objectName = object;
      new_ptr->yes_ptr = NULL;
      new_ptr->no_ptr = NULL;
    }
  }
}

// Function to save the whole tree data structure of the game.
void treeSave(node *currentNode , FILE *openFile)
{
  if (currentNode == NULL);
  else
  {
    if (currentNode->objectName == NULL)
    {
      fprintf(openFile, "Question: %s\n", currentNode->questionData);
      treeSave(currentNode->yes_ptr, openFile);
      treeSave(currentNode->no_ptr, openFile);
    }
    else
    {
      fprintf(openFile, "Object: %s\n", currentNode->objectName);
    }
  }
}

// Function to free the whole tree data structure of the game.
void treeClear(node *currentNode)
{
  if (currentNode == NULL);
  else
  {
    if (currentNode->objectName == NULL)
    {
      free(currentNode->questionData);
      free(currentNode);
      treeClear(currentNode->yes_ptr);
      treeClear(currentNode->no_ptr);
    }
    else
    {
      free(currentNode->objectName);
      free(currentNode);
    }
  }
}

int main(int argc, char **argv)
{
  FILE *openFile;
  openFile = fopen("savefile", "r");

  bool isFirst = true;
  bool isSecond = false;
  bool replay = true;
  bool returnRoot = true;

  char yesNo[5];
  char willReplay[5];
  char saveEnabled[5];
  char loadEnabled[5];

  printf("Would you like to start from a save?\n");
  fgets(loadEnabled, 5, stdin);
  sscanf(loadEnabled, "%s", loadEnabled);
  yesNoAnswer(loadEnabled);
  node *starting_ptr;

  if (strcmp(loadEnabled, "yes") == 0)
  {
    starting_ptr = treeLoad(openFile);
    isFirst = false;
    isSecond = true;
  }
  else
  {
    starting_ptr = makeStartingTree();
  }

  node *curr_ptr = starting_ptr;
  node *head_ptr = starting_ptr;

  while (replay)
  {
    if(!isFirst && returnRoot)
    {
      head_ptr = starting_ptr;
      returnRoot = false;
    }

    printf("OK, please think of something\n");

    bool gameContinue = true;
    while (gameContinue)
    {
      if (starting_ptr->yes_ptr == NULL && starting_ptr->no_ptr == NULL)
      {
        char userAnswer[5];

        printf("Is it %s? \n", starting_ptr->objectName);
        fgets(userAnswer, 5, stdin);
        sscanf(userAnswer, "%s", userAnswer);
        yesNoAnswer(userAnswer);

        if (strcmp(userAnswer,"yes") == 0)
        {
          printf("Good. That was soooo easy.\n");
        }
        else
        {
          char thinkingObject[100];
          char userQuestion[100];
          char isYes[5];

          printf("Oh. Well you win then -- What were you thinking of?\n");
          fgets(thinkingObject, 100, stdin);
          sscanf(thinkingObject, "%[^\n]", thinkingObject);
          checkNewObject(thinkingObject);

          printf("Please give me a question about %s, so I can tell the difference between %s and %s\n",
            thinkingObject, thinkingObject, starting_ptr->objectName);
          fgets(userQuestion, 100, stdin);
          sscanf(userQuestion, "%[^\n]", userQuestion);
          checkNewQuestion(userQuestion);

          printf("What is the answer for %s?\n", thinkingObject);
          fgets(isYes, 5, stdin);
          sscanf(isYes, "%[^\n]", isYes);
          yesNoAnswer(isYes);

          // New strings for new question, new and old objects.
          char *newQuestion = (char*) malloc(strlen(userQuestion));
          strcpy(newQuestion,userQuestion);
          starting_ptr->questionData = newQuestion;

          node *new_ptr = (node*) malloc(sizeof(node));
          char *newObject = (char*) malloc(strlen(thinkingObject));
          strcpy(newObject,thinkingObject);
          new_ptr->objectName = newObject;

          node *old_ptr = (node*) malloc(sizeof(node));
          char *oldObject = (char*) malloc(strlen(starting_ptr->objectName));
          strcpy(oldObject,starting_ptr->objectName);
          old_ptr->objectName = oldObject;

          starting_ptr->objectName = NULL;
          isFirst = false;

          // Insert new nodes into the data structure.
          if (strcmp(isYes,"yes") == 0)
          {
            starting_ptr->yes_ptr = new_ptr;
            starting_ptr->no_ptr = old_ptr;
          }
          else
          {
            starting_ptr->yes_ptr = old_ptr;
            starting_ptr->no_ptr = new_ptr;
          }

          if (!isFirst && strcmp(yesNo, "yes") == 0)
          {
            curr_ptr->yes_ptr = starting_ptr;
          }
          else if (!isFirst && strcmp(yesNo, "no") == 0)
          {
            curr_ptr->no_ptr = starting_ptr;
          }
        }
        gameContinue = false;
      }
      else
      {
        // Print out the current node's question/
        printf("%s \n", starting_ptr->questionData);

        fgets(yesNo, 5, stdin);
        sscanf(yesNo, "%s", yesNo);
        yesNoAnswer(yesNo);

        if (strcmp(yesNo, "yes") == 0)
        {
          curr_ptr = starting_ptr;
          starting_ptr = starting_ptr->yes_ptr;
        }
        else if (strcmp(yesNo, "no") == 0)
        {
          curr_ptr = starting_ptr;
          starting_ptr = starting_ptr->no_ptr;
        }
      }
    }

    if(!isFirst && !returnRoot)
    {
      starting_ptr = head_ptr;
      returnRoot = true;
    }

    printf("Wanna try again?\n");
    fgets(willReplay, 5, stdin);
    sscanf(willReplay, "%s", willReplay);
    yesNoAnswer(willReplay);

    if (strcmp(willReplay,"no") == 0)
    {
      replay = false;
    }

    if (replay == false)
    {
      printf("Would you like to save your current progress?\n");
      fgets(saveEnabled, 5, stdin);
      sscanf(saveEnabled, "%s", saveEnabled);
      yesNoAnswer(saveEnabled);

      if (strcmp(saveEnabled, "yes") == 0)
      {
        fclose(openFile);
        FILE *openFile;
        openFile = fopen("savefile", "w");
        treeSave(head_ptr, openFile);
      }
    }
  }
  fclose(openFile);
  // Free memory from the data structure.
  treeClear(head_ptr);
}



#include <stdlib.h>
#include <stdio.h>
#include <math.h>

FILE *fp;

typedef struct book
{
  double rating;
  double price;
  double relevance;
  int ID;
} B;

B *list;

int read_file(char *infile, int N)
{
  int c;
  if((fp=fopen(infile, "rb")))
    {
      fscanf(fp, "%*s\t%*s\t%*s\t%*s\n");
      c=0;
      while((!feof(fp))&&(c<N))
	{
	  fscanf(fp, "%lf\t%lf\t%lf\t%d\n", &list[c].rating,  &list[c].price, &list[c].relevance, &list[c].ID);
	  c++;
	}
      fclose(fp);
    }
      else
    {
      fprintf(stderr,"%s did not open. Exiting.\n",infile);
      exit(-1);
    }
  return(c);
}

int comp_on_rating(const void *a, const void *b)
{
  if ((*(B *)a).rating < (*(B *)b).rating)
  {
     return -1;
  }
    else
  {
    if ((*(B *)a).rating > (*(B *)b).rating)
    {
      return 1;
    }
      else
    {
      return 0;
    }
  }
}

int comp_on_relev(const void *a, const void *b)
{

  if ((*(B *)a).relevance < (*(B *)b).relevance)
  {
     return -1;
  }
     else
  {
     if ((*(B *)a).relevance > (*(B *)b).relevance)
     {
       return 1;
     }
       else
     {
       return 0;
     }
  }
}

int comp_on_price(const void *a, const void *b)
{

  if ((*(B *)a).price < (*(B *)b).price)
  {
     return 1;
  }
     else
  {
     if ((*(B *)a).price > (*(B *)b).price)
     {
       return -1;
     }
       else
     {
       return 0;
     }
  }
}

// The merge sort were implemented by an example from the website geeksforgeeks.org.
void mergeValues(int lowValue, int midValue, int highValue, int(*compare_value)(const void *, const void *))
{
  int i, j, k;
  int leftArraySize = (midValue - lowValue) + 1;
  int rightArraySize = highValue - midValue;

  // Initialised the 2 book arrays in order to store all the books.
  B leftArray[leftArraySize];
  B rightArray[rightArraySize];

  // 2 for loops to copy data to the 2 temporary book arrays.
  for (i = 0; i < leftArraySize; i++)
    leftArray[i] = list[lowValue + i];
  for (j = 0; j < rightArraySize; j++)
    rightArray[j] = list[midValue + j + 1];

  // Reinitialising the starting index of the subarrays.
  i = 0;
  j = 0;
  k = lowValue;

  // For loop to merge the sub arrays into the main array.
  while (i < leftArraySize && j < rightArraySize)
  {
    if ((compare_value)(&leftArray[i], &rightArray[j]) <= 0)
    {
      list[k] = leftArray[i];
      i++;
    }
    else
    {
      list[k] = rightArray[j];
      j++;
    }
    k++;
  }

  // 2 while loops to deal with any left over value in left or right array.
  while (i < leftArraySize)
  {
    list[k] = leftArray[i];
    i++;
    k++;
  }

  while (j < rightArraySize)
  {
    list[k] = rightArray[j];
    j++;
    k++;
  }
}

void merge_sort(int lowValue, int highValue, int(*compare_value)(const void *, const void *))
{
  int midValue;
  if (lowValue < highValue)
  {
    midValue = (lowValue + highValue) / 2;
    merge_sort(lowValue, midValue, (compare_value));
    merge_sort(midValue + 1, highValue, (compare_value));
    mergeValues(lowValue, midValue, highValue, (compare_value));
  }
}

// Function to sort the list with respect to user response.
void sortField(int N, char chosenField[])
{
  if (strcmp(chosenField, "price") == 0)
    merge_sort(0, N-1, comp_on_price);
  else if (strcmp(chosenField, "relevance") == 0)
    merge_sort(0, N-1, comp_on_relev);
  else if (strcmp(chosenField, "rating") == 0)
    merge_sort(0, N-1, comp_on_rating);
  else
    printf("You entered an invalid sort field. \n");
}

// Function to sort a given list using merge sort.
void user_interface(int N)
{
  // For Part 1 this function calls the sort function to sort on Price only
  // merge_sort(0, N-1, comp_on_price);


  // For Part 2 this function
  // (1) asks the user if they would like to sort their search results
  // (2) asks for the most important field (or key), the next most etc
  // (3) calls your sort function
  char userReply[100];
  char userFirstField[100];
  char userSecondField[100];
  char userLastField[100];

  printf("Do you want to sort your search results?\n");
  fgets(userReply, 100, stdin);
  sscanf(userReply, "%[^\n]", userReply);

  // If statement to check if the user want a sorted search.
  if (strcmp(userReply, "yes") == 0)
  {
    printf("What is your most important field?\n");
    fgets(userReply, 100, stdin);
    sscanf(userReply, "%[^\n]", userFirstField);

    printf("What is your second most important field?\n");
    fgets(userReply, 100, stdin);
    sscanf(userReply, "%[^\n]", userSecondField);

    printf("What is your least important field?\n");
    fgets(userReply, 100, stdin);
    sscanf(userReply, "%[^\n]", userLastField);

    // Sort the list with the order of least, second then first as it's a
    // merge sort.
    sortField(N, userLastField);
    sortField(N, userSecondField);
    sortField(N, userFirstField);
  }
}


void print_results(int N)
{
    int i;
    if((fp=fopen("top20.txt","w")))
    {
      for(i=N-1;i>=N-20;i--)
      {
	  printf("%g %g %g %d\n", list[i].rating, list[i].price, list[i].relevance, list[i].ID);
	  fprintf(fp, "%g %g %g %d\n", list[i].rating, list[i].price, list[i].relevance, list[i].ID);

      }
      fclose(fp);
    }
      else
    {
      fprintf(stderr,"Trouble opening output file top20.txt\n");
      exit(-1);
    }

}


int main(int argc, char *argv[])
{
  int N;

  if(argc!=3)
    {
      fprintf(stderr, "./exec <input_size> <filename>\n");
      exit(-1);
    }

  N=atoi(argv[1]);

  list = (B *)malloc(N*sizeof(B));

  N=read_file(argv[2], N);

  user_interface(N);

  print_results(N);

  return(0);
}

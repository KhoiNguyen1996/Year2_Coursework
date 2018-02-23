#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Function to convert a given value of Celsius temperature to Fahrenheit.
float c2f(float c)
{
  float temperatureF;
  temperatureF = (9 * c / 5) + 32;
  return temperatureF;
}

// Function to convert a given value of Fahrenheit temperature to Celsius.
float f2c(float f) 
{
  float temperatureC;
  temperatureC = (f - 32) * 5 / 9;
  return temperatureC;
}

int main (int argc, char *argv[])
{
  // If statements to check for correct parameters.
  if (argc != 3)
  {
    printf("Please enter exactly 3 arguments.\n");
    exit(-1);
  }
  else if (strcmp(argv[1],"-f") != 0 && strcmp(argv[1],"-c") != 0)
  {
    printf("Please enter the correct first parameter -f or -c.\n");
    exit(-1);
  }

  float temperature;
  sscanf(argv[2],"%f",&temperature);

  // If statements to check for input temperature not below absolute zero.
  if (temperature < -273.15 && strcmp(argv[1],"-c") == 0)
  {
    printf("Please input Celsius temperature that is not below -273.15.\n");
    exit(-1);
  } 
  else if (temperature < -459.67 && strcmp(argv[1],"-f") == 0)
  {
    printf("Please input Fahrenheit temperature that is not below -459.67.\n");
    exit(-1);
  }	

  // If-else statement to print out the correct conversion result.
  if (strcmp(argv[1],"-f") == 0)
  {
    temperature = f2c(temperature);
    printf("%s°F = %f°C\n",argv[2],temperature);
  }
  else
  {
    temperature = c2f(temperature);
    printf("%f°F = %s°C\n",temperature,argv[2]);
  }

  return 0;
}

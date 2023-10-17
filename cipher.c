/*Name: Suman Kafle
Class cs241
Lab -06
*/

/* Project description: I have made 5 funtion. first function to return the character that need to ne decrpyted or encrypted. 
2nd function to return the unsigned long value of m. 3rd function to return the unsigned long value of c. 4th function to encrpyt the message
and  the last fucntion to decrypt the message. This class contain the header file named as lcg.h*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lcg.h"

int lineNumber = 1;

/* fucntion that take pointer as parameter and reutn the pointer  */
char* bytegetter( char *str)
{
  int counter = 0;
  int count1;
  int i;

  /* to go till the end of the string  */

  for (i = 0; i < strlen(str); i++)
    {

      if (str[i]== ',')
	{
	  counter++;
	  /* to check if there are two comma if found return the string after two comma */
	  if ( counter == 2)
	    {
	      count1 = i+1;
	      return str+count1;
	    }
	}
    }
  /* if there are not two comma return null  */
  return NULL;

}


/* function to return the unsigned long value of m  */
unsigned long mgetter(char *str)
{
  unsigned long finalm;
  char *end;
  int i ;
  char subStr;
  for (i = 0; i < strlen(str); i++)
    {
      if (str[i] == ',')
	{
	  char subStr[i];
	  /*to copy the string after the first character  */
	  strncpy(subStr, str + 1, i-1);
	  /* to convert string into unsigned long  */

	  finalm = strtoul(subStr, &end, 10);
	  return finalm;
	}
    }

  /*to return 0  */
  return 0;

}

/*function to return the unsigned long value of c   */
unsigned long cgetter(char *str)
{
  unsigned long finalc = 0;
  char *end;
  int counter = 0;
  int counter1 = 0;
  int counter2 = 0;
  int i;

  /*to go over the end of  input string    */
  for ( i = 0; i < strlen(str); i++)
    {
      if (str[i] == ',')
	{
	  counter1 = counter2 + 1;
	  counter2 = i;
	  counter++;
	  /* to check if there are two commas  */
	  if (counter == 2)
	    {
	      char subStr[counter2 - counter1 + 1];
	      /* to copy the string between first and second comma  */
	      strncpy(subStr, str + counter1, counter2 - counter1);
	      /*to convert the string into usigned long   */
	      finalc = strtoul(subStr, &end, 10);
	      return finalc;
	    }

	}
    }
  return 0;
}

/*function to encrypt the input*/
void encrypted(struct LinearCongruentialGenerator* lcg, char* msg)
{
  int len ;
  int i;
  char* s;
  unsigned long x;
  char ok;
  char sum;
  len = strlen(bytegetter(msg))-1;
  for (i = 0; i < len; i++)
    {
      /* to print error if m is 0*/
      if ( lcg->m ==0 )
	{
	  printf("Error");
	  break;
	}

      s = bytegetter(msg);
      /*to get the new random value */

      x = getNextRandomValue(lcg);
      ok = s[i]^(x % 128);
      /*to check if the new character has ascii value of 127*/
      if ( ok == 127 )
	{
	  printf("*$");
	}

      /*to check if the new character has ascii value of less than 32*/
      else if (ok< 32)
	{
	  printf("*");
	  sum = ok+63;
	  printf("%c",sum);
	}

      /*to check if the new character has ascii value of 42*/
      else if ( ok ==42 )
	{
	  printf("**");
	}
      else
	{
	  printf("%c", ok);
	}

    }
  printf("\n");
}

/*function to decrypt the input*/

void decrypted(struct LinearCongruentialGenerator* lcg, char* msg)
{
  int len;
  int i;
  char* s;
  unsigned long x;
  char ok;

  len = strlen(bytegetter(msg))-1;
  for (i = 0; i < len; i++)
    {
      s = bytegetter(msg);
      x = getNextRandomValue(lcg);
      /*to check if there consecutiv * */
      if (s[i] == '*' && s[i+1]== '*')
	{
	  i++;
	  ok = s[i]^(x % 128);
	  printf("%c", ok);
	}

      /* to chek if * and $ comes consecutively*/
      else if (s[i] == '*' && s[i+1]== '$')
	{
	  s[i+1] = 127;
	  i++;
	  ok = s[i]^(x % 128);
	  printf("%c", ok);
	}

      /*to check the special condition */
      else if (s[i] == '*'&& s[i+1]>=32 && (s[i+1]!= '*'||s[i+1]!= '$' ))
	{
	  /*to subtract 63 from the ascii after the * character*/
	  s[i+1] = s[i+1]-63;
	  i++;
	  ok = s[i]^(x % 128);
	  if ( ok>126 || ok<32)
	    {
	      /*to print error if the output is not in the range of the ascii*/
	      printf("Error");
	      break;
	    }
	  printf("%c", ok);
	}
      else
	{
	  /*to print the character after applying hte formula*/
	  ok = s[i]^(x % 128);
	  if ( ok>126 || ok<32)
	    {
	      /*to print error if the output is not in the range of the ascii*/
	      printf("Error");
	      break;
	    }
	  printf("%c", ok);
	}
    }
  printf("\n");
}


int main()
{
  char input[1000000];
  char firstChar ;
  char secondChar;
  struct LinearCongruentialGenerator lcg1;

  /*to read the input from the user */
  while (fgets(input, sizeof(input), stdin) != NULL)
    {
      firstChar = input[0];
      secondChar = input[1];

      /*to check if the second character is digit or not*/
      if ( !isdigit(secondChar))
	{
	  printf("%5d: Error\n", lineNumber);
	  lineNumber++;
	  continue;
	}

      /*to check if the value of c is zero*/
      if ( cgetter(input)== 0)
	{
	  printf("%5d: Error\n", lineNumber);
	  lineNumber++;
	  continue;
	}


      /*to make the struct that has m and c as a parameter */
      lcg1 = makeLCG(mgetter(input),cgetter(input));

      /*to decrypt the message if the first character is d */
      if ( firstChar== 'd')
	{
	  printf("%5d: ", lineNumber) ;
	  decrypted(&lcg1,input);
	  lineNumber++;
	}

      /*to encrypt the message if the first character is e */
      else if (firstChar== 'e')
	{
	  printf("%5d: ", lineNumber) ;
	  encrypted(&lcg1,input);
	  lineNumber++;
	}

      else
	{
	  /*to print error if first character is neither d or e */
	  printf("%5d: Error\n",lineNumber);
	  lineNumber++;
	}
    }
  return 0;
}




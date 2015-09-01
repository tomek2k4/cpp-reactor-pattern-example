/*
 * utils.c
 *
 *  Created on: 6 mar 2015
 *      Author: Tomek
 */

#include <time.h>
#include <stdlib.h>  // rand(), srand()
#include <stdio.h>
#include "type_def.h"
#define __UTILS__
#include "utils.h"

void randstring(char* const data, int length)
{
  int i;
  time_t t;
  char min = 'A';
  char max = 'z';
  char x;

  if (length)
  {
       /* Intializes random number generator */
       srand((unsigned) time(&t));

       if (data)
       {
           for (i = 0;i < length;i++)
           {
               int key = rand() % (max - min + 1) + min;
               data[i] = key;
           }
       }
  }
}



void error(const char *msg)
{
    perror(msg);
    exit(0);
}


#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/* Fn Declarations */

char *file_to_str();

/* End Fn Declarations */


char *file_to_str(FILE *src) {
  
  char *result = (char *)malloc(128);
  size_t result_size = 128;
  char *buff = (char *)malloc(128);

  result[0] = '\0';
  buff[0] = '\0';
  
  while ( fgets( buff, 128, src) != NULL ) {
    
    if ( strlen(result) + strlen(buff) > result_size ) {

      result_size *= 2;

      if ( realloc(result, result_size) == NULL ) {

	printf("ERROR: Memory could not be reallocated\n");
	return NULL;
	
      }
    }

    strcpy(&result[strlen(result)], buff);
    buff[0] = '\0';
  }

  return result;
  
}

#endif /* FILE_UTILS_H */

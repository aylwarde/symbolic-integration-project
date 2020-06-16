#ifndef MVPOLYS_H
#define MVPOLYS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "frac.h"

typedef struct mvpoly {
  char * variables;
  unsigned int varnum;
  
  unsigned int * degrees;
  unsigned int size;
  
  frac **coefficients;
} mvpoly;

/* Fn Defs */

unsigned int *addr_offset_to_degs(unsigned int, unsigned int, unsigned int *);
unsigned int degs_to_addr_offset(unsigned int, unsigned int *, unsigned int *);

mvpoly *init_mv(char *, unsigned int *);
mvpoly *transpose(mvpoly *, unsigned int, unsigned int);

/* End Fn Defs */

/*
  Convert an address offset of the coefficient list into an array of integers
  corresponding to the degrees of the monomial associated with the given coefficient
*/
unsigned int *addr_offset_to_degs(unsigned int varnum, unsigned int addr_offset, unsigned int *degrees){
  
  unsigned int *result = (unsigned int *)calloc(varnum, sizeof(unsigned int));
  unsigned int r, i;

  for (i=0; i<varnum; ++i) {
    
    r = addr_offset % (degrees[i]+1);
    addr_offset /= (degrees[i]+1);

    result[i] = r;
  }

  return result;
}

/*
  Convert an array of integers of a monomial of a polynomial with specified degrees (coords) to the corresponding
  location offset from the start of the coefficient array.
*/
unsigned int degs_to_addr_offset(unsigned int varnum, unsigned int *coords, unsigned int *degrees) {

  unsigned int result = coords[0], i, q=1;
  printf("%d\n", coords[0]);
  for (i=1; i<varnum; ++i) {
    printf("%d\n", result);
    q *= degrees[i-1]+1;
    result += q*coords[i];
    printf("%d\n", coords[i]);
  }

  return result;
}

// initialise a zero polynomial of given degrees in given variables
mvpoly *init_mv(char * variables, unsigned int * degrees) {
  unsigned int varnum = strlen(variables), i;

  mvpoly *result = (mvpoly *)calloc(1,sizeof(mvpoly));

  result->varnum = varnum;
  result->variables = variables;
  result->degrees = degrees;
  result->size = 1;
  
  for (i=0; i<varnum; ++i) {
    
    result->size *= degrees[i]+1;
    
  }

  frac **coefficients=(frac **)calloc(result->size+1,sizeof(frac *));
	
  mpz_t zero; mpz_init(zero);
  mpz_t one; mpz_init_set_ui(one, 1);

  for(i=0; i<=result->size; ++i) {
		
      coefficients[i] = init_f(zero,one);
      
  }

  result->coefficients = coefficients;

  return result;
}


#endif /* MVPOLYS_H */

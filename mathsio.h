#ifndef MATHSIO_H
#define MATHSIO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gmp.h>

#include "frac.h"
#include "polynomials.h"
#include "rationalfns.h"
#include "bivariate_poly.h"
#include "mvpolys.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/* Fn Defs */

char *string_f(frac *, bool);
char *string_p(poly *);
char *string_r(rational *);
char *string_monomial(char *, unsigned int *);
char *string_mv(mvpoly *);

/* End Fn Defs */


// Takes a frac * argument and returns a string formatted "num/denom" or "num" if retaindenoms = false
char *string_f(frac *frac1, bool retaindenoms) {

  char *numstr = mpz_get_str(NULL, 10, frac1->num);
  char *denomstr = mpz_get_str(NULL, 10, frac1->denom);
  
  long result_len = strlen(numstr) + 1;

  if ( (mpz_cmp_ui(frac1->denom, 1) != 0) || (retaindenoms)) {

    result_len += strlen(denomstr) + 1;
    
  }

  char *result_str = (char *)calloc(result_len, sizeof(char));

  strcat(result_str, numstr);

  if ( (mpz_cmp_ui(frac1->denom, 1) != 0) || (retaindenoms)) {

    strcat(result_str, "/");
    strcat(result_str, denomstr);
    
  }

  free(numstr);
  free(denomstr);
  return result_str;
}


/* 
   Takes poly * argument and returns a string formatted +/-(a_n)x^n as long as a_n is non-zero; the term of 
   degree n is ommitted if its coefficient is zero.
*/  
char *string_p(poly *poly1) {
  
  long *term_len = (long *)calloc(poly1->deg + 1, sizeof(long));
  char **term_str = (char **)calloc(poly1->deg + 1, sizeof(char *));
  long outlen = 0;

  const char fstrs[5][16] =
    {
     "",
     "-(%s)",
     "+(%s)",
     "-(%s)x^%d",
     "+(%s)x^%d"
    };

    int i, j;

  for (i=0; i<=poly1->deg; ++i) {
    
    if (i == poly1->deg) {
      
      switch (mpz_sgn(poly1->coefficients[i]->num)) {

      case -1:

        j = 1;
	break;
	
      case 0:

        j = 0;
	break;

      case 1:

        j = 2;
	break;
	
      }
    } else {
      
      switch (mpz_sgn(poly1->coefficients[i]->num)){

      case -1:

        j = 3;
	break;
	
      case 0:

        j = 0;
	break;

      case 1:

        j = 4;
	break;
      
      }
    }

    frac *acoeff = abs_f(poly1->coefficients[i]);

    char *coeff = string_f(acoeff, false);

    free_f(acoeff);

    term_len[i] = snprintf(NULL, 0, fstrs[j], coeff, poly1->deg - i);
    outlen += term_len[i];
    
    term_str[i] = (char *)calloc(term_len[i] + 1, sizeof(char));
    
    snprintf(term_str[i], term_len[i] + 1, fstrs[j], coeff, poly1->deg - i);

    free(coeff);
  }

  char *result = (char *)calloc(outlen, sizeof(char));

  for (i=0; i<poly1->deg; ++i) {
    strcat(result, term_str[i]);
  }

  strcat(result, term_str[poly1->deg]);

  free(term_len);
  
  for (i=0; i<=poly1->deg; ++i) {
    free(term_str[i]);
  }

  free(term_str);

  return result;
  
}


char *string_r(rational *rf1) {

  char *numstr = string_p(rf1->num);
  char *denomstr = string_p(rf1->denom);

  long maxstrlen = MAX(strlen(numstr), strlen(denomstr));

  char *midbar = (char *)calloc(maxstrlen, sizeof(char));  int i;
  
  for (i=0; i<maxstrlen; ++i) {

    midbar[i] = '-';
    
  }

  char *result_str = (char *)calloc( 3*(maxstrlen + 1), sizeof(char));

  strcat(result_str, numstr);
  strcat(result_str, "\n");
  strcat(result_str, midbar);
  strcat(result_str, "\n");
  strcat(result_str, denomstr);
  
  return result_str;
}

char *string_monomial(char * variables, unsigned int * degrees) {
  unsigned int outterms = 0, i, outlen=0;
  unsigned int *nonzeroterms = (unsigned int *)calloc(strlen(variables), sizeof(unsigned int));

  for (i=0; i<strlen(variables); ++i) {

    if (degrees[i] != 0) {
      nonzeroterms[outterms] = i;
      ++outterms;  
    }
  }
  
  char ** terms = (char **)calloc(outterms, sizeof(char *));

  for (i=0; i<outterms; ++i) {
    terms[i] = (char *)calloc(8, sizeof(char));
    snprintf(terms[i], 8, "%c^%d", variables[nonzeroterms[i]], degrees[nonzeroterms[i]]);
    outlen += strlen(terms[i]);
  }

  char *result = (char *)calloc(outlen + 1, sizeof(char));

  for (i=0; i<outterms; ++i) {
    
    strcat(result, terms[i]);
      
  }

  return result;
}


char *string_mv(mvpoly *mvp1) {
  
  long total_len = 0, *term_len = (long *)calloc(mvp1->size + 1, sizeof(long));
  char **term_str = (char **)calloc(mvp1->size + 1, sizeof(char *));
  long outlen = 0;
  unsigned int i,j;
  
  for (i=0; i<mvp1->size; ++i) {
    
    unsigned int *coords = addr_offset_to_degs(mvp1->varnum, i, mvp1->degrees);
    
    char *coeff = string_f(mvp1->coefficients[i], false);
    char *monomial = string_monomial(mvp1->variables, coords);

    term_len[i] = strlen(coeff) + strlen(monomial) + 1;
    total_len += term_len[i];
    term_str[i] = (char *)calloc(term_len[i] + 1, sizeof(char));

    strcat(term_str[i], "+");
    strcat(term_str[i], coeff);
    strcat(term_str[i], monomial);

    free(coeff);
    free(monomial);
    free(coords);
  }

  char *result = calloc(total_len + 1, sizeof(char));

  for (i=0; i<mvp1->size; ++i) {

    strcat(result, term_str[i]);
    
  }
  
  free(term_len);
  free(term_str);
  return result;
}

#endif /* MATHSIO_H */

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

/* Fn Defs */

char *string_f(frac *, bool);
char *string_p(poly *);

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


char *string_p(poly *poly1) {
  
  long *term_len = (long *)calloc(poly1->deg + 1, sizeof(long));
  char **term_str = (char **)calloc(poly1->deg + 1, sizeof(char *));
  long outlen = 0;

  int i;

  for (i=0; i<=poly1->deg; ++i) {

    char *coeff = string_f(poly1->coefficients[i], false);

    term_len[i] = snprintf(NULL, 0, "(%s)x^%d", coeff, i);
    outlen += term_len[i];
    
    term_str[i] = (char *)calloc(term_len[i] + 1, sizeof(char));
    
    snprintf(term_str[i], term_len[i] + 1, "(%s)x^%d", coeff, poly1->deg - i);

    free(coeff);
  }

  char *coeff = string_f(poly1->coefficients[i], false);

  term_len[poly1->deg] = snprintf(NULL, 0, "(%s)", coeff);
  outlen += term_len[poly1->deg];
    
  term_str[poly1->deg] = (char *)calloc(term_len[i] + 1, sizeof(char));
    
  snprintf(term_str[poly1->deg], term_len[poly1->deg] + 1, "(%s)", coeff);

  free(coeff);

  char *result = (char *)calloc(outlen + poly1->deg, sizeof(char));

  for (i=0; i<poly1->deg; ++i) {
    strcat(result, term_str[i]);
    strcat(result, "+");
  }

  strcat(result, term_str[poly1->deg]);

  free(term_len);
  
  for (i=0; i<=poly1->deg; ++i) {
    free(term_str[i]);
  }

  free(term_str);

  return result;
  
}


#endif /* MATHSIO_H */

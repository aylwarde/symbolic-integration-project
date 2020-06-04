#ifndef FRAC_H
#define FRAC_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>
#include <string.h>

#include "int_utils.h"

/*defining frac struct*/

typedef struct frac {
  mpz_t num;
  mpz_t denom;
} frac;


/* Functions defined in this header */
frac *init_f();

void print_f();
void free_f();

int reduce_f();

frac *add_f();
frac *multiply_f();
frac *negative_f();
frac *reciprocal_f();
frac *pow_f();
frac *divide_f();
frac *subtract_f();
frac *gcd_f();
frac *lcm_f();
frac *gcd_array_f();
frac *lcm_array_f();

frac **copy_array_f();

bool zero_f();
bool equals_f();

char *latex_f( frac *, const char *, const char * );
/* End of function defs*/


void print_f(frac *frac_a) {

         char *string;
         string = (char *)malloc(sizeof(frac));


        mpz_get_str(string, 10, frac_a->num);

        if(mpz_cmp_si(frac_a->denom, -1)==0) {
                mpz_get_str(string, 10, negative_f(frac_a)->num);
                printf("%s", string);

        }

        else {
                mpz_get_str(string, 10, frac_a->num);
                printf("%s", string);

                if (mpz_cmp_si(frac_a->denom, 1) != 0 && mpz_cmp_si(frac_a->num,0)!=0) {
                                mpz_get_str(string, 10, frac_a->denom);
                                printf("/%s", string);
                        }
        }

  free(string);
  string =NULL;

}



//free a frac
void free_f(frac *frac_a) {
	
	mpz_clears(frac_a->num, frac_a->denom, NULL);	
  	free(frac_a);
}


//constructor for fractions; takes arguments for numerator & denominator and returns a pointer to a rational.
frac  *init_f(mpz_t num, mpz_t denom) {

  if( mpz_cmp_si(denom, 0) == 0 ){

	printf("Error:\n");
    	return NULL;

  } 
  
  else {
  
    	frac *frac_a = (frac *)calloc(1,sizeof(frac));
	mpz_init(frac_a->num);
	mpz_init(frac_a->denom);

	mpz_set(frac_a->num, num);
	mpz_set(frac_a->denom, denom);

	reduce_f(frac_a);
	
	return frac_a;
  }
  
}


//fully reduces a fraction; i.e. returns an equal fraction such that numerator and denomenator are coprime.
int reduce_f(frac *frac_a){
  	
	mpz_t div; mpz_init(div);
	
	gcd_z(div, frac_a->num , frac_a->denom );
  
	mpz_t newnum, newdenom;
	mpz_init(newnum);
	mpz_init(newdenom);

	mpz_cdiv_q(newnum, frac_a->num, div);
  	mpz_cdiv_q(newdenom, frac_a->denom, div);     

	if ( mpz_sgn(newdenom) == -1 ) {
	  mpz_neg(newnum, newnum);
	  mpz_neg(newdenom, newdenom);
	}
	
	mpz_set(frac_a->num, newnum);
	mpz_set(frac_a->denom, newdenom);
	
  	/* return init_f(newnum, newdenom); */
	
	mpz_clears(newnum, newdenom, div, NULL);

	return 0;
}


//adds two fractions
frac *add_f(frac *frac_a, frac *frac_b) {

	frac *frac_add;
	mpz_t newnum, newdenom;
	mpz_init(newnum);
	mpz_init(newdenom);

  	mpz_mul(newnum, frac_a->num, frac_b->denom);
       	mpz_addmul(newnum, frac_a->denom, frac_b->num); 	

  	mpz_mul(newdenom, frac_a->denom, frac_b->denom);
  
	frac_add = init_f(newnum, newdenom);
	mpz_clears(newnum, newdenom, NULL);
  	return frac_add;
  
}


//multiplies two fractions
frac *multiply_f(frac *frac_a, frac *frac_b) {

  	frac *frac_mult;
	mpz_t newnum, newdenom;
	mpz_init(newnum);
	mpz_init(newdenom);

  	mpz_mul(newnum, frac_a->num, frac_b->num);
  	mpz_mul(newdenom,frac_a->denom, frac_b->denom);

  	frac_mult = init_f(newnum, newdenom);
	mpz_clears(newnum, newdenom, NULL);
	return frac_mult;

}


//negates a fraction
frac *negative_f(frac *frac_a) {
	
	frac *frac_neg;
	mpz_t neg; mpz_init(neg);
	mpz_neg(neg, frac_a->num);

	frac_neg = init_f(neg, frac_a->denom);
	mpz_clear(neg);
  	return frac_neg;
  
}


//returns the reciprocal of a fraction - make sure that the return value is not NULL as this may occur if you
//provide a zero input and it will break any subsequent code
frac *reciprocal_f(frac *frac_a) {

  	return init_f(frac_a->denom, frac_a->num);
    
}



//exponentiates a fraction to ***INTEGER*** powers
frac *pow_f(frac *frac_a, int exp) {

	frac *frac_pow, *frac_recip; 

  	if (exp == 0) {
    		
		mpz_t one;
		mpz_init_set_ui(one, 1);
		frac_pow = init_f(one, one);
		mpz_clear(one);
    		return frac_pow;
 	} 
	
	else {

	       	if (exp>0) {
	
			mpz_t pow_num, pow_denom;
			mpz_init(pow_num);
			mpz_init(pow_denom);

			pow_z(pow_num, frac_a->num, exp);
			pow_z(pow_denom, frac_a->denom, exp);
    			
			frac_pow = init_f(pow_num, pow_denom);
			mpz_clears(pow_num, pow_denom, NULL);
			return frac_pow;
    		} 
	
		else {
			frac_recip = pow_f(frac_a, -exp);
			frac_pow = reciprocal_f(frac_recip);
			free_f(frac_recip);
    			return frac_pow;
		}
 	}
}



//subtract frac_b from frac_a
frac *subtract_f(frac *frac_a, frac *frac_b) {

	frac *frac_subtract, *frac_neg;
	
	frac_neg = negative_f(frac_b);
	frac_subtract = add_f(frac_a, frac_neg);
	free_f(frac_neg);

  	return  frac_subtract;
  
}


//divide frac_a by frac_b
frac *divide_f(frac *frac_a, frac *frac_b) {

	frac *frac_recip, *frac_divide;

	frac_recip = reciprocal_f(frac_b);
	frac_divide = multiply_f(frac_a, frac_recip);
	free_f(frac_recip);

  	return frac_divide;
  
}


//returns true if rational zero, false otherwise
bool zero_f(frac *frac_a)
{
	bool result = false;

	if(mpz_cmp_si(frac_a->num, 0)==0)
	{
		result = true;
	}
	
	return result;
}


//returns true if two rationals are equal, false otw.
bool equals_f(frac *frac_a, frac *frac_b) {

  	bool result; 
	frac *frac_subtract;

	frac_subtract = subtract_f(frac_a, frac_b);
	result = zero_f(frac_subtract);
	free_f(frac_subtract);
	return result;
    
}



//copy array of fractions
frac **copy_array_f(frac **src, int len) {
  
  	frac **result = (frac **)malloc(len*sizeof(frac *));
  	int i;
  
  	for (i=0 ; i<len; ++i) {
    
    		result[i]=src[i];
    
 	 }
  
  	return result;
  
}

//gcd of two fractions
frac *gcd_f(frac *frac_a, frac *frac_b) {

	frac *frac_gcd;
	mpz_t gcd_num, lcm_denom;
	mpz_init(gcd_num);
	mpz_init(lcm_denom);

	gcd_z(gcd_num, frac_a->num, frac_b->num);
	lcm_z(lcm_denom, frac_a->denom, frac_b->denom);

	frac_gcd = init_f(gcd_num, lcm_denom);
	mpz_clears(gcd_num, lcm_denom, NULL);
	return frac_gcd;
}

//lcm of two fractions
frac *lcm_f(frac *frac_a, frac *frac_b) {
	
	frac *frac_lcm;
	mpz_t lcm_num, gcd_denom;
	mpz_init(lcm_num);
	mpz_init(gcd_denom);

	lcm_z(lcm_num, frac_a->num, frac_b->num);
	gcd_z(gcd_denom, frac_a->denom, frac_b->denom);

	frac_lcm = init_f(lcm_num, gcd_denom);
	mpz_clears(lcm_num, gcd_denom, NULL);
	return frac_lcm;
}

//gcd of an array of fractions, with i index of last entry
frac *gcd_array_f(int i, frac **frac_array) {

  	frac *gcd_array;
  	
	if(i==0) {
      		
		return frac_array[0];
    	}
  	
	else if(i==1) {
     		
		gcd_array = gcd_f(frac_array[1], frac_array[0]);
     		return gcd_array;
    	}

  	else {
     		return gcd_f(gcd_array_f(i-1, frac_array), frac_array[i]);
    	}
}

//gcd of an array of fractions
frac *lcm_array_f(int i, frac **frac_array) {

  	frac *lcm_array;
  	
	if(i==0) {
     
		return frac_array[0];
    	}
  	
	else if(i==1) {
     
		lcm_array = lcm_f(frac_array[1], frac_array[0]);
     		return lcm_array;
    	}
  	
	else {
     		return lcm_f(lcm_array_f(i-1, frac_array), frac_array[i]);
    }
}

/* 
   takes a frac pointer and two const strs  as arguments; returns a char * with the fraction formatted 
   in LaTeX code. Pass to leftwrap and rightwrap the formatters you want to wrap the output in - these can be
   empty.
*/
char *latex_f(frac *frac_a, const char * leftwrap, const char * rightwrap) {

  char *result, *numstr;
  size_t result_size = 64;
  result = (char *)calloc(result_size, sizeof(char));

  strcat(result, leftwrap);

  
  if ( mpz_cmp_ui(frac_a->denom, 1) != 0 ) {
    
    strcat(result, "\\frac{");
    
  }

  size_t result_free = result_size - strlen(result);

  numstr = (char *)mpz_get_str(NULL, 10, frac_a->num);
  
  while ( strlen(numstr) >= result_free ) {
    
    result_size *= 2;
    if ( realloc(result, result_size*sizeof(char)) == NULL) {

  	printf("Error");
  	return NULL;
	
      }
    
  }
  
  strcat(result, numstr);
  
  result_free = result_size - strlen(result);

  if ( mpz_cmp_ui(frac_a->denom, 1) != 0 ) {
    
    char *denomstr = (char *)mpz_get_str(NULL, 10, frac_a->denom);

    while ( strlen(denomstr) >= result_free ) {
    
      result_size *= 2;
      if ( realloc(result, result_size*sizeof(char)) == NULL) {

    	printf("Error");
    	return NULL;
	
      }
    
    }
    
    strcat(result, "}{");
    strcat(result, denomstr);
    strcat(result, "}");
    
  }

  strcat(result, rightwrap);

  return result;
  
}
#endif /* FRAC_H */

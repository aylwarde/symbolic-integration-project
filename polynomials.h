#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "int_utils.h"
#include "file_utils.h"
#include "frac.h"

/*defining a polynomial structure over integers and some basic polynomial operations */

typedef struct poly {
	int deg;
	frac **coefficients;
} poly;


/* Functions defined in this header */
void assign_coeffs_p();
void display_p();
void free_p();
void strip_p();

bool zero_p();
bool equals_p();

poly *initialize_p();
poly **initialize_array_p();
poly *initialize_from_array_p();
poly *copy_p();
poly *negative_p();
poly *scale_p();
poly *add_p();
poly *subtract_p();
poly *multiply_p();
poly *pow_p();
poly **divide_p();
poly *derivative_p();
poly *gcd_p();
poly *conmultiply_p();

frac *content_p();

poly **from_file_p(); //To Do ~ Joe
void to_file_p();

/* End of function defs */


//allocate a polynomial of certain degree, and intialize all coeffs to zero
poly *initialize_p(int degree)
{
	int i;

	poly *result= (poly *)calloc(1,sizeof(poly));
	frac **coefficients=(frac **)calloc(degree+1,sizeof(frac));

	for(i=0; i<=degree; ++i)
	{
		coefficients[i] = init_f(0,1);
	}

	result->deg = degree;
	result->coefficients = coefficients;
	return result;
	}

//initialize an array of a prescribed length of polynomial structures
poly **initialize_array_p(int len) {

	poly **array_p = (poly **)calloc(len,sizeof(poly *));
	int i;

	for(i=0; i<len; ++i)
	{
		array_p[i] = (poly *)calloc(1, sizeof(poly));
	}
	
	return array_p;
}

//allocate a polynomial of given degree with coefficients from a list
poly *initialize_from_array_p(int degree, frac **coefficients) {

  poly *result = (poly *)calloc(1,sizeof(poly));

  result->deg = degree;
  result->coefficients = coefficients;
  return result;
}

//assign values to coefficients of polynomial, in decreasing degree (include zero coefficients)
void assign_coeffs_p(poly *polynomial)
{
	int i;
	for(i=0; i<=polynomial->deg; ++i)
	{
		printf("Enter coefficient of x^%d:\n", (polynomial->deg-i));
		scanf("%ld", &polynomial->coefficients[i]->num);
		printf("/");
		scanf("%ld", &polynomial->coefficients[i]->denom);
	}
}

//display a polynomial
void display_p(poly *polynomial)
{
	int i;
	for(i=0; i<polynomial->deg;++i)
	{
		print_f(polynomial->coefficients[i]);
		printf("x^%d+",polynomial->deg-i);

	}
	print_f(polynomial->coefficients[polynomial->deg]);
	printf("\n");
}


//free a polynomial
void free_p(poly *polynomial)
{
	free(polynomial->coefficients);
	free(polynomial);
}

//check if polynomial is zero (return boolean value)
bool zero_p(poly *polynomial)
{	int i;
	bool result = true;
	for(i=0; i<polynomial->deg+1;++i)
	{
		if(!zero_f(polynomial->coefficients[i]))
		{ 
			result = false;
		}
	}
	return result;
}

//strip a polynomial of higher order terms with zero coefficients
void strip_p(poly *polynomial)
{
	int i, leading_zeroes=0, degree;
	
	//make a zero polynomial the constant zero
	if(zero_p(polynomial))
	{	
		polynomial->deg = 0;
	}
	
	else
	{
		while(zero_f(polynomial->coefficients[leading_zeroes]))
		{
			++leading_zeroes;
		}
		for(i=0; i<=polynomial->deg-leading_zeroes; ++i)
		{
			polynomial->coefficients[i] = polynomial->coefficients[i+leading_zeroes];
		}	
		degree = polynomial->deg-leading_zeroes;
		polynomial->deg = degree;
	}
}

//duplicate a polynomical structure
poly *copy_p(poly *polynomial)
{
	int i;
	poly *duplicate;
	duplicate = initialize_p(polynomial->deg);
	for(i=0; i<polynomial->deg+1;++i)
	{
		duplicate->coefficients[i]=polynomial->coefficients[i];
	}
	return duplicate;
}

//return negative of polynomial
poly *negative_p(poly *polynomial)
{
	int i;
	poly *neg_polynomial;
	neg_polynomial = initialize_p(polynomial->deg);
	for(i=0; i<=neg_polynomial->deg; ++i)
	{ 
		neg_polynomial->coefficients[i] = negative_f(polynomial->coefficients[i]);
	}
	return neg_polynomial;
}

//multiply polynomial by a constant
poly *scale_p(frac *frac, poly *polynomial)
{
	int i;
	poly *scaled_polynomial;
	scaled_polynomial = initialize_p(polynomial->deg);
	for(i=0; i<=scaled_polynomial->deg; ++i)
	{ 
		scaled_polynomial->coefficients[i] = multiply_f(frac, polynomial->coefficients[i]);
	}
	return scaled_polynomial;
}

//add two polynomials
poly *add_p(poly *polynomial1, poly *polynomial2)
{
	int i, difference;
	poly *result;

	if(polynomial1->deg<polynomial2->deg)
	{ 
		result = add_p(polynomial2, polynomial1);
	}
	else
	{
		difference = polynomial1->deg-polynomial2->deg;
		result = initialize_p(polynomial1->deg);

		for(i=0; i<difference; ++i)
		{
			result->coefficients[i] = polynomial1->coefficients[i];
		} 
		for(i=difference; i<result->deg+1; ++i)
		{
			result->coefficients[i] = add_f(polynomial1->coefficients[i],polynomial2->coefficients[i-difference]);
		}
	}
	strip_p(result);
	return result;

}

//subtract poly2 from poly1
poly *subtract_p(poly *polynomial1, poly *polynomial2)
{
	poly *result;
	polynomial2 = negative_p(polynomial2);
	result = add_p(polynomial1, polynomial2);
	return result;
}

//polynomial multiplication
poly *multiply_p(poly *polynomial1, poly *polynomial2)
{
	int i,j;
	poly *result;

	result = initialize_p(polynomial1->deg+polynomial2->deg);
	
	for(i=0; i<=polynomial1->deg; ++i)
	{
		for(j=0; j<=polynomial2->deg; ++j)
		{
			result->coefficients[i+j]=add_f(result->coefficients[i+j], multiply_f(polynomial1->coefficients[i], polynomial2->coefficients[j]));
		}
	}
	strip_p(result);
	return result;
}

//raise a polynomial to a power
poly *pow_p(int exponent, poly *poly) {

	if(exponent==1)
	{ 
		return poly;
	}

	else
	{
		return multiply_p(pow_p(exponent-1, poly), poly);
	}	
}


//polynomial division returns 'list' containing pointers to quotient and remainder

poly **divide_p(poly *polynomial1, poly *polynomial2)
{
	int d=0;
	frac *t;
	poly *division, *quotient, *remainder;
	poly **result;
	
	result = initialize_array_p(2);

	if(zero_p(polynomial2))
	{
		printf("Error: division by zero polynomial");
		exit(0);
	}
	else 
	{
		quotient = initialize_p(polynomial1->deg-polynomial2->deg);
		remainder = copy_p(polynomial1);

		while(!zero_p(remainder) && (remainder->deg-polynomial2->deg)>=0)
		{	
			d = remainder->deg-polynomial2->deg;
			t = divide_f(remainder->coefficients[0], polynomial2->coefficients[0]);
			
			division = initialize_p(d);
			division->coefficients[0] = t;

			quotient = add_p(quotient, division);
			remainder = subtract_p(remainder, multiply_p(polynomial2, division));
		}
	
	result[0] = quotient;
	result[1] = remainder;
	}
	return result;
}

//derivative of polynomial
poly *derivative_p(poly *polynomial) {
	int i;
	long d = (long)polynomial->deg;

	for(i=0; i<(int)d; ++i)
	{
		polynomial->coefficients[i] = multiply_f(init_f(d-i,1), polynomial->coefficients[i]);
	}
	polynomial->deg = (int)d-1;
	return polynomial;
}

poly *gcd_p(poly *polynomial1, poly *polynomial2) {

        poly *q, *r;

        while(!zero_p(polynomial2))
        {
                q = divide_p(polynomial1, polynomial2)[0];
                r = divide_p(polynomial1, polynomial2)[1];
                polynomial1 = copy_p(polynomial2);
                polynomial2 = copy_p(r);
        }

        return polynomial1;
}

//return content of polynomial
frac *content_p(poly *polynomial) {

	return gcd_array_f(polynomial->deg, polynomial->coefficients);
}
/* 
   returns true if poly_a and poly_b are exactly equal in all coefficients; 
   NOTE: will return false for poly_a, poly_b  equal up to a constant factor
*/
bool equals_p(poly *poly_a, poly * poly_b) {

  return zero_p(subtract_p(poly_a, poly_b));
  
}

/* 
   Takes an input stream of a format to be specified containing a number of polynomials specified 
   by rational coefficients and returns a pointer to a list of poninters to polynomial objects
   representing those polynomials, as well as setting outlen to the number of polynomials outputted. 
   Will return null if file is missing a number corresponding to the number of polynomials stored in 
   the file, and will return a null pointer if the degree of any polynomial stored is invalid (i.e. 
   not a positive int)
*/
poly **from_file_p(FILE *src, int *outlen) {
  // read the file src containing some polynomials and output an array containing those polynomials
  char *data = file_to_str(src);
  poly **result = (poly **)calloc(1, sizeof(poly *));
  char *tok1, *tok2;
  char *line;

  // placeholder pointer
  char *ptr1 = data;
  int i=0, j=0, deg;
  long num, denom;
  frac **coeffs;

  int k;

  tok1 = strtok_r(data, "\n", &ptr1);

  while ( tok1 != NULL) {
    
    char *ptr2 = tok1;
    tok2 = strtok_r(tok1, " ", &ptr2);

    j=0;
    deg = atoi(tok2);
    coeffs = calloc((deg+1),sizeof(frac));
    
    while ( tok2 != NULL ) {
      
      if (j != 0) {
	
	if ( sscanf(tok2, "%ld/%ld", &num, &denom) != 2){
	    
	  printf("ERROR: Invalid format\n");
	  return NULL;
	  
	}

	coeffs[j-1] = init_f(num, denom);
	  
      }

      ++j;
      tok2 = strtok_r(NULL, " ", &ptr2);
      
    }
    
    if ( realloc(result, (i+1)*sizeof(poly*)) == NULL ) {
	
	printf("ERROR: Unable to reallocate memory\n");
	return NULL;
	
    }

    result[i] = initialize_from_array_p(deg, copy_array_f(coeffs, j-1));
    
    free(coeffs);
    tok1 = strtok_r(NULL, "\n", &ptr1);

    ++i;
    
  }

  *outlen = i;
  
  return result;
  
}

/* 
   Writes polynomial data to a datastream
*/
void to_file_p(FILE *trgt, poly **polys, int polyslength) {
  
} 

//print latex to comand line
void latex_p(poly *polynomial)
{
  printf("$$");
  int i;
  for(i=0;i<polynomial->deg;--i)
    {
      if(polynomial->coefficients[i]->denom=1)
	{
	  printf(" %ld x^%d",polynomial->coefficients[i]->num,polynomial->deg-i);
	}
      else
	{
          printf(" \frac{%ld}{%ld} x^%d +",polynomial->coefficients[i]->num,polynomial->coefficients[i]->denom,polynomial->deg-i);
	}
    }
  if(polynomial->coefficients[polynomial->deg]->denom=1)
    {
      printf(" %ld $$ \n",polynomial->coefficients[polynomial->deg]->num);
    }
  else
    {
     printf(" \frac{%ld}{%ld} $$ \n",polynomial->coefficients[polynomial->deg]->num,polynomial->coefficients[polynomial->deg]->denom);
    }
}

poly *conmultiply_p(frac *c,poly *polynomial)
{
  poly *q;
  q=initialize_p(0);
  q->coefficients[0]=c;
  return multiply_p(q,polynomial);
}

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
poly *initialize_from_array_p();
poly *copy_p();
poly *negative_p();
poly *add_p();
poly *subtract_p();
poly *multiply_p();

poly **from_file_p(); //To Do ~ Joe
void to_file_p();
poly **divide_p();
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
	
	for(i=0;i<polynomial1->deg+1;++i)
	{
		for(j=0;j<polynomial2->deg+1;++j)
		{
			result->coefficients[i+j]=add_f(result->coefficients[i+j], multiply_f(polynomial1->coefficients[i], polynomial2->coefficients[j]));
		}
	}
	strip_p(result);
	return result;

}
//polynomial division (in Z only !!!) returns 'list' containing pointers to quotient and remainder

poly **divide_p(poly *polynomial1, poly *polynomial2)
{
	int d=0;
	frac *t;
	poly *division, *quotient, *remainder;

	poly **result = (poly **)calloc(2,sizeof(poly *));
	result[0] = (poly *)calloc(1, sizeof(poly));
	result[1] = (poly *)calloc(1, sizeof(poly));
	
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

  printf("%s\n", data);

  char *tok1, *tok2;
  char *line;

  // placeholder pointer
  char *ptr1 = data;
  int i=1;

  tok1 = strtok_r(data, "\n", &ptr1);

  while ( tok1 != NULL) {
    
    printf("%s\n", tok1);
    
    char *ptr2 = tok1;
    tok2 = strtok_r(tok1, " ", &ptr2);

    while ( tok2 != NULL ) {

      printf("%s\n", tok2);

      tok2 = strtok_r(NULL, " ", &ptr2);
      
    }
    
    tok1 = strtok_r(NULL, "\n", &ptr1);
    
  }
  
}

/* 
   Writes polynomial data to a datastream
*/
void to_file_p(FILE *trgt, poly **polys, int polyslength) {
  
} 

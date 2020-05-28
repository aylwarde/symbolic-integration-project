#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "int_utils.h"
#include "file_utils.h"
#include "frac.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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
poly **pseudodiv_p();
poly *pseudogcd_p();
poly *pseudogcdinternal_p();

frac *content_p();

poly **from_file_p(); //To Do ~ Joe
void to_file_p();

/* End of function defs */


//allocate a polynomial of certain degree, and intialize all coeffs to zero
poly *initialize_p(int degree)
{
	int i;

	poly *result= (poly *)calloc(1,sizeof(poly));
	frac **coefficients=(frac **)calloc(degree+1,sizeof(frac *));
	
	mpz_t zero; mpz_init(zero);
	mpz_t one; mpz_init_set_ui(one, 1);

	for(i=0; i<=degree; ++i)
	{
		
		coefficients[i] = init_f(zero,one);
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

//assign  integer values to coefficients of polynomial, in decreasing degree (include zero coefficients)
//this function is a bit useless though, file input preferable...  
void assign_coeffs_p(poly *polynomial)
{
	int i;
	long int *num_store;
	long int *denom_store;
	mpz_t num, denom;

	num_store = (long int *)calloc(polynomial->deg+1, sizeof(long int));
	denom_store = (long int *)calloc(polynomial->deg+1, sizeof(long int));

	for(i=0; i<=polynomial->deg; ++i)
	{
		printf("Enter coefficient of x^%d:\n", (polynomial->deg-i));
		scanf("%ld", &num_store[i]);
		printf("/");
		scanf("%ld", &denom_store[i]);
	}

	for(i=0; i<=polynomial->deg; ++i)
	{
		mpz_init_set_si(num, num_store[i]);
		mpz_init_set_si(denom, denom_store[i]);
		mpz_set(polynomial->coefficients[i]->num, num);
		mpz_set(polynomial->coefficients[i]->denom, denom);
		mpz_clear(num);
		mpz_clear(denom);
	}

}

//display a polynomial
void display_p(poly *polynomial)
{
	int i;
	for(i=0; i<polynomial->deg;++i)
	{
		if(!zero_f(polynomial->coefficients[i])) {
			print_f(polynomial->coefficients[i]);
			printf("x^%d+",polynomial->deg-i);
		}
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

//raise a polynomial to a positive power
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
	if(zero_p(polynomial2))
	{
		printf("Error: division by zero polynomial");
		exit(0);
	}

	else {
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
	  		quotient = initialize_p(MAX(polynomial1->deg-polynomial2->deg,0));
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
}

//derivative of polynomial
poly *derivative_p(poly *polynomial) {
	int i=0;

	poly *derivative;
	derivative = initialize_p(polynomial->deg-1);

	mpz_t d; 
	mpz_init_set_si(d, (long)polynomial->deg);
	mpz_t one; mpz_init_set_ui(one,1);

	while(i<polynomial->deg)
	{
		derivative->coefficients[i] = multiply_f(init_f(d,one), polynomial->coefficients[i]);
		mpz_sub_ui(d, d, 1);
		++i;
	}
	return derivative;
}

//euclidean algorithm
poly *gcd_p(poly *polynomial1, poly *polynomial2) {

        poly *q, *r;

        while(!zero_p(polynomial2))
        {
                //q = divide_p(polynomial1, polynomial2)[0];
                r = divide_p(polynomial1, polynomial2)[1];
                polynomial1 = copy_p(polynomial2);
                polynomial2 = copy_p(r);
        }
	polynomial1 = scale_p(reciprocal_f(content_p(polynomial1)), polynomial1);
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
  char *tok1, *tok2, *tok3, *tok4;
  char *line;

  // placeholder pointer
  char *ptr1 = data;
  int i=0, j=0, deg;
  mpz_t num, denom;
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

	char *ptr3 = tok2;
	
	tok3 = strtok_r(tok2, "/", &ptr3);
	tok4 = strtok_r(NULL, "/", &ptr3);
	
	if ( mpz_init_set_str(num, tok3, 10) == -1 || mpz_init_set_str(denom, tok4, 10) == -1 ) {

	  printf("Error");
	  return NULL;
	    
	}
	

	coeffs[j-1] = init_f(num, denom);

	mpz_clear(num);
	mpz_clear(denom);
	  
      }

      ++j;
      tok2 = strtok_r(NULL, " ", &ptr2);
      
    }

    poly **tmp;
    
    if ( (tmp = realloc(result, (i+1)*sizeof(poly*))) == NULL ) {
	
	printf("ERROR: Unable to reallocate memory\n");
	return NULL;
	
    }

    result = tmp;
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
  char *string;
  string =(char *)malloc(sizeof(frac));
  for(i=0;i<polynomial->deg;++i)
    {
      //if denominator is 1 prints numerator 
      if(mpz_cmp_si(polynomial->coefficients[i]->denom,1)==0)
	{
	  mpz_get_str(string,10,polynomial->coefficients[i]->num);
	  printf("%s",string);
	  printf("x^%d+",polynomial->deg-i);
	}

      //skips x^n term if coefficient is 0/m
      else if(mpz_cmp_si(polynomial->coefficients[i]->num,0)==0)
	{
	  //do nothing
	}
      
      else
	{
	  mpz_get_str(string,10,polynomial->coefficients[i]->num);
	  printf("\\frac{%s}{",string);
	  mpz_get_str(string,10,polynomial->coefficients[i]->denom);
	  printf("%s}x^%d+",string,polynomial->deg-i);
	}
    }

  //if denominator is 1 prints numerator 
  if(mpz_cmp_si(polynomial->coefficients[polynomial->deg]->denom,1)==0)
    {
      mpz_get_str(string,10,polynomial->coefficients[polynomial->deg]->num);
      printf("%s",string);
    }

  //skips deg^th term if coefficient is 0/m
  else if(mpz_cmp_si(polynomial->coefficients[polynomial->deg]->num,0)==0)
    {
      //do nothing
    }
      
  else
    {
      mpz_get_str(string,10,polynomial->coefficients[polynomial->deg]->num);
      printf("\\frac{%s}{",string);
      mpz_get_str(string,10,polynomial->coefficients[polynomial->deg]->denom);
      printf("%s}",string);
    }
  printf("$$\n");
  free(string);
}

poly *conmultiply_p(frac *c,poly *polynomial)
{
  poly *q;
  q=initialize_p(0);
  q->coefficients[0]=c;
  return multiply_p(q,polynomial);
}

// does pseudo division and reutrns result in same form as standard division
poly **pseudodiv_p(poly *poly1 , poly *poly2)
{
  frac *c;
  poly *cpoly;
  
  c = gcd_f(content_p(poly1),content_p(poly2));
  cpoly = conmultiply_p(pow_f(c,MAX(poly1->deg - poly2->deg +1,0)), poly1);
  
  return divide_p(cpoly,poly2);
}


//takes two polynomails and returns the gcd using the Pseudo eucledian algorthem

poly *pseudogcd_p(poly* polynomial1,poly* polynomial2)
{
  frac *c,*cont;
  poly *fk;
  if(zero_p(polynomial1) || zero_p(polynomial2))
    {
      printf("function requires non zero polynomials error");
      return NULL;
    }
  else
    {
      c=gcd_f(content_p(polynomial1),content_p(polynomial2));
      
      fk=pseudogcdinternal_p(polynomial1, polynomial2);
      cont=content_p(fk);
      cont=reciprocal_f(cont);
      fk=conmultiply_p(cont,fk);
      return conmultiply_p(c,fk);
    }
    
    
}


    //needs to return fk-1

  poly *pseudogcdinternal_p(poly* poly1,poly* poly2)
  {
    poly *prem;
    
    if(!zero_p(poly2))
      {
    	prem = pseudodiv_p(poly1,poly2)[1];

	display_p(poly2);
	display_p(prem);
    	return pseudogcdinternal_p(poly2,prem);
      }
     else
         {
	   
	 return poly1;
	 
	 }
  }


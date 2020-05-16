#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "frac.h"

/*defining a polynomial structure over integers and some basic polynomial operations */

typedef struct poly {
	int deg;
	frac **coefficients;
} poly;

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

//assign values to coefficients of polynomial, in decreasing degree (include zero coefficients)
void assign_coeffs_p(poly *polynomial)
{
	int i;
	for(i=0; i<=polynomial->deg; ++i)
	{
		printf("Enter coefficient of x^%d:\n", (polynomial->deg-i));
		scanf("%d", &polynomial->coefficients[i]->num);
		printf("/");
		scanf("%d", &polynomial->coefficients[i]->denom);
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
poly *equal_p(poly *polynomial)
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
		remainder = equal_p(polynomial1);

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


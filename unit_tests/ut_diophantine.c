#include <stdio.h>
#include <stdlib.h>

#include "../euclidean_algorithms.h"

int main()
{
	int deg1, deg2, degc;
	poly *poly1, *poly2, *c;
	poly **result;

	printf("Enter degree of polynomial 1\n");
	scanf("%d", &deg1);
	poly1 = initialize_p(deg1);
	printf("Enter coefficients of polynomial in descending degree,including zero coefficients\n");
	assign_coeffs_p(poly1);
	
	printf("Enter degree of polynomial 2\n");
	scanf("%d", &deg2);
	poly2 = initialize_p(deg2);
	printf("Enter coefficients of polynomial in descending degree,including zero coefficients\n");
	assign_coeffs_p(poly2);
	
	printf("Enter degree of polynomial c\n");
	scanf("%d", &degc);
	c = initialize_p(degc);
	printf("Enter coefficients of polynomial in descending degree,including zero coefficients\n");
	assign_coeffs_p(c);


	result = extended_euclidean_diophantine(poly1, poly2,c);
	display_p(result[0]);
	display_p(result[1]);
	
return 0;	
}


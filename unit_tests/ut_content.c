#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../polynomials.h"

int main()
{
	poly * result; 
	frac *content;

result = initialize_p(3);
assign_coeffs_p(result);

content = content_p(result);

print_f(content);




return 0;
}

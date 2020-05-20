#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../polynomials.h"

int main()
{
	poly * result; 
	poly *derv;

result = initialize_p(3);
assign_coeffs_p(result);

derv = derivative_p(result);

display_p(derv);




return 0;
}

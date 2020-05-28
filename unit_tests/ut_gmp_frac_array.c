#include <stdio.h>
#include "../frac.h"

int main()
{

	char *string1 = "5457", *string2 = "2456", *string3 = "795",  *string4 = "45665";
	char *string5 ="3544", *string6 ="36549";
	frac **fractions, **result;
	mpz_t z, y, x, w, u, v;
	int i;

	//convert strings to gmp ints
	mpz_init_set_str(z, string1, 10);
	mpz_init_set_str(y, string2, 10);
	mpz_init_set_str(x, string3, 10);
	mpz_init_set_str(w, string4, 10);
	mpz_init_set_str(u, string5, 10);
	mpz_init_set_str(v, string6, 10);

	
	//create fraction array
	fractions = (frac **)calloc(6, sizeof(frac *));

	fractions[0] = init_f(z,y);
	fractions[1] = init_f(u,x);
	fractions[2] = init_f(w,v);
	fractions[3] = init_f(y,v);
	fractions[4] = init_f(z,x);
	fractions[5] = init_f(w,z);

	for(i=0; i<6; ++i) {
		print_f(fractions[i]);
		printf("\n");
	}

	printf("\n");

	//test copy_aray_f
	result = copy_array_f(fractions, 6);
	for(i=0; i<6; ++i) {
		print_f(result[i]);
		printf("\n");
	}

	printf("\n");

	//test gcd_array_f
	print_f(gcd_array_f(5, fractions));
	printf("\n");

	//test lcm_array_f
	print_f(lcm_array_f(5, fractions));
	printf("\n");	

			
	return 0;
}

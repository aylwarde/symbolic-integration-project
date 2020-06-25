#ifndef LATEX_H
#define LATEX_H

#include <stdlib.h>

#include "polynomials.h"
#include "rationalfns.h"
#include "bivariate_poly.h"
#include "trivariate_poly.h"
#include "lazard_rioboo_trager.h"
#include "logtoatantri.h"

typedef struct STRING {
	int capacity;
	char *string;
} STRING;

//Fn Defs
STRING *make_string();
void append_to_string();
char *print_monomial();

STRING *latex_poly();
STRING *latex_rational();

STRING *integrate_string();
STRING *integrate_poly();
STRING *integrate_rational();

STRING *sum_over_poly_sol();

STRING *latex_bivariate_poly();
STRING *latex_field_extension();
STRING *latex_biv_rational();
STRING *latex_trivariate_poly();

STRING *log_string();
STRING *log_poly();
STRING *log_rational();
STRING *log_bivariate_poly();
STRING *latex_Logs();

STRING *latex_atan_tri();

int write_to_file();

//End of Fn Defs

//make a string of default length 200 chars
STRING *make_string() {
	STRING *str = (STRING *)calloc(1, sizeof(STRING));
	str->capacity = 200;
	char *contents = (char *)calloc(str->capacity, sizeof(char));
	str->string = contents;
	return str;
}

//append a character string to a string structure (function handles memory allocation)
void append_to_string(STRING *str, char *words) { 
	int wordslen;
	wordslen = strlen(words);

	//if words does not exceed capacity of string
	if(strlen(str->string) + wordslen +1 <= str->capacity) {
		//if original string is empty
		if(strlen(str->string)==0) {
			snprintf(str->string, wordslen+1, "%s", words);
		}
		
		//if original string is non-empty
		else {
			snprintf(&(str->string[strlen(str->string)]), wordslen +2, " %s", words);
		}
	}

	//if words does exceed capacity of string
	else { 
		char *newstr = (char *)calloc(strlen(str->string) + wordslen + 200, sizeof(char));
		//if original string is empty
		if(strlen(str->string)==0) {
			snprintf(newstr, wordslen+1, "%s", words);
		}

		//if original string is non-empty
		else {
			snprintf(newstr, strlen(str->string) +1 +wordslen +1, "%s %s", str->string,
					words);
		}

		str->capacity = strlen(str->string ) +1 +wordslen +200; 
		
		free(str->string);
		str->string = newstr;
	}
}

char *latex_f(frac *frac_a, char *leftbinder, char *rightbinder) {

	STRING *output;
	output = make_string();
	append_to_string(output, leftbinder);
	if(mpz_cmp_ui(frac_a->denom, 1)!=0) {
		append_to_string(output, "\\frac{");
	}
	append_to_string(output, (char *)mpz_get_str(NULL, 10, frac_a->num));
	if(mpz_cmp_ui(frac_a->denom, 1)!=0) {
		append_to_string(output, "}{");
		append_to_string(output, (char *)mpz_get_str(NULL, 10, frac_a->denom));
		append_to_string(output, "}");
	}
	append_to_string(output, rightbinder);

	return output->string;
}


//returns character string: (var)^deg,
char *print_monomial(char *var, int deg) {
	
	STRING *string_result;
	string_result = make_string();

	if(deg==0) {
		//do nothing	
	}

	else {
		append_to_string(string_result, var);

		if(deg != 1) {
			append_to_string(string_result, "^");
			append_to_string(string_result, "{");
			int length = snprintf(NULL, 0, "%d", deg);
			char *str = (char *)calloc(1, length+1);
			snprintf(str, length +1, "%d", deg);
			append_to_string(string_result, str);
			free(str);
			append_to_string(string_result, "}");
		}
	}

	return string_result->string;
}

//create a latex polynomial string using latex_f in frac_h header, and decide variable
STRING *latex_poly(poly *poly1, char *var, char *leftbinder, char *rightbinder) {
	
	STRING *output;
	output = make_string();

	char minus[] = "-", plus[] = "+";

	//create one frac;
	mpz_t one; mpz_init_set_ui(one, 1);
	frac *onefrac;
	onefrac = init_f(one, one);

	append_to_string(output, leftbinder);

	//leading coefficient (we'll assume the poly has been stripped)
	//if leading coeff is negative we print minus sign then absolute value
	if(mpz_sgn(poly1->coefficients[0]->num)<0) {
		append_to_string(output, minus);
	}
	//dont print frac if it is one
	if(!equals_f(abs_f(poly1->coefficients[0]), onefrac) || poly1->deg==0) {
		append_to_string(output, latex_f(abs_f(poly1->coefficients[0]), "", ""));
	}
	append_to_string(output, print_monomial(var, poly1->deg));

	int i;

	//print all subsequent terms
	for(i=1; i<=poly1->deg; ++i) {
		if(!zero_f(poly1->coefficients[i])) {
			//add plus or minus depending on sign of term
			if(mpz_sgn(poly1->coefficients[i]->num)<0) {
				append_to_string(output, minus);
			}
			
			else {
				append_to_string(output, plus);
			}
			
			//add frac and monomial
			//dont print frac if one...unless the constant term is one
			if(!equals_f(abs_f(poly1->coefficients[i]), onefrac) || i==poly1->deg) {
				append_to_string(output, 
						latex_f(abs_f(poly1->coefficients[i]), "", ""));
			}
			append_to_string(output, print_monomial(var, poly1->deg-i));
		}
	}

	mpz_clear(one);

	append_to_string(output, rightbinder);
	return output;
}

//create a latex rational polynomial, with var preferred variable (assume denom non zero)
STRING *latex_rational(rational *rat_poly, char *var, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();

	append_to_string(output, leftbinder);
	
	//if denominator is constant multiply numerator by recip of denom and return latex_poly(num)
	if(rat_poly->denom->deg==0) {
		rat_poly->num = scale_p(reciprocal_f(rat_poly->denom->coefficients[0]), 
				rat_poly->num);
		free(output);
		output = latex_poly(rat_poly->num, var, leftbinder, rightbinder);
	}

	else {
		//if leading coeff of denom is neg, negate both num and denom
		if(mpz_sgn(rat_poly->denom->coefficients[0]->num)<0) {
			free(output);
			rat_poly->num = negative_p(rat_poly->num);
			rat_poly->denom = negative_p(rat_poly->denom);
			output = latex_rational(rat_poly, var, leftbinder, rightbinder);
		}

		else {
			
			append_to_string(output, "\\frac{");
			//append numerator
			append_to_string(output, (latex_poly(rat_poly->num, var, "", ""))->string);
			append_to_string(output, "}{");
			//append denominator
			append_to_string(output, (latex_poly(rat_poly->denom, var, "", ""))->string);
			append_to_string(output, "}");
			append_to_string(output, rightbinder);
		}
	}

	return output;
}

//wrap a integral sign and dvar around string
STRING *integrate_string(STRING *str, char *var, char *leftbinder, char *rightbinder ) {

	STRING *output;
	output = make_string();

	append_to_string(output, leftbinder);
	append_to_string(output, "\\int");
	append_to_string(output, str->string);
	append_to_string(output, "\\ d");
	append_to_string(output, var);
	append_to_string(output, rightbinder);

	return output;
}

//return latex string of integral of poly over particular variable, w/ left/right bindings
STRING *integrate_poly(poly *poly1, char *var, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder);
	append_to_string(output, 
			(integrate_string(latex_poly(poly1, var, "", ""), var, "","")->string));
	append_to_string(output, rightbinder);
	
	return output;
}

//return latex string of integral of poly rational particular variable, w/ left/right bindings
STRING *integrate_rational(rational *rat_poly, char *var, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder);
	append_to_string(output, (integrate_string(
				latex_rational(rat_poly, var, "", ""), var, "","")->string));
	append_to_string(output, rightbinder);
	
	return output;
}

//return latex string for sum over sols of polynomial
STRING *sum_over_poly_sol(poly *poly1, char *var, char *leftbinder, char *rightbinder) {

	STRING *output;
	output = make_string();

	append_to_string(output, leftbinder);
	append_to_string(output, "\\sum_{");
	append_to_string(output, var);
	append_to_string(output, "|");
	if(mpz_sgn(poly1->coefficients[0]->num)<0) {
		append_to_string(output, (latex_poly(negative_p(poly1), var, "", ""))->string);
	}
	else {
		append_to_string(output, (latex_poly(poly1, var, "", ""))->string);
	}
	append_to_string(output, "=0}");
	append_to_string(output, rightbinder);
	
	return output;
}

//print a bivariate poly in Q(var1)(var2) ...so var2 is the dominant variable, so to say
STRING *latex_bivariate_poly(bpoly *b_poly, char *var1, char *var2, char *leftbinder, char *rightbinder) {
	STRING *output;
	output = make_string();
	
	//create (neg)/one frac;
	mpz_t one; mpz_init_set_ui(one, 1);
	mpz_t negone; mpz_init(negone);
	mpz_neg(negone, one);
	frac *onefrac, *negonefrac;
	onefrac = init_f(one, one);
	negonefrac = init_f(negone, one);
	
	append_to_string(output, leftbinder); 
	
	int i=0;
	while(i<=b_poly->deg) {

		if(!zero_p(b_poly->pcoefficients[i])) {
			if(!monomial_p(b_poly->pcoefficients[i])) {
		
				if(i!=0) {
					append_to_string(output, "+");
				}

				append_to_string(output,
					(latex_poly(b_poly->
						    pcoefficients[i], var1, "(",")"))->string);
			}
			//no brackets for monomials
			else {
			
				//add plus sign	
				if(i!=0 && mpz_sgn(b_poly->
							pcoefficients[i]->coefficients[0]->num)>0) {
					append_to_string(output, "+");
				}

				//print minus sign if monomial is neg one (and not const term)
				if(equals_f(b_poly->pcoefficients[i]->coefficients[0],
							negonefrac) && i!=b_poly->deg) {
					append_to_string(output, "-");
				}

				//dont print monomial if its one
				if(!equals_f(abs_f(
					b_poly->pcoefficients[i]->coefficients[0]), onefrac) || 
						i==b_poly->deg) {
				append_to_string(output,
					(latex_poly
					 (b_poly->pcoefficients[i], var1, "",""))->string);
				}
			}

			append_to_string(output, print_monomial(var2, b_poly->deg-i));
		}
			++i;
	}

	append_to_string(output, rightbinder);
	return output;
}

//print a field ext poly in Q(var1)[var2]
STRING *latex_field_extension(field_extension *poly, char *var1, char *var2, char *leftbinder, char *rightbinder) {
	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder); 
	
	int i=0;
	while(i<=poly->deg) {

		if(!zero_p(poly->rcoefficients[i]->num)) {
				if(i!=0) {
					append_to_string(output, "+");
				}
				append_to_string(output,
					(latex_rational(poly->
						    rcoefficients[i], var1, "",""))->string);

			append_to_string(output, print_monomial(var2, poly->deg-i));
		}
			++i;
	}

	append_to_string(output, rightbinder);
	return output;
}

//create a latex biv_rational polynomial (assume denom non zero)
STRING *latex_biv_rational(biv_rational *brat, 
		char *var1, char *var2, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();

	//if denominator is onebpoly then do not print
	bpoly *onebp = one_bp();
	if(equals_fe(brat->denom, bp_to_fe(onebp))) {
		append_to_string(output, latex_field_extension(brat->num, 
					var1, var2, leftbinder, rightbinder)->string);
			}
	else {
		append_to_string(output, leftbinder);
		append_to_string(output, "\\frac{");
		//append numerator
		append_to_string(output, 
				(latex_field_extension(brat->num, var1, var2, "", ""))->string);
		append_to_string(output, "}{");
		//append denominator
		append_to_string(output, 
				(latex_field_extension(brat->denom, var1, var2, "", ""))->string);
		append_to_string(output, "}");
		append_to_string(output, rightbinder);
		
		}

	return output;
}
//print a trivariate poly in Q(var1)(var2)[var3]
STRING *latex_trivariate_poly(tpoly *t_poly, char *var1, char *var2, char *var3, char *leftbinder, char *rightbinder) {
	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder); 
	
	int i=0;
	while(i<=t_poly->deg) {

		if(!zero_fe(t_poly->brcoefficients[i]->num)) {
				if(i!=0) {
					append_to_string(output, "+");
				}
				if(i==t_poly->deg) {
					append_to_string(output,(latex_biv_rational(t_poly->
						 brcoefficients[i], var1, var2, "",""))->string);
				}
				
				else {
					append_to_string(output,(latex_biv_rational(t_poly->
						 brcoefficients[i], var1, var2, "(",")"))->string);
				}

			append_to_string(output, print_monomial(var3, t_poly->deg-i));
		}
			++i;
	}

	append_to_string(output, rightbinder);
	return output;
}

//wrap a log argument around string ( i wont set any conditions on the argument for now....)
STRING *log_string(STRING *str, char *leftbinder, char *rightbinder ) {

	STRING *output;
	output = make_string();

	append_to_string(output, leftbinder);
	append_to_string(output, "\\log[");
	append_to_string(output, str->string);
	append_to_string(output, "]");
	append_to_string(output, rightbinder);

	return output;
}

//return latex string of log of poly over particular variable, w/ left/right bindings
STRING *log_poly(poly *poly1, char *var, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder);
	append_to_string(output, 
			(log_string(latex_poly(poly1, var, "", ""), "","")->string));
	append_to_string(output, rightbinder);
	
	return output;
}

//return latex string of log of poly rational particular variable, w/ left/right bindings
STRING *log_rational(rational *rat_poly, char *var, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder);
	append_to_string(output, (log_string(
				latex_rational(rat_poly, var, "", ""), "","")->string));
	append_to_string(output, rightbinder);
	
	return output;
}


//return latex string of log of bivariate poly in Q(var1)(var2), w/ left/right bindings
STRING *log_bivariate_poly(bpoly *b_poly, char *var1, char *var2, char *leftbinder, char *rightbinder) { 

	STRING *output;
	output = make_string();
	
	append_to_string(output, leftbinder);
	append_to_string(output, (log_string(
			latex_bivariate_poly(b_poly, var1, var2, "", ""), "","")->string));
	append_to_string(output, rightbinder);
	
	return output;
}

//takes a Log struct and returns string with required latex-formatted output
//var 1 should be "a" in K (field) closure, var2 the indeterminate our polys are defined over eg x
STRING *latex_Logs(logpart *input, char *var1, char *var2, char *leftbinder, char *rightbinder) {

	int i;
	STRING *output;
	output = make_string();

	append_to_string(output, leftbinder);
	for(i=0; i<input->num; ++i) {
		append_to_string(output, sum_over_poly_sol(input->roots[i], var1, "", "")->string);
		append_to_string(output, var1);
		append_to_string(output,
				log_bivariate_poly(input->arguments[i], var1, var2, "", "")->string);
		if(i != input->num-1) {
			append_to_string(output, "+");
		}
	}
	append_to_string(output, rightbinder);

	return output;
}
	

STRING *latex_atan_tri(atan_tri *input, char *var1, char *var2, char *var3, char *leftbinder, char *rightbinder) {

	STRING *output;
	output = make_string();
	tpoly *onetp = one_tp();

	append_to_string(output, leftbinder);
	if(!equals_tp(input->denom, onetp)) {
		append_to_string(output, "\\arctan(\\frac{");
		append_to_string(output, 
			latex_trivariate_poly(input->num, var1, var2, var3, "", "}")->string);
		append_to_string(output, 
			latex_trivariate_poly(input->denom, var1, var2, var3, "{", "})")->string);
	}

	else {
		append_to_string(output, 
				latex_trivariate_poly
				(input->num, var1, var2, var3, "\\arctan(", ")")->string);
	}
	append_to_string(output, rightbinder);
	return output;
}


//write a given string to a file, returns 1
int write_to_file(char *filename, STRING *str) { 
	
	FILE *file;
	//opens in same directory 
	file = fopen(filename, "w"); 
	fprintf(file, "%s", str->string); 
	fclose(file);

	return 1;
}







#endif /* LATEX_H */

#ifndef RINTEGRATION_H
#define RINTEGRATION_H

#include "hermite_reduce.h"
#include "lazard_rioboo_trager.h"
#include "logtoatantri.h"

typedef struct real_transcendental_part {
	int num; //number of distinct squarefree factors we sum over
	bpoly ***complex_roots; //arrays of length 2
	tpoly **magnitude;
	atan_tri ***arctan_arguments; // arrays of length lens[i]
	int *lens;
} realtrans;


void integrate_r(rational *rpoly1,rational **g,poly **Q,logpart **Rh)
{
  
  rational **hermite, *rat;
  poly **polydiv;
  void *result;

  hermite = hermite_reduce(rpoly1);
  polydiv = divide_p(hermite[1]->num,hermite[1]->denom);
  *g = hermite[0];
  *Q = integrate_p(polydiv[0]);

  if(zero_p(polydiv[1]))
    {
      
      *Rh = NULL;

      return;
      
    }
  else
    {
      rat = init_r(polydiv[1],hermite[1]->denom);
      
      *Rh = int_rational_log_part(rat);

      return;
    }
}

realtrans *logpart_to_realtrans(logpart *input) {

	realtrans *result = (realtrans *)calloc(1,
			sizeof(realtrans));
	
	result->num = input->num;
	result->lens = (int *)calloc(result->num, sizeof(int));
	result->complex_roots = (bpoly ***)calloc(result->num, sizeof(bpoly **));
	result->magnitude = (tpoly **)calloc(result->num, sizeof(tpoly *));
	result->arctan_arguments = (atan_tri ***)calloc(result->num, sizeof(atan_tri **));

	tpoly **complex_log_arguments;

	for(int i=0; i<result->num; ++i) {
		
		result->complex_roots[i] = poly_complexify(input->roots[i]);
		complex_log_arguments = bpoly_complexify(input->arguments[i]);
		
		result->magnitude[i] = add_tp(pow_tp(complex_log_arguments[0], 2),
			       	pow_tp(complex_log_arguments[1], 2));

		if(zero_tp(complex_log_arguments[1])) {
			result->arctan_arguments[i] = NULL;
		}
		else {
			result->arctan_arguments[i] = logtoatantri(complex_log_arguments[0], 
				complex_log_arguments[1], &result->lens[i]);
		}
		
		free_tp(complex_log_arguments[0]);
		free_tp(complex_log_arguments[1]);
		free(complex_log_arguments);
	}

	return result;
}

#endif /* RINTEGRATION_H */

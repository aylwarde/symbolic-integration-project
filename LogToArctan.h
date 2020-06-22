#ifndef LOGTOARCTAN_H
#define LOGTOARCTAN_H

#include "Rintegration.h"
#include "euclidean_algorithms.h"
#include "rationalfns.h"

typedef struct arctan {
  int num;
  rational **arguments;

} arctan;



arctan *initialize_arc();
arctan *log_to_arc();
void free_arc();
void print_arc();

//send arguments A B C D as specified in bronstien (can be edited to take a log and using extra coefficent to encode which part is compelx)



void print_arc(arctan *tan)
{
  int i;
  
  for(i=0;i<tan->num;++i)
    {
  
  printf("   2 arctan(");
  print_r(tan->arguments[i]);
    }
}

void free_arc(arctan *tan)
{
  int i;
  for(i=0;i<tan->num;++i)
    {
      free_r(tan->arguments[i]);
 
    }
  free(tan);
}

  
  
arctan *log_to_arc(poly *A ,poly *B)
{
  poly **euclid,*a1,*b1;
  int i=0;

  arctan *result = (arctan *)calloc(1,sizeof(arctan));

 
  while(!zero_p(divide_p(B,A)[1]))
    {
      if(A->deg<B->deg)
	{
	  a1=A;
	  b1=B;
	  B=a1;
	  A=negative_p(b1);
            
      
	}
      else
	{
	  //initializing the i^th term of the arctan part
	  result->num = i;
          result-> arguments = (arctan *)realloc(result,i);

      

	  euclid = extended_euclidean(B,negative_p(A));
	  result->arguments[i] =init_r( add_p(multiply_p(A,euclid[0]),multiply_p(B,euclid[1])), euclid[2]);

	  ++i;
	}
    }
  //initialize the i^th term of the arctan part
  result->arguments = (arctan *)realloc(result,i);
  result->num =i;
  result->arguments[i] = init_r(A,B);

  return result;
}
     
#endif

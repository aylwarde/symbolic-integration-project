#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

long gcd_z();
long lcm_z();
long pow_z();

long gcd_z(long a, long b) {
  
  long r = 0;
  long q = 0;
  
  if(a==b) {
    
    return a;
    
  } else {
    
    if(a==0 || b==0) {
      
      return MAX(a,b);
      
    } else {
      
      return gcd_z(MIN(a,b),MAX(a,b)%MIN(a,b));
      
    }
  }
  return -1;
}

long lcm_z(long a, long b) {

  return (a*b)/gcd_z(a,b);

}

long pow_z(long b, long exp) {

  if (exp == 0) {

    return 1;
    
  } else if ( exp % 2) {

    return b * pow_z(b, exp - 1);
    
  } else {

    long temp = pow_z(b, exp / 2);
    return temp * temp;
  }
}

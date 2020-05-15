#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int gcd();

int main() {
  printf("%d\n", gcd(3*4*5,3*7));
    return 0;
}

int gcd(int a, int b) {
  printf("gcd(%d,%d)\n", a, b);
  int r = 0;
  int q = 0;
  if(a==b) {
    return a;
  } else {
    if(a==0 || b==0) {
      return MAX(a,b);
    } else {
      return gcd(MIN(a,b),MAX(a,b)%MIN(a,b));
    }
  }
  return -1;
}

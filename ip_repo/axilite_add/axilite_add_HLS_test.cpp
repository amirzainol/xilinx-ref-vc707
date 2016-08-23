#include <stdio.h>

void axilite_add(unsigned int *a, unsigned int *b, unsigned int *c);

int main()
{

  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
  unsigned int sw_result;


  printf("HLS AXI-Lite Example\n");
  printf("Function c += a + b\n");
  printf("Initial values a = 5, b = 10, c = 0\n");

  a = 5;
  b = 10;
  c = 0;
  d = 0;

  axilite_add(&a,&b,&c);
  d += a + b;

  printf("HW result = %d\n",c);
  printf("SW result = %d\n",d);

  if(d == c){
    printf("Success SW and HW results match\n");
    return 0;
  }
  else{
    printf("ERROR SW and HW results mismatch\n");
    return 1;
  }
}
  
 

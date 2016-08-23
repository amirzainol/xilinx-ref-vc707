#include <stdio.h>

void axilite_add(unsigned int *a, unsigned int *b, unsigned int *c)
{

#pragma HLS INTERFACE s_axilite port=a bundle=BUS_A
#pragma HLS INTERFACE s_axilite port=b bundle=BUS_A
#pragma HLS INTERFACE s_axilite port=c register bundle=BUS_A
#pragma HLS INTERFACE s_axilite port=return bundle=BUS_A

  *c += *a + *b;

}
  
  

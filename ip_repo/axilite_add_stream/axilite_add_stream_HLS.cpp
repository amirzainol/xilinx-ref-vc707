#include <stdio.h>

void axilite_add_stream (	unsigned int *a,
							unsigned int *b,
							unsigned int *c,
							unsigned int input1[50],
							unsigned int input2[50],
							unsigned int output[50])
{

#pragma HLS INTERFACE s_axilite port=a bundle=BUS_CMD
#pragma HLS INTERFACE s_axilite port=b bundle=BUS_CMD
#pragma HLS INTERFACE s_axilite port=c register bundle=BUS_CMD
#pragma HLS INTERFACE s_axilite port=return bundle=BUS_CMD

#pragma HLS INTERFACE axis port=input1
#pragma HLS INTERFACE axis port=input2
#pragma HLS INTERFACE axis port=output

  int i;
  unsigned int var_a, var_b;

  var_a = *a;
  var_b = *b;

  for(i = 0; i < 50; i++){
	  output[i] = input1[i] + input2[i] + var_a + var_b;
  }

  *c = 1;

}

#include <stdio.h>

void axilite_add_stream(	unsigned int *a,
							unsigned int *b,
							unsigned int *c,
							unsigned int input1[50],
							unsigned int input2[50],
							unsigned int output[50]);

int main()
{

  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
  unsigned int sw_result;

  unsigned int input1[50];
  unsigned int input2[50];
  unsigned int temp[50];
  unsigned int output[50];

  int i;

  a = 5;
  b = 10;
  c = 0;

  //Put data into A
  for(i=0; i < 50; i++){
	  input1[i] = i;
  }

  //Put data into B
  for(i=0; i < 50; i++){
	  input2[i] = i;
  }

  //Call the hardware function
  axilite_add_stream(&a, &b, &c, input1, input2, output);

  //Run a software version of the hardware function to validate results
  for(i=0; i < 50; i++){
    temp[i] = input1[i] + input2[i] + a + b;
  }

  //Compare results
  for(i=0; i < 50; i++){
    if(output[i] != temp[i]){
      printf("ERROR HW and SW results mismatch\n");
      return 1;
    }
  }
  printf("Success HW and SW results match\n");

  return 0;

}



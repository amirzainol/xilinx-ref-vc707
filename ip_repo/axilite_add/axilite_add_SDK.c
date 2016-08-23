#include <stdio.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xaxilite_add.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void print(char *str);

// the HW Acc instance
XAxilite_add XAxilite_add_HW_dev;
XAxilite_add_Config XAxilite_add_HW_config = { 0, AXILITE_ADD_BASE_ADDRESS };

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{

	int Status;

	// Run the HW Accelerator;
	Status = XAxilite_add_CfgInitialize(&XAxilite_add_HW_dev, 
	                                    &XAxilite_add_HW_config);
	if(Status != XST_SUCCESS){
		xil_printf("Error: example setup failed\r\n");
		return XST_FAILURE;
	}
    // The interrupt is not connected.
	XAxilite_add_InterruptGlobalDisable(&XAxilite_add_HW_dev);
	XAxilite_add_InterruptDisable(&XAxilite_add_HW_dev, 1);

	// Send Data to A and B
	XAxilite_add_Set_a(&XAxilite_add_HW_dev, 0x10001000);
	XAxilite_add_Set_b(&XAxilite_add_HW_dev, 0xCEADAEEF);

    // Start the accelerator
	XAxilite_add_Start(&XAxilite_add_HW_dev);

	// Print the result from C
	xil_printf("\r\n%08X", XAxilite_add_Get_c_o(&XAxilite_add_HW_dev));

    return 0;
}

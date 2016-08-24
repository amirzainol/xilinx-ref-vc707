#include <stdio.h>
#include "platform.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xaxilite_add_stream.h"

/*
 * Device hardware build related constants.
 */

#define DMA_DEV_ID_A		XPAR_AXI_DMA_A_DEVICE_ID
#define DMA_DEV_ID_B		XPAR_AXI_DMA_B_DEVICE_ID
#define DMA_DEV_ID_R		XPAR_AXI_DMA_R_DEVICE_ID

#define MEM_BASE_ADDR		(XPAR_MIG_7SERIES_0_BASEADDR + 0x10000000)

#define TX_BUFFER_BASE_A	(MEM_BASE_ADDR + 0x00100000)
#define TX_BUFFER_BASE_B	(MEM_BASE_ADDR + 0x00200000)
#define RX_BUFFER_BASE_R	(MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)

#define MAX_PKT_LEN_WORDS	50
#define MAX_PKT_LEN			MAX_PKT_LEN_WORDS*4

#define NUMBER_OF_TRANSFERS	1

/************************** Function Prototypes ******************************/

int XAxiDma_SimplePollExample(u16 DeviceId);
static int CheckData(void);

/************************** Variable Definitions *****************************/
/*
 * Device instance definitions
 */
XAxiDma AxiDmaA;
XAxiDma AxiDmaB;
XAxiDma AxiDmaR;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// the HW Acc instance
XAxilite_add_stream XAxilite_add_HW_dev;
XAxilite_add_stream_Config XAxilite_add_HW_config = { 0, XPAR_AXILITE_ADD_STREAM_0_S_AXI_BUS_CMD_BASEADDR };

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
	int Status;

    init_platform();
	//xil_printf("\r\n--- Entering main() --- \r\n");

	/* Run the poll example for simple transfer */
	Status = XAxiDma_SimplePollExample(DMA_DEV_ID_A);

	if (Status != XST_SUCCESS) {

		xil_printf("Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Passed\r\n");

	//cleanup_platform();

	xil_printf("--- Exiting main() --- \r\n");

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* The example to do the simple transfer through polling. The constant
* NUMBER_OF_TRANSFERS defines how many times a simple transfer is repeated.
*
* @param	DeviceId is the Device Id of the XAxiDma instance
*
* @return
*		- XST_SUCCESS if example finishes successfully
*		- XST_FAILURE if error occurs
*
* @note		None
*
*
******************************************************************************/
int XAxiDma_SimplePollExample(u16 DeviceId)
{
	XAxiDma_Config *CfgPtr;
	int Status;
	int Tries = NUMBER_OF_TRANSFERS;
	u32 i;
	u32 *TxBufferPtrA;
	u32 *TxBufferPtrB;
	u32 *RxBufferPtrR;

	TxBufferPtrA = (u32 *)TX_BUFFER_BASE_A;
	TxBufferPtrB = (u32 *)TX_BUFFER_BASE_B;
	RxBufferPtrR = (u32 *)RX_BUFFER_BASE_R;

	// Run the HW Accelerator;
	Status = XAxilite_add_stream_CfgInitialize(&XAxilite_add_HW_dev,
										&XAxilite_add_HW_config);
	if(Status != XST_SUCCESS){
		xil_printf("Error: example setup failed\r\n");
		return XST_FAILURE;
	}
    // The interrupt is not connected.
	XAxilite_add_stream_InterruptGlobalDisable(&XAxilite_add_HW_dev);
	XAxilite_add_stream_InterruptDisable(&XAxilite_add_HW_dev, 1);

	// Send Data to A and B
	XAxilite_add_stream_Set_a(&XAxilite_add_HW_dev, 4);
	XAxilite_add_stream_Set_b(&XAxilite_add_HW_dev, 8);

    // Start the accelerator
	XAxilite_add_stream_Start(&XAxilite_add_HW_dev);

	Status = init_dma_A();
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	Status = init_dma_B();
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	Status = init_dma_R();
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	for(i = 0; i < MAX_PKT_LEN_WORDS; i ++) {
		TxBufferPtrA[i] = i;	// initialize TxBufferPtr
		TxBufferPtrB[i] = i;	// initialize TxBufferPtr
	}

	Xil_DCacheFlushRange((u32)TxBufferPtrA, MAX_PKT_LEN);
	Xil_DCacheFlushRange((u32)TxBufferPtrB, MAX_PKT_LEN);

	for(i = 0; i < Tries; i ++) {

		Status = XAxiDma_SimpleTransfer(&AxiDmaR,(u32) RxBufferPtrR,
					MAX_PKT_LEN, XAXIDMA_DEVICE_TO_DMA);

		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		Status = XAxiDma_SimpleTransfer(&AxiDmaA,(u32) TxBufferPtrA,
					MAX_PKT_LEN, XAXIDMA_DMA_TO_DEVICE);

		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		Status = XAxiDma_SimpleTransfer(&AxiDmaB,(u32) TxBufferPtrB,
					MAX_PKT_LEN, XAXIDMA_DMA_TO_DEVICE);

		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		//while (XAxiDma_Busy(&AxiDmaR,XAXIDMA_DEVICE_TO_DMA)) {}

		Status = CheckData();
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

	}

	/* Test finishes successfully
	 */
	return XST_SUCCESS;
}

/*****************************************************************************/
/*
*
* This function checks data buffer after the DMA transfer is finished.
*
* @param	None
*
* @return
*		- XST_SUCCESS if validation is successful.
*		- XST_FAILURE otherwise.
*
* @note		None.
*
******************************************************************************/
static int CheckData(void)
{
	u32 *RxPacket;
	u32 Index = 0;

	RxPacket = (u32 *) RX_BUFFER_BASE_R;

	//RxPacket[MAX_PKT_LEN_WORDS-1] = MAX_PKT_LEN_WORDS-1;
	/* Invalidate the DestBuffer before receiving the data, in case the
	 * Data Cache is enabled
	 */
	Xil_DCacheInvalidateRange((u32)RxPacket, MAX_PKT_LEN);

	//xil_printf("Last RxBuffer value: %d\r\n",RxPacket[MAX_PKT_LEN_WORDS-1]);
	//xil_printf("Data received: ");
	for(Index = 0; Index < MAX_PKT_LEN_WORDS; Index++) {
		if((Index >= 0) && (Index < 5))
			xil_printf("%d ", (unsigned int)RxPacket[Index]);
		if((Index >= MAX_PKT_LEN_WORDS-5) && (Index < MAX_PKT_LEN_WORDS))
			xil_printf("%d ", (unsigned int)RxPacket[Index]);
	}
	/*
	xil_printf("\r\nLast received: ");
	for(Index = MAX_PKT_LEN_WORDS-5; Index < MAX_PKT_LEN_WORDS; Index++) {
		xil_printf("0x%X ", (unsigned int)RxPacket[Index]);
	}
	*/
	xil_printf("\r\n");

	return XST_SUCCESS;
}

int init_dma_A() {

	XAxiDma_Config *CfgPtr;
	int status;

	CfgPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_A_DEVICE_ID);
	if(!CfgPtr){
		//print("Error looking for AXI DMA config\n\r");
		return XST_FAILURE;
	}
	status = XAxiDma_CfgInitialize(&AxiDmaA,CfgPtr);
	if(status != XST_SUCCESS){
		//print("Error initializing DMA\n\r");
		return XST_FAILURE;
	}
	//check for scatter gather mode
	if(XAxiDma_HasSg(&AxiDmaA)){
		//print("Error DMA configured in SG mode\n\r");
		return XST_FAILURE;
	}
	// Disable interrupts, we use polling mode
	XAxiDma_IntrDisable(&AxiDmaA, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDmaA, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS;
}

int init_dma_B() {

	XAxiDma_Config *CfgPtr;
	int status;

	CfgPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_B_DEVICE_ID);
	if(!CfgPtr){
		//print("Error looking for AXI DMA config\n\r");
		return XST_FAILURE;
	}
	status = XAxiDma_CfgInitialize(&AxiDmaB,CfgPtr);
	if(status != XST_SUCCESS){
		//print("Error initializing DMA\n\r");
		return XST_FAILURE;
	}
	//check for scatter gather mode
	if(XAxiDma_HasSg(&AxiDmaB)){
		//print("Error DMA configured in SG mode\n\r");
		return XST_FAILURE;
	}
	// Disable interrupts, we use polling mode
	XAxiDma_IntrDisable(&AxiDmaB, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDmaB, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS;
}

int init_dma_R() {

	XAxiDma_Config *CfgPtr;
	int status;

	CfgPtr = XAxiDma_LookupConfig(XPAR_AXI_DMA_R_DEVICE_ID);
	if(!CfgPtr){
		//print("Error looking for AXI DMA config\n\r");
		return XST_FAILURE;
	}
	status = XAxiDma_CfgInitialize(&AxiDmaR,CfgPtr);
	if(status != XST_SUCCESS){
		//print("Error initializing DMA\n\r");
		return XST_FAILURE;
	}
	//check for scatter gather mode
	if(XAxiDma_HasSg(&AxiDmaR)){
		//print("Error DMA configured in SG mode\n\r");
		return XST_FAILURE;
	}
	// Disable interrupts, we use polling mode
	XAxiDma_IntrDisable(&AxiDmaR, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDmaR, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	return XST_SUCCESS;
}

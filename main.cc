/*
 * Empty C++ Application
 */
#include "xil_exception.h"
#include "xil_cache.h"
#include "xparameters.h"
#include <iostream>
using namespace std;
int main()
{
	//declaring pointers to the direct adresses as stated in xparameters.h
	u32* cdma_address = (u32*)XPAR_AXI_CDMA_0_BASEADDR;
	u32* source_address = (u32*)XPAR_PS7_DDR_0_S_AXI_HP0_BASEADDR;
	u32* destination_address = (u32*)XPAR_PS7_DDR_0_S_AXI_HP2_BASEADDR;


	//initializing the source address to not be undefined on start
	//loading values into source array
	*source_address = 0;
	*(source_address+1) = 12;
	*(source_address+2) = 22;
	*(source_address+3) = 32;
	*(source_address+4) = 42;
	*(source_address+5) = 52;
	//cout << std::hex << *cdma_address <<endl;

	//initializing the destination address
	*destination_address = -1;
	*(destination_address+1) = -1;
	*(destination_address+2) = -1;
	*(destination_address+3) = -1;
	*(destination_address+4) = -1;
	*(destination_address+5) = -1;
	//cout << std::hex << *cdma_address <<endl;

	*cdma_address = 0x04; //control register addressing (reset it) and turn on simple mode no other interrupts are activated
	//cout << std::hex << *cdma_address <<endl;
	*cdma_address = 0x00; //lift reset
	//cout << std::hex << *cdma_address <<endl;
	//CDMASR.idle = 1 means that register is idle so we should wait while the value is 0

	while(!(*(cdma_address+1) & (1<<1))){} //wait until bit is 1
	*(cdma_address+6) = (u32)source_address; //assigning source address to that of the preset address
	*(cdma_address+8) = (u32)destination_address; //destination address cdma -> destination address preset
	//cout << std::hex << *cdma_address <<endl;
	//flush cache
	Xil_DCacheFlush();


	*(cdma_address+10) = 0x14; //byte to transfer register ... transfering 4 words indexes(0,1,2,3)
	//cout << std::hex << *cdma_address <<endl;
	while(!(*(cdma_address+1) & (1<<1))){} //wait until bit is 1 (polling)
	//*(cdma_address+1) = 1; //status
	//cout << std::hex << *cdma_address <<endl;
	//following instructions from paper
	for(int i = 0; i < 5; i++){
		if(*(destination_address+i) == *(source_address+i)){
			cout <<"True"<<endl;
			cout <<"Source Array value: " << *(source_address+i)<<endl;
			cout <<"Destination Array value: " << *(destination_address+i) <<endl;

		}
			else {
//				cout<< "Source Array value: " <<*(source_address+i)<<endl;
//				cout <<"Destination Array value: "<<*(destination_address+i)<<endl;
//
				cout <<"False so ending application"<<endl;
			}

	}
	//cout << std::hex << *cdma_address <<endl;

	return 0;
}

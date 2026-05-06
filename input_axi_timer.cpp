#include "stdbool.h"

#include "xparameters.h"
#include "xil_types.h"
#include "xgpio.h"
#include "xil_io.h"
#include "xil_exception.h"

#include "xtmrctr.h"

#include <iostream>
using namespace std;

int main()

{


    static XGpio GPIOInstance_Ptr;
    //XTmrCtr TimerInstancePtr;
    u32* Timer_Ptr = (u32*)XPAR_TMRCTR_0_BASEADDR;
    int xStatus;
    //static int rounds= 0;

    cout << "####  Counter (Part 3) Application Starts ####" << endl;


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Step-1: AXI GPIO Initialization
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    xStatus = XGpio_Initialize(&GPIOInstance_Ptr, XPAR_AXI_GPIO_0_DEVICE_ID);
    if(xStatus != XST_SUCCESS)
    {
	    cout << "GPIO A Initialization FAILED" << endl;
	    return 1;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Step-2: AXI GPIO Set the Direction
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //XGpio_SetDataDirection(XGpio *InstancePtr, unsigned Channel, u32 DirectionMask);
    //we use only channel 1, and 0 is the the parameter for output

    XGpio_SetDataDirection(&GPIOInstance_Ptr, 1, 0);

    int period = 0;
    int duty_cycle;
    while(1){
    		cout <<"Please enter the desired period (full range = 50 000 000)"<<endl;
    		cin >> period;
    		*(Timer_Ptr+1) = period;
    		*Timer_Ptr = 0x20;
    		cout <<"Please enter the desired duty cycle (same as above, entire number)"<<endl;
    		cin >> duty_cycle;
    		*(Timer_Ptr+5) = duty_cycle;
    		*(Timer_Ptr+4) = 0x20;
    		//clearing the load instruction
    	    *Timer_Ptr = 0x00;
    	    *(Timer_Ptr+4) = 0x00;
    	    //enabling PWM mode
    	    *Timer_Ptr = 0x296;
    	    *(Timer_Ptr + 4) = 0x296;
    };

    return 0;

}

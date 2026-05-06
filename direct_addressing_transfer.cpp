#include "xil_cache.h"
#include "xparameters.h"
#include "xil_io.h"
#include <iostream>
using namespace std;

// CDMA register offsets
#define CDMACR_OFF    0x00    // Control Register
#define CDMASR_OFF    0x04    // Status Register
#define SA_OFF        0x18    // Source Address Register
#define DA_OFF        0x20    // Destination Address Register
#define BTT_OFF        0x28    // Bytes to Transfer Register

#define XPAR_AXI_TIMER_0_BASEADDR 0x42800000

int main()
{
    cout << "----- Starting Benchmark -----" << endl;

    // Declare and initialize three pointers to u32 for the
    // cdma_base_address, the source_address and the destination_address
    volatile u32* cdma_base = (u32*)XPAR_AXI_CDMA_0_BASEADDR;
    u32* source_addr         = (u32*)XPAR_PS7_DDR_0_S_AXI_HP0_BASEADDR;
    u32* dest_addr             = (u32*)XPAR_PS7_DDR_0_S_AXI_HP2_BASEADDR;

    // Pointer to AXI timer
    volatile u32* timer_ptr = (u32*)XPAR_AXI_TIMER_0_BASEADDR;

    int sizes[] = {1024, 4096, 8192, 16384, 32767, 1048576};

    cout << "Size, CPU_Cycles, CDMA_Cycles" << endl;

    for(int i = 0; i < 6; i++) {
        int n = sizes[i];
        int bytes = n*4;

        //Init arrays
        for(int j = 0; j < n; j++) {source_addr[j]; dest_addr[j] = 0;}

        //CPU Test
        *(timer_ptr + 1) = 0; *(timer_ptr) = 0x20; //Reset and load 0
        *(timer_ptr) = 0x80; //Start Timer
        for(int j = 0; j < n; j++) {dest_addr[j] = source_addr[j];}
        u32 cpu_cycles = *(timer_ptr + 2); //Read TCR0
        *(timer_ptr) = 0;    //Stop

        //CDMA Test
        Xil_DCacheFlush();

        //Reset CDMA
        cdma_base[CDMACR_OFF/4] = 0x04;
        while(cdma_base[CDMACR_OFF/4] & 0x04);

        *(timer_ptr + 1) = 0; *(timer_ptr) = 0x20; *(timer_ptr) = 0x80;    //Reset and start timer

        cdma_base[SA_OFF/4] = (u32)source_addr;
        cdma_base[DA_OFF/4] = (u32)dest_addr;
        cdma_base[BTT_OFF/4] = bytes;        //Start transfer

        while(!(cdma_base[CDMASR_OFF/4] & 0x02));

        u32 cdma_cycles = *(timer_ptr + 2);
        *(timer_ptr) = 0;

        cout << n << "," << cpu_cycles << "," << cdma_cycles << endl;
    }
    cout << "---- Benchmark done ----" << endl;
    return 0;
}

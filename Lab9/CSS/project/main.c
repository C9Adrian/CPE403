#include <stdint.h>
#include <stdbool.h>
#include <math.h>           //Math Functions
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"  //Floating Point
#include "driverlib/sysctl.h"

#define TARGET_IS_BLIZZARD_RB1      //Symbol to access ROM
#include "driverlib/rom.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846     //pi
#endif

#define SERIES_LENGTH 100               //100 calulations
float gSeriesData[SERIES_LENGTH];       //holds calculated data
int32_t i32DataCount = 0;               //count of data to iterate


int main(void)
{
    float fRadians;                 //holds two pi

    ROM_FPULazyStackingEnable();        //turn on lazy stack
    ROM_FPUEnable();                    //turn on fpu

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Clock to 50 MHz

    fRadians = ((2 * M_PI) / SERIES_LENGTH);        //segment of 2pi

    while(i32DataCount < SERIES_LENGTH)
    {
        gSeriesData[i32DataCount] = sinf(fRadians * i32DataCount);      //calculate sine value
        i32DataCount++;             //get the next sine value
    }
    while(1)
    {

    }
}

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

#define SERIES_LENGTH 1200               //1000 calulations
float gSeriesData[SERIES_LENGTH];       //holds calculated data
int32_t i32DataCount = 0;               //count of data to iterate


int main(void)
{
    float fRadians;                 //holds two pi
    float Pi2 = (M_PI / 2);
    float temp, holder;

    FPULazyStackingEnable();        //turn on lazy stack
    FPUEnable();                    //turn on fpu

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Clock to 50 MHz

    fRadians = ((2 * M_PI) / SERIES_LENGTH);        //segment of 2pi

    while(i32DataCount < SERIES_LENGTH)
    {
        temp = sinf(fRadians * i32DataCount * M_PI * 100) ;      //calculate sine value
        holder = 0.5*cosf(fRadians * i32DataCount * 400 * M_PI);
        gSeriesData[i32DataCount] =temp + holder;
        i32DataCount++;             //get the next sine value
    }
    while(1)
    {

    }
}

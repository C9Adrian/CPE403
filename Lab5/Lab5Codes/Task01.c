#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/gpio.h"

#ifdef DEBUG
void_error_(char *pcFilename, uint32_t ui32Line)
{}
#endif

int main(void)
{
    uint32_t ui32ADC0Value[4]; //an array that stores data from the adc

    //variables used to hold tempature info
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    //run clock at 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ);

    //enable the ADC peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //enable port f
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);


    //ADC Sample Averaged
    ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    //Configure the ADC sequencer to sample at 1Msps
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //Configure the ADC sequencer steps to sample temperture
    //Sample the tempature and configure the interrupt flag
    //Since ss3 can only take one sample at a time, i set up the flag at the same time
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

    //Enable the Sequencer
    ADCSequenceEnable(ADC0_BASE,3);


    while(1)
    {
        //Clear the ADC Flag
        ADCIntClear(ADC0_BASE, 3);

        //ADC conversion with software
        ADCProcessorTrigger(ADC0_BASE, 3);

        //wait conversion to be complete
        while(!ADCIntStatus(ADC0_BASE,3,false))
        {

        }

        //Copy ADC samples into the array
        ADCSequenceDataGet(ADC0_BASE,1, ui32ADC0Value);

        //Calculate the average tempature samples
        ui32TempAvg = (ui32ADC0Value[0]+ui32ADC0Value[1]+ui32ADC0Value[2]+ui32ADC0Value[3] + 2)/4;

       //Calculate the Tempature in Celcuis from the samples
        ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096) / 10;

        //Calculate The Temp in Far
        ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;

       //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);


        if(ui32TempValueF > 72)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);

        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        }

    }

}

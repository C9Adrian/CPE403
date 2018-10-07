#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#ifdef DEBUG
void_error_(char *pcFilename, uint32_t ui32Line)
{}
#endif

int main(void)
{

    //run clock at 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ);

    //enable the ADC peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //ADC Sample Averaged
    ADCHardwareOversampleConfigure(ADC0_BASE, 32);

    //Configure the ADC sequencer to sample at 1Msps
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

    //Configure the ADC sequencer steps to sample temperture
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);

    //Sample the tempature and configure the interrupt flag
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);


    //Enable Timer1, set period, and enable interrupt
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A,(SysCtlClockGet()/2)-1);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    IntMasterEnable();

    //Enable the Sequencer
    ADCSequenceEnable(ADC0_BASE,1);

    TimerEnable(TIMER1_BASE, TIMER_A);

    while(1)
    {
    }

}

void Timer1IntHandler(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    uint32_t ui32ADC0Value[4]; //an array that stores data from the adc

    //variables used to hold tempature info
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    //Clear the ADC Flag
          ADCIntClear(ADC0_BASE, 1);

          //ADC conversion with software
          ADCProcessorTrigger(ADC0_BASE, 1);

          //wait conversion to be complete
          while(!ADCIntStatus(ADC0_BASE,1,false))
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


          if(ui32TempValueF > 72)
          {
              GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);

          }
          else
          {
              GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
          }

}

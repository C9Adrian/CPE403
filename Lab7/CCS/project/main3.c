#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "string.h"

void printm(char *str)
{
    //While it's not NULL
    while(*str != '\0')
    {
        UARTCharPut(UART0_BASE,*str);
        ++str;
    }
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //enable GPIO port for LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //enable pin for LED PF2

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //enable the ADC peripheral
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

        //ADC Sample Averaged
        ADCHardwareOversampleConfigure(ADC0_BASE, 64);

        //Configure the ADC sequencer to sample at 1Msps
        ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

        //Configure the ADC sequencer steps to sample temperture
        ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
        ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
        ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);

        //Sample the tempature and configure the interrupt flag
        ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);

        uint32_t ui32ADC0Value[4]; //an array that stores data from the adc


        //variables used to hold tempature info
        volatile uint32_t ui32TempAvg;
        volatile uint32_t ui32TempValueC;
        volatile uint32_t ui32TempValueF;



    printm("\r\n");
    printm("Enter the cmd: R: Red LED, G: Green LED, B: Blue LED, T: Temperature");
    printm("\r\n");

    char holder;
    char str[10];

    ADCSequenceEnable(ADC0_BASE,1);

    while (1)
    {
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

        //This is what the user entered
        if (UARTCharsAvail(UART0_BASE))
        {

            holder = UARTCharGet(UART0_BASE);
            UARTCharPut(UART0_BASE,holder);
            printm("\r\n");


            if(holder == 'B'){
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, GPIO_PIN_2);
                printm("Turning on Blue LED");
                printm("\r\n");
            }

            if(holder == 'b')
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, 0);
                printm("Turning off Blue LED");
                printm("\r\n");
            }

            if(holder == 'R'){
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
                printm("Turning on Red LED");
                printm("\r\n");
            }

            if(holder == 'r')
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
                printm("Turning off Red LED");
                printm("\r\n");
            }

            if(holder == 'G'){
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, GPIO_PIN_3);
                printm("Turning on Green LED");
                printm("\r\n");
            }

            if(holder == 'g')
            {
                GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3, 0);
                printm("Turning off Green LED");
                printm("\r\n");
            }

            if(holder == 'T')
            {
                itoa(ui32TempValueF, str);
                printm("Tempature in Far: ");
                printm(str);
                printm("\r\n");
            }

        }
    }

}


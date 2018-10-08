#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

#define PWM_FREQUENCY 55


int main(void)
{
    volatile uint32_t ui32Load;
    volatile uint32_t ui32PWMClock;
    volatile uint8_t ui8Adjust;             //adjust the position of the servo
    ui8Adjust = 83;                         //center position of servo

    //Run CPU at 40MHz
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    //Make the divder range 2 to 64
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    //Enable PWM1 and GPIOD modules for PWM and GPIOF modules
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    //Enable PORTF modules
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Configure PD1 as a PWM output(M1PW5)
    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Configure the mux that selects each LED
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);


    //Set up the PWM clock and load Values into the PWM registers
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);

    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

    //Set Pulse Width and enable PWM
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);


    while(1)
    {
        int r = 100;
        int g = 100;
        int b = 100;

        for(r; r < 900; r++)
        {
            ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, r * ui32Load /1000);
            ROM_SysCtlDelay(10000);
            for(b; b < 900; b++)
            {
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, b * ui32Load /1000);
                ROM_SysCtlDelay(10000);
                for(g; g < 900; g++)
                {
                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, g * ui32Load /1000);
                    ROM_SysCtlDelay(10000);
                }
            }
        }

        for(g; g >= 100; g--)
        {
            ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, g * ui32Load /1000);
            ROM_SysCtlDelay(10000);
        }
        for(b; b >= 100; b--)
        {
            ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, b * ui32Load /1000);
            ROM_SysCtlDelay(10000);
        }
        for(r; r >= 100; r--)
        {
            ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, r * ui32Load /1000);
            ROM_SysCtlDelay(10000);
        }

    }

}

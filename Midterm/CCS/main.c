
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/interrupt.h"
#include "driverlib/hibernate.h"
#include "Adafruit_TSL2591.h"
#include "utils/ustdlib.h"

void ConfigureUART(void)
//Configures the UART to run at 19200 baud rate
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);    //enables UART module 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    //enables GPIO port b

    GPIOPinConfigure(GPIO_PB1_U1TX);    //configures PB1 as TX pin
    GPIOPinConfigure(GPIO_PB0_U1RX);    //configures PB0 as RX pin
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  //sets the UART pin type

    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);   //sets the clock source
    //enables UARTstdio baud rate, clock, and which UART to use
    UARTStdioConfig(1, 115200, 16000000);
}


void I2C0_Init ()
//Configure/initialize the I2C0
{
    SysCtlPeripheralEnable (SYSCTL_PERIPH_I2C0);    //enables I2C0
    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);   //enable PORTB as peripheral
    GPIOPinTypeI2C (GPIO_PORTB_BASE, GPIO_PIN_3);   //set I2C PB3 as SDA
    GPIOPinConfigure (GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL (GPIO_PORTB_BASE, GPIO_PIN_2);    //set I2C PB2 as SCLK
    GPIOPinConfigure (GPIO_PB2_I2C0SCL);
    //Set the clock of the I2C to ensure proper connection
    I2CMasterInitExpClk (I2C0_BASE, SysCtlClockGet(), false);
    while (I2CMasterBusy (I2C0_BASE));  //wait while the master SDA is busy
}

void I2C0_Write (uint8_t addr, uint8_t N, ...)
//Writes data from master to slave
//Takes the address of the device, the number of arguments, and a variable amount of register addresses to write to
{
//Find the device based on the address given
    I2CMasterSlaveAddrSet (I2C0_BASE, addr, false);
    while (I2CMasterBusy (I2C0_BASE));

    va_list vargs;  //variable list to hold the register addresses passed

    va_start (vargs, N); //initialize the variable list with the number of arguments
//put the first argument in the list in to the I2C bus
    I2CMasterDataPut (I2C0_BASE, va_arg(vargs, uint8_t));
    while (I2CMasterBusy (I2C0_BASE));
    if (N == 1) //if only 1 argument is passed, send that register command then stop
    {
        I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy (I2C0_BASE));
        va_end (vargs);
    }
    else
    //if more than 1, loop through all the commands until they are all sent
    {
        I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while (I2CMasterBusy (I2C0_BASE));
        uint8_t i;
        for (i = 1; i < N - 1; i++)
        {
        //send the next register address to the bus
            I2CMasterDataPut (I2C0_BASE, va_arg(vargs, uint8_t));
            while (I2CMasterBusy (I2C0_BASE));
        //burst send, keeps receiving until the stop signal is received
            I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
            while (I2CMasterBusy (I2C0_BASE));
        }
        //puts the last argument on the SDA bus
        I2CMasterDataPut (I2C0_BASE, va_arg(vargs, uint8_t));
        while (I2CMasterBusy (I2C0_BASE));
        //send the finish signal to stop transmission
        I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while (I2CMasterBusy (I2C0_BASE));

        va_end (vargs);
    }

}

uint32_t I2C0_Read (uint8_t addr, uint8_t reg)
//Read data from slave to master
//Takes in the address of the device and the register to read from
{
//find the device based on the address given
    I2CMasterSlaveAddrSet (I2C0_BASE, addr, false);
    while (I2CMasterBusy (I2C0_BASE));
//send the register to be read on to the I2C bus
    I2CMasterDataPut (I2C0_BASE, reg);
    while (I2CMasterBusy (I2C0_BASE));
//send the send signal to send the register value
    I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy (I2C0_BASE));
//set the master to read from the device
    I2CMasterSlaveAddrSet (I2C0_BASE, addr, true);
    while (I2CMasterBusy (I2C0_BASE));
//send the receive signal to the device
    I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while (I2CMasterBusy (I2C0_BASE));
//return the data read from the bus
    return I2CMasterDataGet (I2C0_BASE);
}

void TSL2591_init ()
//Initializes the TSL2591 to have a medium gain,
{
    uint32_t x;
    x = I2C0_Read (TSL2591_ADDR, (TSL2591_COMMAND_BIT | TSL2591_REGISTER_DEVICE_ID));//read the device ID
    if (x == 0x50)
    {
    }
    else
    {
      while (1){};      //loop here if the dev ID is not correct
    }
//configures the TSL2591 to have medium gain adn integration time of 100ms
    //I2C0_Write (TSL2591_ADDR, 2, (TSL2591_COMMAND_BIT | TSL2591_CONFIG), 0x10);
    I2C0_Write (TSL2591_ADDR, 2, (TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL), 0x10);
    //enables proper interrupts and power to work with TSL2591

    //I2C0_Write (TSL2591_ADDR, 2, (TSL2591_COMMAND_BIT | TSL2591_ENABLE),
    I2C0_Write (TSL2591_ADDR, 2, (TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE),
    (TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN |
                                TSL2591_ENABLE_NPIEN));
}

uint32_t GetLuminosity ()
//This function will read the channels of the TSL and returns the calculated value to the caller
{
    float atime = 100.0f, again = 25.0f;    //the variables to be used to calculate proper lux value
    uint16_t ch0, ch1;  //variable to hold the channels of the TSL2591
    uint32_t cp1, lux1, lux2, lux;
    uint32_t x = 1;

    //x = I2C0_Read (TSL2591_ADDR, (TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_HIGH));
    x = I2C0_Read (TSL2591_ADDR, (TSL2591_COMMAND_BIT | TSL2591_C0DATAH));
    x <<= 16;
    //x |= I2C0_Read (TSL2591_ADDR, (TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_lOW));
    x |= I2C0_Read (TSL2591_ADDR, (TSL2591_COMMAND_BIT | TSL2591_C0DATAL));

    ch1 = x>>16;
    ch0 = x & 0xFFFF;

    cp1 =  (uint32_t) (atime * again) / TSL2591_LUX_DF;
    lux1 = (uint32_t) ((float) ch0 - (TSL2591_LUX_COEFB * (float) ch1)) / cp1;
    lux2 = (uint32_t) ((TSL2591_LUX_COEFC * (float) ch0) - (TSL2591_LUX_COEFD * (float) ch1)) / cp1;
    lux = (lux1 > lux2) ? lux1: lux2;

    return lux;
}

void main (void)
{
    //set the main clock to run at 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    uint32_t lux = 0, i;
    uint32_t luxAvg = 0;

    ConfigureUART ();   //configure the UART of Tiva C
    I2C0_Init ();       //initialize the I2C0 of Tiva C
    TSL2591_init ();    //initialize the TSL2591

    //enable button 2 to be used during hibernation
    SysCtlPeripheralEnable (SYSCTL_PERIPH_HIBERNATE);
    //Get the system clock to set to the hibernation clock
    HibernateEnableExpClk (SysCtlClockGet());
    //Retain the pin function during hibernation
    HibernateGPIORetentionEnable ();
    //Set RTC hibernation
    HibernateRTCSet (0);
    //enable RTC hibernation
    HibernateRTCEnable ();
    //hibernate for 30 minutes
    //HibernateRTCMatchSet (0, 1800);
    HibernateRTCMatchSet (0, 30);
    //allow hibernation wake up from RTC time or button 2
    HibernateWakeSet (HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);

    for (i = 0; i < 20; i++)
    //finds the average of the lux channel to send through uart
    {
        lux = GetLuminosity ();
        luxAvg += lux;
    }
    luxAvg = luxAvg/20;
    //UARTprintf("Lux Value: %d \r\n",luxAvg);
    UARTprintf("%d\r",luxAvg);
    SysCtlDelay(100000);

    HibernateRequest ();    //Hibernate
    while (1)
    {};
}

/*Switch is low when pressed, and LED is on when HIGH*/


#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#include "driverlib/pin_map.h"
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

void delayMs(int n);
void servo_rotate_C(int angle);
void PWM1();

int main(void)
{
    unsigned int value;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //servo_Init();
    PWM1();
    int pos;
    while(1)
    {
                for (int pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    servo_rotate_C(pos);            // tell servo to go to position in variable 'pos'
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }
                  for (int pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
                      servo_rotate_C(pos);             // tell servo to go to position in variable 'pos'
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }

    }
}

void servo_rotate_C(int angle)
{
    int width;

         delayMs(1000);
         float val = angle*26.38;
         width = 1250 + val;
         PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,width);   //PC4
         delayMs(1000);
         //width= width_cal1(170);
         //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,width);   //PC4


}


void PWM1()
{
    /***********************************************************************************************************************************************************
       * * PWM C SETUP
      * *********************************************************************************************************************************************************/

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

      //Configure PWM Clock to match system clock/16 (i.e. PWM CLOCK  (40MHZ / 16 = 2.5MHZ))
         SysCtlPWMClockSet(SYSCTL_PWMDIV_16);

     // Enable port C used for PWM
         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

     //The Tiva Launchpad has two modules (0 and 1). Module 0 covers the PC4
         SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

     //  M0PWM6  PC4 Module 0 Generator 3 out 6 => M0PWM6
         GPIOPinConfigure(GPIO_PC4_M0PWM6);
         GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);


     //Configure PWM Options
         PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);


     //Set the Period (expressed in clock ticks)
         PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 50000);


     //Set PWM duty-1.45ms for 90 degree rotation
     PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,3625);   //PC4

     // Enable the PWM generator
     PWMGenEnable(PWM0_BASE, PWM_GEN_3); //PC4

     // Turn on the Output pins for  PC4
     PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);



/***********************************************************************************************************************************************************
      * PWM C Setup complete
      * *********************************************************************************************************************************************************/



}

void delayMs(int n)
{
    int i, j;

    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  /* do nothing for 1 ms */
}


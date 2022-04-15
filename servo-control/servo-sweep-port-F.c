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
void servo_rotate_F(int angle);

void PWM3();

int main(void)
{
    unsigned int value;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //servo_Init();
    PWM3();
    int pos;
    while(1)
    {

        for (int pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    servo_rotate_F(pos);            // tell servo to go to position in variable 'pos'
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }
                  for (int pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
                      servo_rotate_F(pos);             // tell servo to go to position in variable 'pos'
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }

    }


}

void servo_rotate_F(int angle)
{
    int width;

           //while(1)
            //{
                 delayMs(1000);
                 float val = angle*26.38;
                 width = 1250 + val;
                 //printf("%d", width);
                 //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,3625);   //PE4
                 PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,width);   //PE4
                 //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,width);   //PE4
                 delayMs(1000);
                 //width= width_cal1(170);
                 //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,width);   //PE4
            //}



}



void PWM3()
{

    /***********************************************************************************************************************************************************
     * * PWM F SETUP
    * *********************************************************************************************************************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);



    //Configure PWM Clock to match system clock/16 (i.e. PWM CLOCK  (40MHZ / 16 = 2.5MHZ))
       SysCtlPWMClockSet(SYSCTL_PWMDIV_16);

   // Enable port F used for PWM
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

   //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins
       SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

   //  M1PWM5, M1PWM6, M1PWM7 for PF1, PF2, PF3 respectively
       GPIOPinConfigure(GPIO_PF1_M1PWM5);
       GPIOPinConfigure(GPIO_PF2_M1PWM6);
       GPIOPinConfigure(GPIO_PF3_M1PWM7);
       GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

       PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
       PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

   //Set the Period (expressed in clock ticks)
       PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 50000);
       PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 50000);


   //Set PWM duty-1.45ms for 90 degree rotation
   //    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,3625);
     //  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,3625);
       //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,3625);

       // Enable the PWM generator
         PWMGenEnable(PWM1_BASE, PWM_GEN_2);
         PWMGenEnable(PWM1_BASE, PWM_GEN_3);

         // Turn on the Output pins
         PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

}

void delayMs(int n)
{
    int i, j;

    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  /* do nothing for 1 ms */
}


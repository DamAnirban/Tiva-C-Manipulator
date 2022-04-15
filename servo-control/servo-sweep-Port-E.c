//Port E servo sweep


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
int width_cal1(float angle);
void PWM_PORTE();

int main(void)
{
    unsigned int value;
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //servo_Init();
    PWM_PORTE();
    //int pos;
    while(1)
    {
        //PWM_PORTE();

        for (int pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            servo_rotate_E(pos);            // tell servo to go to position in variable 'pos'
            delayMs(15);                       // waits 15ms for the servo to reach the position
          }
          for (int pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
              servo_rotate_E(pos);             // tell servo to go to position in variable 'pos'
            delayMs(15);                       // waits 15ms for the servo to reach the position
          }

        //int pos = 45;
        //servo_rotate_E(pos);
        //delayMs(2000);
        //servo_rotate_E(180);

    }


}

void servo_rotate_E(int angle)
{
    int width;
    //while(1)
    //{
         delayMs(1000);
         float val = angle*26.38;
         width = 1250 + val;
         printf("%d", width);
         PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,width);   //PE4
         delayMs(1000);
         //width= width_cal1(170);
         //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,width);   //PE4
    //}

}

int width_cal1(float angle)
{
    int new_width ;

    /*
    * PWM Module Clock Period = 1 / 2.5MHz = 0.4µs
    * PWM Signal Period = x ms
    * Load register value = x ms/0.4µs = 2500 x
    * for x=0.5ms      load=1250  degree_of_rotation = 0
    * for x=1.45ms     load=3625  degree_of_rotation = 90
    * for x=2.4ms      load=6000  degree_of_rotation = 180
    * which can be stored by our 16 bit registers
    */


    new_width = 1250 + ((angle)/180) * 4750;
    /* width = 1250 [0.5   milliseconds or 500us] for 0   degree
     * width = 3625 [1.5 milliseconds or 1500us] for 90  degree
     * width = 6000 [2.4   milliseconds or 2400us] for 180 degree
     */

    return new_width;
   // return 1250;
   // return 3625;
   //  return 6000;
}

void PWM_PORTE()
{

    /***********************************************************************************************************************************************************
     * * PWM E SETUP M0PWM4 PE4 Module 0 Generator 2
    * *********************************************************************************************************************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


      //Configure PWM Clock to match system clock/16 (i.e. PWM CLOCK  (40MHZ / 16 = 2.5MHZ))
         SysCtlPWMClockSet(SYSCTL_PWMDIV_16);

   // Enable port E used for PWM //M0PWM4   PE4 Module 0 Generator 2
         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

   //The Tiva Launchpad has two modules (0 and 1). Module 0 covers PE4 Module 0 Generator 2
         SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

   // M0PWM4    PE4 Module 0 Generator 2
         GPIOPinConfigure(GPIO_PE4_M0PWM4);
         GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);


   //Configure PWM Options
         PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

   /*
    * PWM Module Clock Period = 1 / 2.5MHz = 0.4µs
    * PWM Signal Period = 1 / 50Hz = 20ms
    * Load register value = 20ms/0.4µs = 50,000 (within the range of 65,536)
    * which can be stored by our 16 bit registers
    */
   //Set the Period (expressed in clock ticks)
         PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 50000);


  /*
  * PWM Module Clock Period = 1 / 2.5MHz = 0.4µs
  * PWM Signal Period = x ms
  * Load register value = x ms/0.4µs = 2500 x
  * for x=0.5ms      load=1250  degree_of_rotation = 0
  * for x=1.45ms    load=3625  degree_of_rotation = 90
  * for x=2.4ms      load=6000  degree_of_rotation = 180
  * which can be stored by our 16 bit registers
  */
   //Set PWM duty-1.45ms for 90 degree rotation
        // PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,3625);   //M0PWM4  PE4 Module 0 Generator 2

   // Enable the PWM generator
         PWMGenEnable(PWM0_BASE, PWM_GEN_2); //M0PWM4 PE4 Module 0 Generator 2

   // M0PWM4    PE4 Module 0 Generator 2
         PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);


   /***********************************************************************************************************************************************************
    * PWM E Setup complete
    * *********************************************************************************************************************************************************/

}

void delayMs(int n)
{
    int i, j;

    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  /* do nothing for 1 ms */
}


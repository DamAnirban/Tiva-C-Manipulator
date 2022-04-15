#ifndef SERVO_INIT_C_
#define SERVO_INIT_C_

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

void PWM_PORTF();
void PWM_PORTE();
void PWM_PORTC();

void servo_rotate_C(int angle);
void servo_rotate_E(int angle);
void servo_rotate_F(int angle);

void delayMs(int n);

void servo_rotate_C(int angle)
{
    int width;
    float val = angle*26.38;
    width = 1250 + val;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,width);   //PC4
    delayMs(100);
}

void servo_rotate_E(int angle)
{
    int width;
    float val = angle*26.38;
    width = 1250 + val;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,width);   //PE4
    delayMs(100);
}

void servo_rotate_F(int angle)
{
    int width;
    float val = angle*26.38;
    width = 1250 + val;
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,width);   //PF2
    delayMs(100);
}


void PWM_PORTC()
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
     //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,3625);   //PC4

     // Enable the PWM generator
     PWMGenEnable(PWM0_BASE, PWM_GEN_3); //PC4

     // Turn on the Output pins for  PC4
     PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);



        /***********************************************************************************************************************************************************
      * PWM C Setup complete
      * *********************************************************************************************************************************************************/



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

void PWM_PORTF()
{

    /***********************************************************************************************************************************************************
     * * PWM F SETUP
    * *********************************************************************************************************************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //Configure PWM Clock to match system clock/16 (i.e. PWM CLOCK  (40MHZ / 16 = 2.5MHZ))
       SysCtlPWMClockSet(SYSCTL_PWMDIV_16);

   // Enable port F used for PWM
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

   // The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins
       SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

   // M1PWM5, M1PWM6, M1PWM7 for PF1, PF2, PF3 respectively
       GPIOPinConfigure(GPIO_PF1_M1PWM5);
       GPIOPinConfigure(GPIO_PF2_M1PWM6);
       GPIOPinConfigure(GPIO_PF3_M1PWM7);
       GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

       PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
       PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

   // Set the Period (expressed in clock ticks)
       PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 50000);
       PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 50000);


   // Set PWM duty-1.45ms for 90 degree rotation
   // PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,3625);
   // PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,3625);
   // PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,3625);

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



#endif /* SERVO_C_ */

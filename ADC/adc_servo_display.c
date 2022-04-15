
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


#include <string.h>
#include <stdlib.h>
#include <driverlib/uart.h>

void delayMs(int n);
void servo_rotate_C(int angle);
void PWM1();
int count[]= {0x3F, 0X06, 0X5B, 0x4F, 0X66, 0X6D, 0x7D, 0x07, 0x7F, 0x6F};

void adc_init(void)
{
    /* enable clocks */
    SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to PE (AIN0 is on PE3) */
    SYSCTL_RCGCADC_R |= 1;     /* enable clock to ADC0 */

    /* initialize PE3 for AIN0 input  */
    GPIO_PORTE_AFSEL_R |= 8;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~8;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 8;   /* enable analog function */

    /* initialize ADC0 */
    ADC0_ACTSS_R &= ~8;        /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;    /* software trigger conversion */
    ADC0_SSMUX3_R = 0;         /* get input from channel 0 */
    ADC0_SSCTL3_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 8;         /* enable ADC0 sequencer 3 */

    SYSCTL_RCGC2_R |= 0x00000003;     /* enable clock to GPIOA, B at clock gating control register */

        GPIO_PORTA_DIR_R = 0xF0;
        GPIO_PORTA_DEN_R = 0xF0;          /* enable the GPIO pins for digital function */

        GPIO_PORTB_DIR_R = 0xFF;          /* enable the GPIO pins for the LED (PF3) as output and PF0, PF4 as Input */
        GPIO_PORTB_DEN_R = 0xFF;          /* enable the GPIO pins for digital function */
}

int main(void)
{
    unsigned int value;
    volatile int result;

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);


    adc_init();
    PWM_PORTC();
    int pos;
    while(1)
    {
        ADC0_PSSI_R |= 8;      /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0)
              ;                  /* wait for conversion complete */
        result = ADC0_SSFIFO3_R; /* read conversion result */

        float new_result = result * 0.04394;
        pos = new_result;

        servo_rotate_C(pos);

        int a = result%10;
        int b = (result/10)%10;
        int c = (result/100)%10;
        int d = (result/1000)%10;

        for(int i = 0; i< 20; i++)
        {
                GPIO_PORTA_DATA_R = 0x10;
                GPIO_PORTB_DATA_R = count[a];
                delayMs(1);

                GPIO_PORTA_DATA_R = 0x20;
                GPIO_PORTB_DATA_R = count[b];
                delayMs(1);

                GPIO_PORTA_DATA_R = 0x40;
                GPIO_PORTB_DATA_R = count[c];
                delayMs(1);

                GPIO_PORTA_DATA_R = 0x80;
                GPIO_PORTB_DATA_R = count[d];
                delayMs(1);
        }
        ADC0_ISC_R = 8;        /* clear completion flag */
    }


}

void servo_rotate_C(int angle)
{
    int width;


         float val = angle*26.38;
         width = 1250 + val;
         PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,width);   //PC4
         delayMs(100);
         //width= width_cal1(170);
         //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,width);   //PC4


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
     //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,1250);   //PC4

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


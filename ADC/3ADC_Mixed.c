/* A to D conversion
 *
 * This program converts the analog input from AIN0 (J3.9 of LaunchPad)
 * using sample sequencer 3 and software trigger continuously.
 *
 * Note: AIN0 channel is on PE3 pin.
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include <stdbool.h>
#include <./inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

void delayMs(int n);
int count[]= {0x3F, 0X06, 0X5B, 0x4F, 0X66, 0X6D, 0x7D, 0x07, 0x7F, 0x6F};
char val;



int main(void)
{
    volatile int result, result1, result2;
    char result_str[50];

    /* enable clocks */
    SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to PE (AIN0 is on PE3) */
    SYSCTL_RCGCADC_R |= 1;     /* enable clock to ADC0 */

    //PE1
    GPIO_PORTE_AFSEL_R |= 2;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~2;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 2;   /* enable analog function */

    /* initialize PE2 for AIN0 input  */
    GPIO_PORTE_AFSEL_R |= 4;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~4;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 4;   /* enable analog function */

    //PE3
    GPIO_PORTE_AFSEL_R |= 8;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~8;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 8;   /* enable analog function */

    /* initialize ADC0 */
    ADC0_ACTSS_R &= ~8;        /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~0xF000;    /* software trigger conversion */
    //common end


    //PE3
    ADC0_SSMUX3_R = 1;         /* get input from channel 1 */
    ADC0_SSCTL3_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 8;         /* enable ADC0 sequencer 3 */

    //PE2
    ADC0_SSMUX2_R = 0;         /* get input from channel 0 */
    ADC0_SSCTL2_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 4;         /* enable ADC0 sequencer 3 */

    //PE1
    ADC0_SSMUX1_R = 2;         /* get input from channel 2 */
    ADC0_SSCTL1_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= 2;         /* enable ADC0 sequencer 3 */






    //7 segment
    SYSCTL_RCGC2_R |= 0x00000003;     /* enable clock to GPIOA, B at clock gating control register */

    GPIO_PORTA_DIR_R = 0xF0;
    GPIO_PORTA_DEN_R = 0xF0;          /* enable the GPIO pins for digital function */

    GPIO_PORTB_DIR_R = 0xFF;          /* enable the GPIO pins for the LED (PF3) as output and PF0, PF4 as Input */
    GPIO_PORTB_DEN_R = 0xFF;          /* enable the GPIO pins for digital function */


    while(1) {
        /////////////
        ADC0_PSSI_R |= 8;      /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0)
            ;                  /* wait for conversion complete */
        result = ADC0_SSFIFO3_R; /* read conversion result */





        ///////////
        ADC0_PSSI_R |= 4;      /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0)
                    ;                  /* wait for conversion complete */
        result1 = ADC0_SSFIFO2_R;




        ///////////
        ADC0_PSSI_R |= 2;      /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0)
                    ;                  /* wait for conversion complete */
        result2 = ADC0_SSFIFO1_R;



        //GPIO_PORTA_DATA_R = 0x10;
        //GPIO_PORTB_DATA_R = result;
        int a = (result/100)%10;
        int b = (result/1000)%10;

        int c = (result1/1000)%10;

        int d = (result2/1000)%10;

        for(int i = 0; i< 50; i++)
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
        ADC0_ISC_R = 4;
        ADC0_ISC_R = 2;
    }
}
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++) {}   /* do nothing for 1 ms */
}

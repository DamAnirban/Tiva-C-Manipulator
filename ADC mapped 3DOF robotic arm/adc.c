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


void adc_init(void);

void adc_init()
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

    //PE2
    GPIO_PORTE_AFSEL_R |= 4;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~4;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 4;   /* enable analog function */

    //PE0
    GPIO_PORTE_AFSEL_R |= 1;   /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~1;    /* disable digital function */
    GPIO_PORTE_AMSEL_R |= 1;   /* enable analog function */

    /* initialize ADC0 */
    ADC0_ACTSS_R &= ~14;        /* disable SS3, SS2, SS1 during configuration */
    ADC0_EMUX_R &= ~0xF000;    /* software trigger conversion */
    //common end


    //PE0
    ADC0_SSMUX3_R = 3;         /* get input from channel 3 */
    ADC0_SSCTL3_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    //ADC0_ACTSS_R |= 4;         /* enable ADC0 sequencer 2 */

    //PE2
    ADC0_SSMUX2_R = 1;         /* get input from channel 1 */
    ADC0_SSCTL2_R |= 6;        /* take one sample at a time, set flag at 1st sample */
    //ADC0_ACTSS_R |= 4;         /* enable ADC0 sequencer 2 */

    //PE1
    ADC0_SSMUX1_R = 2;         /* get input from channel 2 */
    ADC0_SSCTL1_R |= 6;        /* take one sample at a time, set flag at 1st sample */

    ADC0_ACTSS_R |= 14;         /* enable ADC0 sequencer 3,2,1 */


    //7 segment
    SYSCTL_RCGC2_R |= 0x00000003;     /* enable clock to GPIOA, B at clock gating control register */

    GPIO_PORTA_DIR_R = 0xF0;
    GPIO_PORTA_DEN_R = 0xF0;          /* enable the GPIO pins for digital function */

    GPIO_PORTB_DIR_R = 0xFF;          /* enable the GPIO pins for the LED (PF3) as output and PF0, PF4 as Input */
    GPIO_PORTB_DEN_R = 0xFF;          /* enable the GPIO pins for digital function */

}

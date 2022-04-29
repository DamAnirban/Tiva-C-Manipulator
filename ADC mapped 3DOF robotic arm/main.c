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
#include "servo_init.h"
#include "adc.h"


int count[]= {0x3F, 0X06, 0X5B, 0x4F, 0X66, 0X6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(void)
{
    volatile int result0, result1, result2;
    char result_str[50];
    unsigned int value;
    PWM_PORTF();
    PWM_PORTE();
    PWM_PORTC();
    adc_init();
    int pos;
    while(1)
    {

        /////////////
        ADC0_PSSI_R |= 8;      /* start a conversion sequence 3 */
        while((ADC0_RIS_R & 8) == 0)
                    ;                  /* wait for conversion complete */
        result0 = ADC0_SSFIFO3_R; /* read conversion result */

        ADC0_PSSI_R |= 4;      /* start a conversion sequence 2 */
        while((ADC0_RIS_R & 4) == 0)
                            ;                  /* wait for conversion complete */
        result1 = ADC0_SSFIFO2_R;

        ADC0_PSSI_R |= 2;      /* start a conversion sequence 1 */
        while((ADC0_RIS_R & 2) == 0)
                            ;                  /* wait for conversion complete */
        result2 = ADC0_SSFIFO1_R;




        float new_result0 = result0 * 0.04394;
        int fin_result0 = new_result0;
        if(fin_result0 > 170) fin_result0 = 170;
        if(fin_result0 < 20) fin_result0 = 20;

        float new_result1 = result1 * 0.04394;
        int fin_result1 = new_result1;
        if(fin_result1 > 110) fin_result1 = 110;
        if(fin_result1 < 10) fin_result1 = 10;

         float new_result2 = result2 * 0.04394;
         int fin_result2 = new_result2;
         if(fin_result2 > 170) fin_result2 = 170;
         if(fin_result2 < 90) fin_result2 = 90;

         servo_rotate_F(fin_result0);
         servo_rotate_C(fin_result1);
         servo_rotate_E(fin_result2);


         int b = (result0/1000)%10;
         int c = (result1/1000)%10;
         int d = (result2/1000)%10;

         for(int i = 0; i< 50; i++)
         {

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

         ADC0_ISC_R = 14;

    }

}








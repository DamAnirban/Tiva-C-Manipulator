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


int main(void)
{
    unsigned int value;
    PWM_PORTF();
    PWM_PORTE();
    PWM_PORTC();
    int pos;
    while(1)
    {

        //C
        for (int pos = 10; pos <= 110; pos += 10) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    servo_rotate_C(pos);
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }

        //E
        for (int pos = 90; pos <= 170; pos += 10) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    servo_rotate_E(pos);            // tell servo to go to position in variable 'pos'
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }
        //F
        for (int pos = 20; pos <= 170; pos += 10) { // goes from 0 degrees to 180 degrees
                    // in steps of 1 degree
                    servo_rotate_F(pos);            // tell servo to go to position in variable 'pos'
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }

        //C reverse
        for (int pos = 110; pos >= 10; pos -= 10) { // goes from 180 degrees to 0 degrees
                    servo_rotate_C(pos);
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }
        //E reverse
        for (int pos = 170; pos >= 90; pos -= 10) { // goes from 180 degrees to 0 degrees
                    servo_rotate_E(pos);
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }
        // F reverse
        for (int pos = 170; pos >= 20; pos -= 10) { // goes from 180 degrees to 0 degrees
                    servo_rotate_F(pos);
                    delayMs(15);                       // waits 15ms for the servo to reach the position
                  }

    }

}






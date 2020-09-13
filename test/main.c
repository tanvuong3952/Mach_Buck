#include "msp430.h"
#include <stdio.h>
#include <stdint.h>
#include "lib/Buck_boost.h"

volatile uint16_t duty=0;

int main(void)
{
	Config_stop_WDT();
	Config_Clocks();
	GPIO_init();
	PWM_start();
	while(1)
	{

	}

}

#pragma vector = PORT1_VECTOR
__interrupt void BUTTON_Interrupt_Handle(void)
{
	P1OUT ^= LED_GREEN;
	//delay_us (20);
    if ((P1IFG & BTN1) == BTN1)
    {
    	P1OUT ^= LED_RED;
    	duty += 100;
    	if(duty>1000)
    		duty=0;
        PWM_update(duty);
    }
    P1IFG &= ~BTN1;
}


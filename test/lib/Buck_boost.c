/******************************************************************************
 *
 * www.payitforward.edu.vn
 *
 ******************************************************************************/

/******************************************************************************
 *
 * TI LAUNCH PAD CODING
 *
 ******************************************************************************/

/******************************************************************************
 *
 *    Module       : Buck_boost.c
 *    Description  :
 *
 *  Tool           : CCS 6.1
 *  Chip           : MSP430G2xx3
 *  History        : 5/4/2018
 *  Version        : 1.0
 *
 *  Author         : Nguyen Khoi Nguyen, Pay It Forward Club
 *  Email		   : knguyentnh98@gmail.com
 *  Notes          :
 *      To apply these functions, you must include the header file uart.h and
 *      add this code file to your project.
 *
 *		To find or edit the UART configuration, check the function uart_init()
 *
******************************************************************************/

 /****************************************************************************
 * IMPORT
******************************************************************************/

#include <msp430.h>
#include "Buck_boost.h"


int i=0;
volatile uint8_t counter=0;

 /****************************************************************************
* FUNCTIONS
******************************************************************************/

void Buck_boost_init(void)
{
	Config_stop_WDT();
	Config_Clocks();

	//ADC_init();
}

void GPIO_init(void)
{
    P1SEL  &= ~(BTN1 + LED_GREEN + LED_RED);
    P1SEL2 &= ~(BTN1 + LED_GREEN + LED_RED);
    //Init button
    P1DIR  &= ~BTN1;
    P1REN  |= BTN1;
    P1OUT  |= BTN1;
    P1IE   |= BTN1;
    P1IES  |= BTN1;
    //Init LED GREEN
    P1DIR |= LED_GREEN;
    P1OUT &= ~LED_GREEN;
    //Init LED RED
    P1DIR |= LED_RED;
    P1OUT &= ~LED_RED;
    //Init PWM output
    P2DIR |= PIN_OUT;
    P2OUT &= ~PIN_OUT;
    P2SEL|= PIN_OUT;
    //Global interupt
    _BIS_SR(GIE);

}

void Timer_start(void)
{
    TA0CCR0 = SAMPLING_TIME;
    TA0CTL = TASSEL_2 + MC_1 + TAIE;           // SMCLK, Up mode, Overflow interrupt
    TA0CCTL0 &= ~CCIFG;
}

void Timer_stop(void)
{
	TA0CTL = 0 ;
	TA0R = 0;
	P1OUT  &= ~LED_GREEN;
}

void PID_process(volatile pidparams *pid)
{
	pid->P_part = pid->Kp*(pid->Error - pid->pre_Error);
	pid->I_part = 0.5*pid->Ki*pid->T*(pid->Error + pid->pre_Error);
	pid->D_part = pid->Kd/pid->T*( pid->Error - 2*pid->pre_Error+ pid->pre_pre_Error);
	pid->Out = pid->pre_Out + pid->P_part + pid->I_part + pid->D_part ;
	pid->pre_pre_Error = pid->pre_Error;
	pid->pre_Error = pid->Error;
	pid->pre_Out = pid->Out;
}

void PWM_start(void)
{
	TA1CCR0 = TIME_INTERVAL;
	TA1CCR1 = DUTY_CIRCLE;
	TA1CTL = TASSEL_2 + MC_1 + ID_0 ;                 // SMCLK, Up mode
	TA1CCTL1 = OUTMOD_7; //+ CCIE;      			  // falling edge & raising edge, capture mode, capture/compare
}

void PWM_update(uint16_t duty)
{
	TA1CCR1 = duty;
}

void PWM_stop(void)
{
	TA1CCR0 = 0;
	TA1CCR1 = 0;
	TA1CTL = 0 ;
	TA1CCTL1 = 0;
	P2OUT &= ~PIN_OUT;
}

uint16_t Average_filter(volatile uint16_t *array)
{
	int i;
	float sum;
	for(i=0;i<SAMPLE_NUM;i++)
	{
		sum = array[i];
	}
	sum /= SAMPLE_NUM;
	return (uint16_t)sum;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
    switch( TA0IV )
    {
    case  2:                            // CCR1 not used
        break;
    case  4:                            // CCR2 not used
        break;
    case 10:                            // overflow

//    	if(counter >9)
//    		counter=0;
//    	//read voltage from P1.7
//    	voltage_buf[counter] = ADC10_Read_Channel(4);
//		voltage_pid.Error  = Average_filter(voltage_buf);
//    	//read current from P1.5
//    	current_buf[counter] = ADC10_Read_Channel(5);
//		current_pid.Error  = Average_filter(current_buf);
//
//		if(current_pid.Error > CURRENT_THERSHOLD)
//		{
//			PWM_stop();
//			Timer_stop();
//		}
//		else
//		{
//			PID_process(&voltage_pid);
//			PWM_update(voltage_pid.Out);
//		}
//
//
//    	counter++;

    	P1OUT ^= BIT0;
    	break;
    }
}


/******************************************************************************
 * END OF Buck_bost.C
*******************************************************************************/

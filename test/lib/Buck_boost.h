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
 *    Module       : Buck_boost.H
 *    Description  : This header file includes definitions and prototypes of
 *          basic configuration functions such as Initialization clock or delay
 *
 *  Tool           : CCS 6.1
 *  Chip           : MSP430G2xxx
 *  History        : 10/4/2018
 *  Version        : 1.0
 *
 *  Author         : Nguyen Khoi Nguyen, Pay It Forward Club
 *  Email		   : knguyentnh98@gmail.com
 *
 *  Notes          :
 *      To apply these functions, you must include the this header file and add
 *      the Basic_config.c file to your project.
 *
******************************************************************************/



#ifndef BUCK_BOOST_H_
#define BUCK_BOOST_H_

#include "Basic_config.h"
#include <stdint.h>

#define SMCLK_			16
#define SAMPLING_TIME   (SMCLK_*1000)     // sampling at __us with clock at SMCLK Hz

//PWM duty circle adjustment
#define TIME_INTERVAL	(500*SMCLK_)
#define DUTY_CIRCLE    	(uint16_t)(TIME_INTERVAL*0.7)			// 0 < DUTY_CIRCLE < 1
#define TIME_ON  		(uint16_t)(TIME_INTERVAL*DUTY_CIRCLE)


#define PIN_IN          BIT4        // read form P1.4
#define PIN_OUT			BIT1		// output at P2.1

#define LED_RED			BIT0
#define LED_GREEN		BIT6
#define BTN1			BIT3

#define SAMPLE_NUM		10
#define VOLTAGE_K		10
#define CURRENT_K		10

#define MAX_CURRENT		3			// units in ampe
#define CURRENT_THERSHOLD	(MAX_CURRENT*CURRENT_K)

typedef
    struct {
        // The fields are ordered to reduce memory over caused by struct-padding
		float Ki;
		float Kp;
		float Kd;
		float D_part;
		float I_part;
		float P_part;
		float T;
		float pre_Out;
		float Out;
		float pre_pre_Error;
		float pre_Error;
		float Error;

    }
pidparams;

typedef
    struct {
        // The fields are ordered to reduce memory over caused by struct-padding
		float Ki;
		float Kp;
		float Kd;
		char rev_buf[100];
    }
uartstring;

volatile uint16_t voltage_buf[SAMPLE_NUM];
volatile uint16_t current_buf[SAMPLE_NUM];

volatile pidparams voltage_pid;
volatile pidparams current_pid;

volatile uartstring uart_rev;

void Buck_boost_init(void);
void GPIO_init(void);
void Timer_start(void);
void Timer_stop(void);

void PWM_start(void);
void PWM_update(uint16_t duty);
void PWM_stop(void);

void PID_process(volatile pidparams *pid);
void PID_update(pidparams *pid);

uint16_t Average_filter(volatile uint16_t *array);

#endif /* LIB_BUCK_BOOST_H_ */

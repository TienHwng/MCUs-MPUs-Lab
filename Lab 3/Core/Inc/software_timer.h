/*
 * software_timer.h
 *
 *  Created on: Oct 10, 2025
 *      Author: Hwng
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE		10

extern int timer0_flag; // 2 Hz blinking LEDs
extern int timer1_flag; //
extern int timer2_flag;
extern int timer3_flag;

extern int timer0_counter;
extern int timer1_counter;
extern int timer2_counter;
extern int timer3_counter;

void setTimer0(int duration);
void setTimer1(int duration);
void setTimer2(int duration);
void timerRun(void);

#endif /* INC_SOFTWARE_TIMER_H_ */

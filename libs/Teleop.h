
#ifndef TELEOP_H
#define TELEOP_H

#ifndef JOYSTICKDRIVER_C
	#define JOYSTICKDRIVER_C
	#include "JoystickDriver.c"
#endif

/*
 * Controller button registers
 */

#define A				2
#define B				3
#define X				1
#define Y				4
#define LB				5
#define RB				6
#define LT				7
#define RT				8
#define SELECT				9
#define START				10

/*
 * D-Pad registers
 */
#define NORTH				0
#define NORTHEAST			1
#define EAST				2
#define SOUTHEAST			3
#define SOUTH				4
#define SOUTHWEST			5
#define WEST				6
#define NORTHWEST			7

/*
 * Teleop Specific stuff
 */
#ifndef NUM_DRIVE_MOTORS
	#define NUM_DRIVE_MOTORS	2
#endif
#define DRIVE_L 			(joystick.joy1_y1 * MAX_MOTOR / 127) //In order for PID to work we can't be at 100
#define DRIVE_R				(joystick.joy1_y2 * MAX_MOTOR / 127)
#ifndef MOTOR_SPEEDS
	#define MOTOR_SPEEDS
	#define MAX_MOTOR		95
	#define MIN_MOTOR		(-95)
#endif
#define NO_DRIVE_THRESHOLD		15
#define DEFAULT_SLEW_RATE		10	// 375 ms from full rev to fwd
#define FAST_SLEW_RATE			256
#define SLEW_TASK_DELAY			15	// task 1/freq in ms (66Hz)

/*
 * Button maps
 */

#define INTERNAL_BALL_COUNT_INCREASE	(joy2Btn(A))
#define INTERNAL_BALL_COUNT_DECREASE	(joy2Btn(B))

/*
 * Variable initializations
 */

Motors mtrList;
Robot bot_6133;
LG3 gyro;
I2CSensor SPB;

int motorReq[ NUM_MOTORS ];
int motorSlew[ NUM_MOTORS ];
int currInternalBallCount;

/*
 * function initializations
 */

void initializeRobot();
void leftDriveMotors(int val);
void rightDriveMotors(int val);

#endif

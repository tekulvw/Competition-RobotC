#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     _SPB,                sensorI2CCustom9V)
#pragma config(Sensor, S3,     light,          sensorLightActive)
#pragma config(Sensor, S4,     sonar,          sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     leftDrive,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightDrive,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "HTSPB-driver.h"
#include "libs/i2c.h"

#ifndef ROBOT_SOURCE
	#define ROBOT_SOURCE
	#include "libs/Robot.h"
	#include "libs/Robot.c" //Well this is stupid...ROBOTC doesn't link!
#endif

#ifndef LG3_SOURCE
	#define LG3_SOURCE
	#include "libs/LG3.h"
	#include "libs/LG3.c"
#endif

#ifndef JOYSTICKDRIVER_C
	#define JOYSTICKDRIVER_C
	#include "JoystickDriver.c"
#endif

#include "libs/Teleop.h"

tMotor allMtrs[8] = {leftDrive, rightDrive, 0, 0, 0, 0, 0, 0}; // Define NUM_MOTORS in Robot.h

void initializeRobot() {
	// SDA pin B0
	// SCL pin B1
	i2c_init(SPB);
	i2c_begin(SPB, _SPB, 0, 1);
	enableDefault(gyro);
	mtrList.mtrList = allMtrs;
	mtrList.numUsed = NUM_MOTORS;
	setupBot(bot_6133, 0, leftDrive, rightDrive, mtrList);
	resetEncoder(bot_6133);
	currInternalBallCount = getBallCount(bot_6133);
}

/*
 * Modifies left drive train speed target
 * @param val int <= abs(100)
 */
void leftDriveMotors(int val) {
	motorReq[0] = val;
}

/*
 * Modifies right drive train speed target
 * @param val int <= abs(100)
 */
void rightDriveMotors(int val) {
	motorReq[1] = val;
}

/*
 * Keeps track of internal ball count and stops/starts/reverses motors as necessary
 */
task InternalBallCountMonitor() {
	bool full = false;
	while (true) {
		currInternalBallCount = getBallCount(bot_6133);
		if (currInternalBallCount > 5) {
			// reverse intake motor(s) to get rid of the f***er
			full = true;
		} else if (currInternalBallCount == 5) {
			//Stop intake motor! Possibly reverse?
			full = true;
		} else {
			full = false;
		}
		if (!full) {
			//Let intake motor run normally
		}
		wait1Msec(25);
	}
}

/*
 * Takes motor speed target and increments actual speed at DEFAULT_SLEW_RATE
 */
task MotorSlew() {
	int motorIndex, motorTmp;
	for (motorIndex = 0; motorIndex != NUM_DRIVE_MOTORS; motorIndex++) {
		motorReq[motorIndex] = 0;
		motorSlew[motorIndex] = DEFAULT_SLEW_RATE;
	}
	while (true) {
		for (motorIndex = 0; motorIndex != NUM_DRIVE_MOTORS; motorIndex++) {
			motorTmp = motor[allMtrs[motorIndex]];
			if (motorTmp != motorReq[motorIndex]) {
				if (motorReq[motorIndex] > motorTmp) { //Need to speed up
					motorTmp += motorSlew[motorIndex];
					if (motorTmp > motorReq[motorIndex]) { //If over the what we need then go to what we need
						motorTmp = motorReq[motorIndex];
					}
				}
				if (motorReq[motorIndex] < motorTmp) { //Need to slow down
					motorTmp -= motorSlew[motorIndex];
					if (motorTmp < motorReq[motorIndex]) { // yada yada yada
						motorTmp = motorReq[motorIndex];
					}
				}
				motor[allMtrs[motorIndex]] = motorTmp;
			}
		}
		wait1Msec(SLEW_TASK_DELAY);
	}
}

/*
 * Modifies motor speed target based on joystick via [left/right]DriveMotors() and
 * 	has functionality to drive synchronously if joystick vals are close in number
 */
task TankDrive() {
	int drive_left, drive_right; // vals we want
	int average;
	while (true) {
		if ((abs(DRIVE_L) < NO_DRIVE_THRESHOLD) && (abs(DRIVE_R) < NO_DRIVE_THRESHOLD)) {
			drive_left = 0;
			drive_right = 0;
		} else if (abs(DRIVE_L - DRIVE_R) < NO_DRIVE_THRESHOLD)	{ // Accelerate in straight line
			average = (DRIVE_L/2 + DRIVE_R/2);
			drive_left = average;
			drive_right = average;
		} else {
			drive_left = (DRIVE_L > MAX_MOTOR) ? MAX_MOTOR :
								((DRIVE_L < MIN_MOTOR) ? MIN_MOTOR : DRIVE_L);
			drive_right = (DRIVE_R > MAX_MOTOR) ? MAX_MOTOR :
								((DRIVE_R < MIN_MOTOR) ? MIN_MOTOR : DRIVE_R);
		}
		writeDebugStreamLine("LEFT: %i RIGHT: %i", drive_left, drive_right);
		leftDriveMotors(drive_left);
		rightDriveMotors(drive_right);

		wait1Msec(25);
	}
}

/*
 * Monitor dedicated to Joystick 1
 */
task Joy1ButtonMonitor() {
	while (true) {
		abortTimeslice();
	}
}

/*
 * Monitor dedicated to Joystick 2 (that's you Ben)
 */
task Joy2ButtonMonitor() {
	while (true) {
		if (INTERNAL_BALL_COUNT_INCREASE) {
			hogCPU();			//why hog? guarantees ball count is updated prior to any more processing
			ballIntake(bot_6133);
			releaseCPU();
		}
		if (INTERNAL_BALL_COUNT_DECREASE) {
			hogCPU();
			ballScored(bot_6133);
			releaseCPU();
		}
		wait1Msec(25);
	}
}

task gyroMonitor() {
	while (true) {
		read(gyro);
		//Do stuff
		wait1Msec(25);
	}
}

task main()
{
	initializeRobot();

	StartTask(InternalBallCountMonitor);
	StartTask(MotorSlew);
	StartTask(TankDrive);
	StartTask(Joy1ButtonMonitor);
	StartTask(Joy2ButtonMonitor);
	//StartTask(gyroMonitor);

	//waitForStart();

	while (true) {
		getJoystickSettings(joystick);
	}

}
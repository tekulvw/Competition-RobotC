
#ifndef ROBOT_H
#define ROBOT_H

#define NUM_MOTORS			2
#ifndef NUM_DRIVE_MOTORS
	#define NUM_DRIVE_MOTORS	2
#endif
#ifdef MOTOR_SPEEDS
	#undef MOTOR_SPEEDS
#endif
#ifndef MOTOR_SPEEDS
	#define MOTOR_SPEEDS
	#define MAX_MOTOR		95
	#define MIN_MOTOR		(-95)
#endif
#define PPR				1120 // Encoder Pulses per Rotation: 1440 for TETRIX
#define DEGREES_PER_INCH		(PPR / 10) // PPR / Inch

/*
 * Watch out for the Andymark vs TETRIX motors.
 * http://goo.gl/ghNRKq
 */

typedef struct {
	tMotor mtrList[8];
	int numUsed;
} Motors;

typedef struct {
	int internalBallCount;
	tMotor leftDriveMotor, rightDriveMotor;
	Motors mtrList;
	int rotationDegreesPerInch; // Num of degrees (1440) per inch
} Robot;

void setupBot(Robot& bot, int ballCount, tMotor leftDriveMotor, tMotor rightDriveMotor, Motors& mtrList); //init bot struct
int move(Robot& bot, float dist, short speed); //Distance in inches
int move(tMotor mtr, float dist, short speed); // moves single motor
int move(Motors& mtrList, float dist, short speed); // moves motors in list
void stopMotor(Motors& mtrs); //stops all motors in struct
void stopMotor(tMotor mtr); //stops given motor
void stopDriveMotors(Robot& bot); //stops only drive motors
void resetEncoder(Robot& bot); //Resets all encoders on a bot
void resetEncoder(Motors& mtrs); //Reset all encoders in struct
void resetEncoder(tMotor mtr); //Resets given encoder
void resetDriveEncoders(Robot& bot); //resets only drive motors
void ballIntake(Robot& bot); //adds 1 to the internalBallCount
void ballIntake(Robot& bot, int num); //adds num to the internalBallCount
void ballScored(Robot& bot); //subtracts 1 from the internalBallCount
int getBallCount(Robot& bot); //derp

#endif

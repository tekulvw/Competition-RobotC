
#ifndef ROBOT_C
#define ROBOT_C

/*
 * initial setup of the bot
 * @param rotationRatio number of rotational degrees per inch
 * @param ballCount current internal ball count
 * @param leftEncoder left drive train motor
 * @param rightEncoder right drive train motor
 * @param motor list
 * @return robot bot struct
 */
void setupBot(Robot& bot, int ballCount, tMotor leftDriveMotor, tMotor rightDriveMotor, Motors& mtrList) {
	bot.internalBallCount = ballCount;
	bot.leftDriveMotor = leftDriveMotor;
	bot.rightDriveMotor = rightDriveMotor;
	bot.mtrList = mtrList;
	bot.rotationDegreesPerInch = DEGREES_PER_INCH;
	nMotorPIDSpeedCtrl[leftDriveMotor] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[rightDriveMotor] = mtrSpeedReg;
}

/*
 * takes a distance in inches and uses PID to drive straight in that direction
 * @param Robot& our bot struct
 * @param dist float inches
 * @param speed short less than or equal to 95
 */
int move(Robot& bot, float dist, short speed) {
	stopDriveMotors(bot);
	short direction = (dist > 0) ? 1 : (-1);
	resetDriveEncoders(bot);
	for (int i = 0; i != NUM_DRIVE_MOTORS; i++) {
		motor[bot.mtrList.mtrList[i]] = (direction * ((speed <= 95) ? speed : 95));
	}
	while (abs(nMotorEncoder[bot.leftDriveMotor]) < abs(dist * DEGREES_PER_INCH)) {}
	stopDriveMotors(bot);
	return 0;
}

/*
 * takes a distance in inches and moves single motor
 * @param mtr tMotor motor to move
 * @param dist float inches
 * @param speed short less than or equal to 95
 */
int move(tMotor mtr, float dist, short speed) {
	stopMotor(mtr);
	short direction = (dist > 0) ? 1 : (-1);
	resetEncoder(mtr);
	motor[mtr] = (direction * ((speed <= MAX_MOTOR) ? speed : MAX_MOTOR));
	while (abs(nMotorEncoder[mtr]) < abs(dist * DEGREES_PER_INCH)) {}
	stopMotor(mtr);
	return 0;
}

/*
 * takes a distance in inches and moves multiple motors
 * @param mtrList Motors& motors to move
 * @param dist float inches
 * @param speed short less than or equal to 95
 */
int move(Motors& mtrList, float dist, short speed) {
	stopMotor(mtrList);
	short direction = (dist > 0) ? 1 : (-1);
	resetEncoder(mtrList);
	for (int i = 0; i != mtrList.numUsed; i++) {
		motor[mtrList.mtrList[i]] = (direction * ((speed <= 95) ? speed : MAX_MOTOR));
	}
	while (abs(nMotorEncoder[mtrList.mtrList[0]]) < abs(dist * DEGREES_PER_INCH)) {}
	stopMotor(mtrList);
	return 0;
}

/*
 * Stops all Motors
 * @param Motors& Motors struct containing motors to stop
 */
void stopMotor(Motors& mtrs) {
	for (int i = 0; i != mtrs.numUsed; i++) {
		motor[mtrs.mtrList] = 0;
	}
}

/*
 * Stops specified motor
 * @param tMotor motor to stop
 */
void stopMotor(tMotor mtr) {
	motor[mtr] = 0;
}

void stopDriveMotors(Robot& bot) {
	for (int i = 0; i != NUM_DRIVE_MOTORS; i++) {
		motor[bot.mtrList.mtrList[i]] = 0;
	}
}

/*
 * Resets all motor encoders
 * @param Robot& bot struct
 */
void resetEncoder(Robot& bot) {
	for (int i = 0; i != bot.mtrList.numUsed; i++) {
		nMotorEncoder[bot.mtrList.mtrList[i]] = 0;
	}
}

/*
 * Resets specified motor encoder
 * @param tMotor motor to reset
 */
void resetEncoder(tMotor mtr) {
	nMotorEncoder[mtr] = 0;
}
/*
 * Resets all motor encoders in struct
 * @param Motors& Motors struct containing motors to reset
 */
void resetEncoder(Motors& mtrs) {
	for (int i = 0; i != mtrs.numUsed; i++) {
		nMotorEncoder[mtrs.mtrList[i]] = 0;
	}
}

/*
 * resets drive motor encoders on bot
 * @param bot Robot&
 */
void resetDriveEncoders(Robot& bot) {
	for (int i = 0; i != NUM_DRIVE_MOTORS; i++) {
		nMotorEncoder[bot.mtrList.mtrList[i]] = 0;
	}
}

/*
 * Adds one to the internalBallCount
 * @param Robot& our bot
 */
void ballIntake(Robot& bot) {
	bot.internalBallCount++;
}

/*
 * adds num to the internalBallCount
 * @param Robot& our bot
 * @param num to add to internal count
 */
void ballIntake(Robot& bot, int num) {
	int curr = bot.internalBallCount;
	bot.internalBallCount = curr+num;
}

/*
 * We scored. Yay! Minus one to the internal count
 * @param Robot& our bot
 */
void ballScored(Robot& bot) {
	bot.internalBallCount--;
}

/*
 * derpy get func
 */
int getBallCount(Robot& bot) {
	return bot.internalBallCount;
}

#endif

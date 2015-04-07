#include "MotorControllerMaster.h"

void MotorControllerMaster::begin(){
}

void MotorControllerMaster::setAcceleration(unsigned int forwardAcceleration, unsigned int ccwAcceleration, unsigned int forwardDeceleration, unsigned int ccwDeceleration){
	byte send[9];
	send[0] = COMMAND_SETACCELERATION;
	send[1] = forwardAcceleration;
	send[2] = forwardAcceleration >> 8;
	send[3] = ccwAcceleration;
	send[4] = ccwAcceleration >> 8;
	send[5] = forwardDeceleration;
	send[6] = forwardDeceleration >> 8;
	send[7] = ccwDeceleration;
	send[8] = ccwDeceleration >> 8;
	Wire.beginTransmission(MOTOR_CONTROLLER_ADDRESS);
	Wire.write(send,9);
	Wire.endTransmission();
}

void MotorControllerMaster::goVelocity(int forwardVelocity, int ccwVelocity){
	byte send[5];
	send[0] = COMMAND_GOVELOCITY;
	send[1] = forwardVelocity;
	send[2] = forwardVelocity >> 8;
	send[3] = ccwVelocity;
	send[4] = ccwVelocity >> 8;
	Wire.beginTransmission(MOTOR_CONTROLLER_ADDRESS);
	Wire.write(send,5);
	Wire.endTransmission();
}
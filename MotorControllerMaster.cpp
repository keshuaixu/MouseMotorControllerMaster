#include "MotorControllerMaster.h"

void MotorControllerMaster::begin(){
}

void MotorControllerMaster::setAcceleration(unsigned int forwardAcceleration, unsigned int ccwAcceleration, unsigned int forwardDeceleration, unsigned int ccwDeceleration){
	byte send[9];
	send[0] = COMMAND_SETACCELERATION;
	send[1] = forwardAcceleration;
	send[1] = forwardAcceleration >> 8;
	Wire.beginTransmission(MOTOR_CONTROLLER_ADDRESS);
	Wire.write(send,9);
	Wire.endTransmission();
}
#define INPUT_PULLUP
#define ENCODER_USE_INTERRUPTS
#define ENCODER_OPTIMIZE_INTERRUPTS

//#define CONTROL_BOARD_ONE

#include <Encoder.h>
#include <RegulatedMotor.h>;

const int M1ID = 1;
const int M2ID = 2;
const String BOARDID = "ID:2";


/*
timer 1 (controls pin 12, 11);
timer 2 (controls pin 10, 9);

prescaler = 1 ---> PWM frequency is 31000 Hz
prescaler = 2 ---> PWM frequency is 4000 Hz
prescaler = 3 ---> PWM frequency is 490 Hz (default value)
prescaler = 4 ---> PWM frequency is 120 Hz
prescaler = 5 ---> PWM frequency is 30 Hz
prescaler = 6 ---> PWM frequency is <20 Hz

*/

//TCCR1B = TCCR1B & 0b11111000 | 1;


Encoder enc1(18,19);
Encoder enc2(20,21);

RegulatedMotor m1(&enc1,4,5,9);
RegulatedMotor m2(&enc2,6,7,10);

void setup(){
	pinMode(4,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(6,OUTPUT);
	pinMode(7,OUTPUT);
	pinMode(9,OUTPUT);
	pinMode(10,OUTPUT);

        TCCR2B = TCCR2B & 0b11111000 | 0x02;
        bitSet(TCCR1B, WGM12);
        
	m1.setSampleTime(6);
	m2.setSampleTime(6);
        m1.setPID(0.2,0.07,0.0,0);
        m2.setPID(0.2,0.07,0.0,0);

	Serial.begin(115200);
        
        Serial.println(BOARDID);   
        
        //m1.setSpeed(1000);
        
}

int lastHeartbeat = 0;

void loop(){
	processInput();
	m1.run();
	m2.run();
        if ( millis()-lastHeartbeat > 1000 ){
          m1.setState(3);
          m2.setState(3);
        }
}



void dispatchMessage(String tag, int* ints, float* floats, int intCount, int floatCount){
  if (tag.equals("H")){
        lastHeartbeat = millis();
  }
  
  if (tag.equals("MSP")){
    if ( ints[0] == M1ID ) {
      m1.setSpeed(ints[1]);
      return;
    }
    if ( ints[0] == M2ID ) {
      m2.setSpeed(ints[1]);
      return;
    }
    return;
  } 
  
  if (tag.equals("MST")){
    if ( ints[0] == M1ID ) {
      m1.setState(ints[1]);
      return;
    }
    if ( ints[0] == M2ID ) {
      m2.setState(ints[1]);
      return;
    }
    return;
  } 
  
  if (tag.equals("MPID")){
  	if ( ints[0] == M1ID ) m1.setPID(floats[0],floats[1],floats[2],floats[3]);
  	if ( ints[0] == M2ID ) m2.setPID(floats[0],floats[1],floats[2],floats[3]);
  }
  


}








String currentTag;

int currentIntegers[16];
int currentIntegersPointer = 0;

boolean parsingFloat = false;
float currentFloats[16];
int currentFloatsPointer = 0;

String currentBuffer;

void processInput(){
  if (Serial.available()){
    char incoming = Serial.read();
    switch (incoming){
      case ',':
        if (parsingFloat) {
          char floatBuf[32];
          currentBuffer.toCharArray(floatBuf,sizeof(floatBuf));
          currentFloats[currentFloatsPointer++] = atof(floatBuf);
        } else {
          currentIntegers[currentIntegersPointer++] = currentBuffer.toInt();          
        }
        currentBuffer = "";
        parsingFloat = false;
        break;
      case ':':
        currentTag = currentBuffer;
        currentBuffer = "";
        break;
      case '\n':
        if (parsingFloat) {
          char floatBuf[32];
          currentBuffer.toCharArray(floatBuf,sizeof(floatBuf));
          currentFloats[currentFloatsPointer++] = atof(floatBuf);
        } else {
          currentIntegers[currentIntegersPointer++] = currentBuffer.toInt();          
        }
        currentBuffer = "";
        parsingFloat = false;
        
        dispatchMessage(currentTag, currentIntegers, currentFloats, currentIntegersPointer, currentFloatsPointer);
        currentIntegersPointer = 0;
        currentFloatsPointer = 0;
        break; 
      case '.':
        parsingFloat = true;
        currentBuffer += String(incoming);
        break;      
      default:
        currentBuffer += String(incoming);
        break;
    }
  }
}

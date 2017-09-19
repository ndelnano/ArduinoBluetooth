/**
 * Created by nickdelnano on 12/5/15.
 */

/*
* This Arduino script acts as a client to a Bluetooth server and implements a text-based protocol
* to control two sets of bidirectional motors using an Adafruit Motor Shield V2. There exits an Android
* application containing a server implementation that can be used with this code. The Android application 
* is published on the Google Play store, and its name and source code can be found at 
* https://github.com/ndelnano/BluetoothAndroidControllerAdafruitV2/blob/master/README.md.
*
* A blog post containing details of my use of these projects and a video demonstration
* be found here: http://ndelnano.github.io/2016/01/bluetooth-and-arduino
*
* The text based protocol is as follows:
*      'stop x' - stop all motors
*      'move {LEFT_DIRECTION}{LEFT_SPEED} {RIGHT_DIRECTION}{RIGHT_SPEED} x'
*          - Where *_DIRECTION are 0 or 1, and *_SPEED is in range [-255,255]
*              *_DIRECTION = 0 => forward, 1 => backwards
*
*
*  Ex: "move 0100 0100x" --sets both wheels forward at speed 100
*      "move 0255 1255x" --sets left wheel at max speed forward, right wheel at max speed backward
*              -- This will make your vehicle do an awesome donut! 
*/

#include "Adafruit_MotorShield.h"
#include <SoftwareSerial.h>


SoftwareSerial BTSerial(2, 3); // RX | TX

Adafruit_MotorShield shieldOne = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor1 = shieldOne.getMotor(1);
Adafruit_DCMotor *myMotor2 = shieldOne.getMotor(2);
Adafruit_DCMotor *myMotor3 = shieldOne.getMotor(3);
Adafruit_DCMotor *myMotor4 = shieldOne.getMotor(4);

String incomingString;
char received;
String leftSpeed;
String rightSpeed;

char c = ' ';
boolean NL = true;

void setup() 
{
      Serial.begin(9600);
      BTSerial.begin(9600);
      shieldOne.begin();
}

void loop()
{

 // While stream has data, read data from stream char by char
 while (BTSerial.available() > 0)
    {
        received = BTSerial.read();

        if (received == 'x')
        { 
            String commandString = getCommand(incomingString);

            if (commandString == "stop")
            {
              stopMotors();
            }
            
            if (commandString == "move")
            {
              leftSpeed = getLeftSpeed(incomingString);
              rightSpeed = getRightSpeed(incomingString);

              setLeftMotors(leftSpeed.substring(1, leftSpeed.length()).toInt(), leftSpeed.substring(0,1).toInt());
              setRightMotors(rightSpeed.substring(1, rightSpeed.length()).toInt(), rightSpeed.substring(0,1).toInt());
            }

            // Clear received buffer
            incomingString = ""; 
        }
        
        if (received != 'x')
        {
            incomingString += received; 
        }
    }

    
}

void setLeftMotors(int speed, int direction) {
  myMotor3->setSpeed(speed);
  myMotor4->setSpeed(speed);

  if(direction == 0) {
    myMotor3->run(FORWARD);
    myMotor4->run(FORWARD);
  }
  else if(direction == 1) {
    myMotor3->run(BACKWARD);
    myMotor4->run(BACKWARD);
  }
}

void setRightMotors(int speed, int direction) {
  myMotor1->setSpeed(speed);
  myMotor2->setSpeed(speed);

  if(direction == 0) {
    myMotor1->run(FORWARD);
    myMotor2->run(FORWARD);
  }
  else if(direction == 1) {
    myMotor1->run(BACKWARD);
    myMotor2->run(BACKWARD);
  }
}

void stopMotors() {
  myMotor1->run(RELEASE);
  myMotor2->run(RELEASE);
  myMotor3->run(RELEASE);
  myMotor4->run(RELEASE);
}

String getCommand(String inputtedString) {
  
  String commandString = "";

  for(int i = 0; i < inputtedString.length(); i++) 
  {
    if(inputtedString[i] == ' ') {
      return commandString;
      }

      commandString += inputtedString[i];
  }
}

String getLeftSpeed(String inputtedString) {

  String leftSpeed = "";
  boolean atNumbers = false;

  for(int i = 0; i < inputtedString.length(); i++) 
  {
    if(inputtedString[i] == ' ' && atNumbers)
      return leftSpeed;
      
    if(atNumbers)
      leftSpeed+= inputtedString[i];
      
    if(inputtedString[i] == ' ') {
        atNumbers = true;
      }
  }
}

String getRightSpeed(String inputtedString) {
  String rightSpeed = "";
  boolean atLeftNumbers = false;
  boolean atRightNumbers = false;

  for(int i = 0; i < inputtedString.length(); i++) 
  {
    if(inputtedString[i] == 'x')
      return rightSpeed;
      
    if(atLeftNumbers && atRightNumbers)
      rightSpeed+= inputtedString[i];
      
    if(inputtedString[i] == ' ' && atLeftNumbers) {
        atRightNumbers = true;
      }

      if(inputtedString[i] == ' ')
      atLeftNumbers = true;
  }
}


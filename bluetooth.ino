/**
 * Created by nickdelnano on 12/5/15.
 */
//Arduino code to connect Adafruit motor shield V2 to bluetooth controller
 
#include <Adafruit_MotorShield.h>
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

//FORMAT TO RECEIVE A COMMAND

//*COMMAND* + " " + "DIRECTION_LEFT_INT" + "VALUES_LEFT" + " " + "DIRECTION_RIGHT_INT" + "VALUES_RIGHT" + "x";
//0 signals forward, 1 signals backward

//"move 0100 0100x" --sets both wheels forward at speed 100

//commands defined: move, stop



void setup() 
{
      Serial.begin(9600);
      BTSerial.begin(9600);
      shieldOne.begin();
}

void loop()
{

 while (BTSerial.available() > 0)
    {
        received = BTSerial.read();

        if (received == 'x')
        { 
            String commandString = getCommand(incomingString);

            if(commandString == "stop")
            {
              stopMotors();
            }
            
            if(commandString == "move")
            {
              leftSpeed = getLeftSpeed(incomingString);
              rightSpeed = getRightSpeed(incomingString);

              Serial.println(rightSpeed);
              Serial.println(leftSpeed);
              
              setLeftMotors(leftSpeed.substring(1, leftSpeed.length()).toInt(), leftSpeed.substring(0,1).toInt());
              setRightMotors(rightSpeed.substring(1, rightSpeed.length()).toInt(), rightSpeed.substring(0,1).toInt());
            }

            incomingString = ""; // Clear recieved buffer
        }
        
        if(received != 'x')
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


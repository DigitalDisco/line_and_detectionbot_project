//Speed test
#include <Servo.h>

Servo servoLeft;
Servo servoRight;

void setup() {
  // put your setup code here, to run once:
  tone(4,1500,1000);
  delay(5000);
  servoLeft.attach(12);
  servoRight.attach(11);

  int melodyNoteArray[] = {2500, 4000, 2500, 3500, 4000};
  int melodyTimeArray[] = {200, 200, 200, 500, 500};

  //To prevent turning while going straight
  int forwardLeftMs = 1595;
  int forwardRightMs = 1395;

  driveStraight(40, forwardLeftMs, forwardRightMs);
  
  turnRight(200);
  
  driveStraight(80, forwardLeftMs, forwardRightMs);
  
  turnLeft(650);
  
  driveStraight(60, forwardLeftMs, forwardRightMs);

  standStill();

  delay(200);

  // Melodi
  for( int i = 0; i < 5; i++){
    tone(4, melodyNoteArray[i], melodyTimeArray[i]);
    delay(melodyTimeArray[i]);
  }

  delay(1600);
  
  //Turn around
  turnRight(1200);
  standStill();
  miniturnLeft();

  driveStraight(10, forwardLeftMs, forwardRightMs);

  turnRight(300);

  driveStraight(80, forwardLeftMs, forwardRightMs);

  turnLeft(650);
  
  driveStraight(80, forwardLeftMs, forwardRightMs);

  standStill();

  servoLeft.detach();
  servoRight.detach();
}

void turnLeft(int time)                      // Left turn function
{
  for(int left = 1595; left > 1500; left -= 1){
    servoLeft.writeMicroseconds(left);         // Left wheel clockwise
    servoRight.writeMicroseconds(1400);        // Right wheel clockwise
    delay(30);
  }
  servoLeft.writeMicroseconds(1500);         // Left wheel clockwise
  servoRight.writeMicroseconds(1400);
  delay(time);                               // Maneuver for time ms
}

void miniturnLeft()                           // Essentially a left end-of-turn function
{
  for(int left = 1595; left > 1530; left -= 1){
    servoLeft.writeMicroseconds(left);         // Left wheel clockwise
    servoRight.writeMicroseconds(1400);        // Right wheel clockwise
    delay(30);
  }                              
  standStill();
}

void turnRight(int time)                     // Right turn function
{
  for(int right = 1395; right < 1500; right +=1){
    servoLeft.writeMicroseconds(1600);
    servoRight.writeMicroseconds(right);
    delay(30);
  }
  servoLeft.writeMicroseconds(1600);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1500);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

void driveStraight(int ticks, int forwardLeftMs, int forwardRightMs){
  for (int tick = 0; tick <= ticks; tick +=1){
    servoLeft.writeMicroseconds(forwardLeftMs);
    servoRight.writeMicroseconds(forwardRightMs);
    delay(20);
  }
}

void standStill (){
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

void loop() {
  

}

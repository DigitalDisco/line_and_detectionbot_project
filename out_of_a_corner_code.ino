//Speed test
#include <Servo.h>

Servo servoLeft;
Servo servoRight;

void setup() {
  
  tone(4,1500,1000);
  delay(5000);
  servoLeft.attach(12);
  servoRight.attach(11);

  //Set up for detectors
  pinMode(7, INPUT);
  pinMode(5, INPUT);

  //Set up for LEDs
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  
}

void turnLeft(int time)                      // Left turn function
{
  for(int left = 1595; left > 1500; left -= 5){
    servoLeft.writeMicroseconds(left);         // Left wheel clockwise
    servoRight.writeMicroseconds(1400);        // Right wheel clockwise
    delay(30);
  }
  servoLeft.writeMicroseconds(1500);         // Left wheel clockwise
  servoRight.writeMicroseconds(1400);
  delay(time);                               // Maneuver for time ms
}


void turnRight(int time)                     // Right turn function
{
  for(int right = 1395; right < 1500; right +=5){
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

//Counters used in the code
int totalCount = 0;
int leftCount = 0;
int rightCount = 0;
int forwardCount = 0;

//Flashes the specified LEDs a specified number of times
void flash50ms(bool left, bool right, int times){
  
  for(int i = 0; i < times; i++){
    if(left){
      digitalWrite(2, HIGH);  //Turn the left LED on if specified
    }
    if(right){
      digitalWrite(8, HIGH);  //Turn the right LED on if specified
    }
    delay(80);                //Wait, turn them off, wait
    digitalWrite(2, LOW);
    digitalWrite(8, LOW);
    delay(50);
  }
}

void loop() {
  //Setup
  byte wLeft = digitalRead(7);
  byte wRight = digitalRead(5);

  //If the robot was just started, have both lights on to indicate that nothing has been detected
  if(totalCount == 0){
    digitalWrite(2, HIGH);
    digitalWrite(8, HIGH);
  } else{
    digitalWrite(2, LOW);
    digitalWrite(8, LOW);
  }
  
  
  //In each loop, drive forward for a bit
  driveStraight(5, 1595, 1395);

  //If the number of total hits by the detectors grows to large, try a larger manuevor.
  //  Try this manuevur more and more often if the robot can't find its way out.
  if(totalCount % 10 == 0 && totalCount != 0){
    driveStraight(80, 1395, 1595);
    flash50ms(true, true, 5);
    if (totalCount < 80){
      totalCount += int(totalCount/10);
    } else{
      flash50ms(true, true, 3);
      totalCount = 80;
    }
    turnLeft(400);
  }

  //Detection on the left side
  if (wLeft == 0){
    //Reset the counters
    forwardCount = 0;
    rightCount = 0;

    //If the robot has done a lot of turns in one directions lately, do a larger one
    if(leftCount > 5){
      leftCount += 1;
      totalCount += 1;
      flash50ms(true, false, 3);
      driveStraight(80, 1395, 1595);
      turnLeft(600);
    } else{
      //Otherwise, do a smaller one
      flash50ms(true, false, 1);
      leftCount += 1;
      totalCount += 1;
      driveStraight(40, 1395, 1595);
      turnLeft(200);
    }
  }

  //Works the same way as wLeft
  if (wRight == 0){
    forwardCount = 0;
    leftCount = 0;
    if (rightCount > 5){
      rightCount += 1;
      totalCount += 1;
      flash50ms(false, true, 3);
      driveStraight(80, 1395, 1595);
      turnRight(400);
    } else{
      flash50ms(false, true, 1);
      rightCount += 1;
      totalCount += 1;
      driveStraight(40, 1395, 1595);
      turnRight(200);
    }
  }

  //Increase a counter if the robot drives forward. 
  if (wLeft == 1 && wRight == 1){
    forwardCount += 1;

    //If the robot has done a lot of forward lately, it might be stuck, start flashing to warn people.
    if (forwardCount > 60){
      flash50ms(true, true, 4);
    }
    //The robot thinks it's stuck, stops, flashes angrily and reverses out and does a large turn
    if (forwardCount > 64){
      standStill();
      digitalWrite(2, LOW);
      digitalWrite(8, LOW);
      delay(200);
      flash50ms(true, true, 20);
      driveStraight(80, 1395, 1595);
      turnRight(700);
      forwardCount = 0;
    }
  }

}

/*
 * Robotics with the BOE Shield - RoamingWithIr
 * Adaptation of RoamingWithWhiskers with IR object detection instead of
 * contact switches.
 */

#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
 
void setup()                                 // Built-in initialization block
{ 
  pinMode(10, INPUT);  pinMode(9, OUTPUT);   // Left IR LED & Receiver
  pinMode(3, INPUT);  pinMode(2, OUTPUT);    // Right IR LED & Receiver

  tone(4, 3000, 1000);                       // Play tone for 1 second
  delay(1000);                               // Delay to finish tone

  servoLeft.attach(12);                      // Attach left signal to pin 13 
  servoRight.attach(11);                     // Attach right signal to pin 12

  Serial.begin(9600);

  long baseline = rcTime(6);   

  int status_int = 0;

  while(true){
    int irLeft = irDetect(9, 10, 38000);       // Check for object on left
    int irRight = irDetect(2, 3, 38000);       // Check for object on right
    Serial.println("int: " + status_int);
    if(status_int == 0){
      forward(100);
      if(irLeft == 0){
        stop_moving();
        turnLeft(520); //Hitta 90 grader
        status_int++;
      }
    }
    else if(status_int == 1){
      forward(100);
      if(irRight == 1){
        stop_moving();
        turnRight(300); //Hitta 90 grader
        status_int++;
        Serial.println("Turning right...");
      }
    }
    else if(status_int == 2){
      forward(100);
      if(irRight == 1){
        forward(50);
        turnRight(250);
        Serial.println("Turning right...");
        status_int++;
      }
    }
    else if(status_int == 3){
      forward(100);
      if(irLeft == 0){
        stop_moving();
        turnLeft(520); //Hitta 90 grader
        status_int++;
      }
    }
    else if(status_int == 4){
      forward(100);
      if(irLeft == 0){
        forward(1350);
        turnLeft(800);
        Serial.println("Turning left...");
        status_int++;
      }
    }
    else if (status_int == 5){
      forward(100);
      if (rcTime(6) < baseline/2){
        servoLeft.detach();
        servoRight.detach();
      }
    }
    
  }
}  

long rcTime(int pin)                         // ..returns decay time
{                                            
  pinMode(pin, OUTPUT);                      // Charge capacitor 
  digitalWrite(pin, HIGH);                   // ..by setting pin ouput-high
  delay(1);                                  // ..for 5 ms
  pinMode(pin, INPUT);                       // Set pin to input
  digitalWrite(pin, LOW);                    // ..with no pullup
  long time  = micros();                     // Mark the time
  while(digitalRead(pin));                   // Wait for voltage < threshold
  time = micros() - time;                    // Calculate decay time
  return time;                               // Return decay time
}  
void loop()                                  // Main loop auto-repeats
{/*
  int irLeft = irDetect(9, 10, 38000);       // Check for object on left
  int irRight = irDetect(2, 3, 38000);       // Check for object on right

  if((irLeft == 0) && (irRight == 0))        // If both sides detect
  {
    stop_moving();                           // Turn left about 120 degrees
    Serial.println("Both sides detect");
  }
  else if(irRight == 0)                      // If only right side detects
  {
    forward(200);                          // Back up 1 second
    Serial.println("Right side detect");
  }
  else                                       // Otherwise, no whisker contact
  {
    turnRight(800);                            // Forward 1/50 of a second
    Serial.println("Forward");
  }*/
}

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  

void forward(int time)                       // Forward function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnLeft(int time)                      // Left turn function
{
  for(int left = 1595; left > 1500; left -= 1.2){
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
  for(int right = 1395; right < 1500; right +=2){
    servoLeft.writeMicroseconds(1600);
    servoRight.writeMicroseconds(right);
    delay(30);
  }
  servoLeft.writeMicroseconds(1600);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1500);        // Right wheel counterclockwise
  delay(time);  
}

void backward(int time)                      // Backward function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

void stop_moving(){
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}

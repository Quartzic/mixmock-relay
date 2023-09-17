/* DrinkPour
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/
#include <Servo.h>

Servo myServo[6];  // create servo objects to control a servo
// twelve servo objects can be created on most boards

const byte servoPins[] = {1,2,3,4,5,6};


int pos[6] = {110, 100, 100, 100, 100, 110};    // variable to store the servo position
int low[6] = {45, 45, 45, 45, 45, 55};
int high[6] = {110, 100, 100, 100, 100, 110};
int incomingByte = 90;
int drink = -1;
int amount = -1;

void setup() {
  for (int i = 0; i < 6; i++){
    myServo[i].attach(servoPins[i]);
    myServo[i].write(high[i]); 
  }

  // testAllDrinks();

  // myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  // myservo.write(90); 
  // turnServoUp();
}

void testAllDrinks(){
  for (int i = 0; i < 6; i++){
    letDrinkOut(i, 2000);
  }
}

void loop() {
  while (Serial.available() == 0) {}    //wait for data available
  if (drink == -1) drink = Serial.parseInt();  //read until timeout
  else if (amount == -1) amount = Serial.parseInt();                        // remove any \r \n whitespace at the end of the String

  if (drink != -1 && amount != -1){
    letDrinkOut(drink, amount * 1000);
    drink = -1;
    amount = -1;
  }
  // if (Serial.available() > 0) {
  //   // turnServoB();
  //   incomingByte = Serial.read();
  //   Serial.print("I got: ");
  //   Serial.println(incomingByte, DEC);
  // }

  // { t = Serial.read();
  //    } while (Serial.available() > 0);
  
  // for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                    // waits 15 ms for the servo to reach the position
  // }
  // delay(1000);

  // for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  // delay(1000);
}

// void turnServo(){
//   for (int temp = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                    // waits 15 ms for the servo to reach the position
//   }
//   delay(1000);

//   for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//   delay(1000);
// }

// void turnServoB(){

//   for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//   delay(1000);

//   for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                    // waits 15 ms for the servo to reach the position
//   }
//   delay(1000);
// }

// void turnServoUp(){

//   for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//   delay(1000);
// }

void turnServoUp(int drinkNum, int timeWaitAfter = 1000){

  while (pos[drinkNum] <= high[drinkNum]) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo[drinkNum].write(pos[drinkNum]);              // tell servo to go to position in variable 'pos'
    delay(15);
    pos[drinkNum] += 1;                       // waits 15 ms for the servo to reach the position
  }
  delay(timeWaitAfter);
}

void turnServoDown(int drinkNum, int timeWaitAfter = 1000){

  while (pos[drinkNum] >=low[drinkNum]) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo[drinkNum].write(pos[drinkNum]);              // tell servo to go to position in variable 'pos'
    delay(15);
    pos[drinkNum] -= 1;                       // waits 15 ms for the servo to reach the position
  }
  delay(timeWaitAfter);
}

void letDrinkOut(int drinkNum, int drinkPourTime){
  turnServoDown(drinkNum, drinkPourTime);
  turnServoUp(drinkNum);
}

void letDrinkOutReverse(int drinkNum, int drinkPourTime){
  turnServoUp(drinkNum, drinkPourTime);
  turnServoDown(drinkNum);
}

// void turnServoDown(){

//   while (pos <= 90) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);
//     pos+= 1;                       // waits 15 ms for the servo to reach the position
//   }
//   delay(1000);
// }

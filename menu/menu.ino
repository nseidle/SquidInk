
//created by Lili Cushing-Quevli
//modified: December 1st, 2019

//IN PROGRESS
// 1) Match with relay switches
// 2) Make the PCB board
// 3) Make functions to press each button for easy calling in loop sequence function
// 4) Edit button pushing code to work with larger than length 3 (optional)
// 5) TEST THIS CODE TO MAKE SURE IT FUNCTIONS AS PLANNED


#include <Servo.h>
Servo myservo;
//citation for Servo library and code:
//by BARRAGAN <http://barraganstudio.com>
//This example code is in the public domain.
//https://www.arduino.cc/en/Tutorial/Sweep
// modified 8 Nov 2013
// by Scott Fitzgerald
//****RELAYS*****
int relay1 = 13;
int relay2 = 2;
int relay3 = 3;
int relay4 = 4;
int relay5 = 5;
int relay6 = 6;
int relay7 = 7;
int relay8 = 8;
int relay9 = 9;
int relay0 = 10;
int EnterA = 11;
int EnterB = 12;

int delayTime=500;


//used in the menu, typesetting
float reading = 0;
//array of all of manually entered values
float testedComboArray[100];
//keep track of which value in the array is free :)
int increment = 0;
//entered code, set with returned value. If it's -1, something's wrong
int codeLength = 0;
//used in loopSequence function
int comboArray[3] = {0, 0, 0};

//*****SERVO MOTOR********
//position of motor
int pos = 0;
int servoPin = 12;

void setup()
{
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  pinMode(relay9, OUTPUT);
  pinMode(relay0, OUTPUT);
  pinMode(EnterA, OUTPUT);
  pinMode(EnterB, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  digitalWrite(relay7, HIGH);
  digitalWrite(relay8, HIGH);
  digitalWrite(relay9, HIGH);
  digitalWrite(relay0, HIGH);
  digitalWrite(EnterA, HIGH);
  

  myservo.attach(servoPin);
  Serial.begin(9600);

  printMenu();


}

void loop() {

  while (Serial.available() > 0)
  {

    //if there's something to read, do something

    reading = Serial.read();

    if (reading == '\n') {
      Serial.print("\n");

    }

    //Serial.println(reading);
    if (reading == '1')
    {

      codeLength = enterLength();

    }

    else if (reading == '2')
    {
      manualEnter();

    }

    else if (reading == '3')
    {
      loopSequence();

    }

    else
    {
      //error catching
      //      Serial.print("Oops! You entered somthing that is not in this menu.  ");
      //      Serial.println("Please try again");

    }


  }


}

void printMenu()
{
  Serial.println("Cracking a Keypad Safe, Choose an option");
  Serial.println("******When the safe combo is found, the squids will light up*******");
  Serial.println("1. Enter how long the combination is");
  Serial.println("2. Manually enter a code");
  Serial.println("3. Start loop sequence");


}

int enterLength()
{
  delay(10);
  while (Serial.available() > 0) {

    Serial.read();
    //clears out the buffer

  }

  Serial.println("ENTER THE LENGTH OF COMBO");
  while (Serial.available() == 0) {

    //Do nothing
  }
  while (Serial.available() > 0)
  {

    //if there's something to read, take that number
    //make it the length

    int inputLength = Serial.parseInt();
    Serial.print(inputLength);

    return inputLength;

  }

}


//TO DO: Modify so that the entered code is saved and skips over it
//when going through the sequence, print to serial monitor
void manualEnter()
{
  delay(10);
  while (Serial.available() > 0)
  {

    //if there's something to read, take that number
    //save it in a value/ array
    //for now has an array of a large size, but could consider adding vector library, at the cost of memory
    //eeprom?
    //wait for carriage return

    while (Serial.available() > 0) {

      Serial.read();
      //clears out the buffer

    }

    Serial.println("ENTER THE COMBO (Do not use letters, please only 3 digits long)");
    while (Serial.available() == 0) {

      //Do nothing
    }
    while (Serial.available() > 0)
    {

      //if there's something to read, take that number
      //make it the length

      int testCombo = Serial.parseInt();
      Serial.print("The combo you've entered is: ");
      Serial.println(testCombo);

      //code citation
      //by Christop
      //source link: https://forum.arduino.cc/index.php?topic=307479.0
      //parses the full code entered and separates it into individual numbers
      unsigned n = testCombo;
      unsigned firstVal = (n / 100U) % 10;
      unsigned secondVal = (n / 10U) % 10;
      unsigned thirdVal = (n / 1U) % 10;

      //      Serial.println(firstVal);
      //      Serial.println(secondVal);
      //      Serial.println(thirdVal);

      //press the numbers entered
      pressNumber(firstVal);
      pressNumber(secondVal);
      pressNumber(thirdVal);
      pressEnter();

      //add it to the already tested combinations
      testedComboArray[increment] = testCombo;

      //increment global variable
      increment++;
      return;

    }




  }


}

void loopSequence()
{
  Serial.println("Sequence of all possible combos is about to begin");
  bool found = 0;
  if (codeLength != 0)
  {
    while (found == 0)
    {
      //by default, array is set to have all zeros
      //start with 0,0,0 and then increment the last value, going until the last value hits the tenth value
      //increment the second value and start the for loop again
      for (int i = 0; i < 10; i++)
      {
        //call switch case menu to execute the pressing of the buttons
        pressNumber(comboArray[0]);
        pressNumber(comboArray[1]);
        pressNumber(i);

      }
      //if the second combo does not have the value nine in it
      if (comboArray[1] < 10)
      {
        //move up one.
        //Example : instead of 0,0,0
        // it is now 0,1,[i]
        comboArray[1] += 1;

      }

      else
      {
        //if we have already gone through the second and third values with the current first value
        //and none worked, we increment the first value and try every possible value for the
        //second and third
        if (comboArray[0] < 10)
        {
          comboArray[0] += 1;
          comboArray[1] = 0;
        }

        else
        {
          //all other codes have been exhausted, if the code has not been found, exit the while loop
          found = 1;


        }


      }


    }

  }
  else
  {
    Serial.println("Does not have code length. Please enter before proceeding");
    return;
  }



}


//TODO: print to serial monitor code that was inputted
//NEEDS TO BE TESTED
void pressNumber( int var)
{
  switch (var)
  {
    case 0:
      pressZero();
      break;

    case 1:
      pressOne();
      break;

    case 2:
      pressTwo();
      break;

    case 3:
      pressThree();
      break;

    case 4:
      pressFour();
      break;

    case 5:
      pressFive();
      break;

    case 6:
      pressSix();
      break;

    case 7:
      pressSeven();
      break;

    case 8:
      pressEight();
      break;

    case 9:
      pressNine();
      break;


  }



}


void turnHandle()
{

  //I DID NOT WRITE THIS CODE
  //reference: https://www.arduino.cc/en/Tutorial/Sweep

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

}


void stopSequence()
{
  //delay for a four minutes and 30 seconds while the pcb times out
  delay(270000);

}



//************RELAY PRESS*******************
void pressOne()
{
  digitalWrite(relay1, LOW);
  delay(delayTime);
  digitalWrite(relay1, HIGH);



}
void pressTwo()
{
  digitalWrite(relay2, LOW);
  delay(delayTime);
  digitalWrite(relay2, HIGH);


}
void pressThree()
{
  digitalWrite(relay3, LOW);
  delay(delayTime);
  digitalWrite(relay3, HIGH);


}
void pressFour()
{
  digitalWrite(relay4, LOW);
  delay(delayTime);
  digitalWrite(relay4, HIGH);



}
void pressFive()
{
  digitalWrite(relay5, LOW);
  delay(delayTime);
  digitalWrite(relay5, HIGH);

}
void pressSix()
{
 digitalWrite(relay6, LOW);
  delay(delayTime);
  digitalWrite(relay6, HIGH);

}
void pressSeven()
{
  digitalWrite(relay7, LOW);
  delay(delayTime);
  digitalWrite(relay7, HIGH);
}
void pressEight()
{
  digitalWrite(relay8, LOW);
  delay(500);
  digitalWrite(relay8, HIGH);


}
void pressNine()
{
  digitalWrite(relay9, LOW);
  delay(delayTime);
  digitalWrite(relay9, HIGH);


}
void pressZero()
{
  digitalWrite(relay0, LOW);
  delay(delayTime);
  digitalWrite(relay0, HIGH);

}

void pressEnter()
{
  digitalWrite(EnterA, LOW);
  delay(delayTime);
  digitalWrite(EnterA, HIGH);

}

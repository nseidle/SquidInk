
//created by Lili Cushing-Quevli
//modified: December 11th, 2019

//IN PROGRESS
// 1) Match with relay switches
// 2) Make the PCB board
// 3) Make functions to press each button for easy calling in loop sequence function
// 4) Edit button pushing code to work with larger than length 3 (optional)
// 5) TEST THIS CODE TO MAKE SURE IT FUNCTIONS AS PLANNED

//this is testing

#include <EEPROM.h>

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
bool resumeLastKnown = 0;
int readResistor;

//*******LEDS**************
int LED1 = A5;
int LED2 = A4;
int LED3 = A3;
int LED4 = A2;
int LED5 = A1;


//**********EEPROM***********
int addr = 0;


int photoResistor = A0;

int delayTime = 114;

//can go up to 3 codes before needing delay sequence called.
int timeOut = 1;

//used in the menu, typesetting
float reading = 0;
//array of all of manually entered values
float testedComboArray[2];
int randSelectCombo[1000];
//keep track of which value in the array is free :)
int increment = 0;
//entered code, set with returned value. If it's -1, something's wrong
int codeLength = 0;
//used in loopSequence function
int comboArray[3] = {0, 0, 0};
int winCodeArray[3] = {0, 0, 0};

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

  //fill the random combo array with values from 0-999
  int beginningIncrement = 0;
  for (int i = 0; i < 1000; i++)
  {
    randSelectCombo[i] = i;
  }


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
      startCombo();


    }

    else if (reading == '2')
    {
      manualEnter();

    }

    else if (reading == '3')
    {
      loopSequence();

    }

    else if (reading == '4')
    {
      startCombo();


    }

    else if (reading == '5')
    {

      retrieveWinCode();

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
  Serial.println("1. Set starting code");
  Serial.println("2. Manually enter a code");
  Serial.println("3. Start loop sequence");
  Serial.println("4. Set Code Manually");
  Serial.println("5. Retrieve Win Code :)");


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
      //

      //press the numbers entered
      pressNumber(firstVal);
      pressNumber(secondVal);
      pressNumber(thirdVal);
      pressEnter();
      //Serial.println("Checking Winstate: ");
      bool testWin = winState();



      if (testWin == true)
      {
        Serial.print("Congratulations, you've entered the right combo. Your Combo is: ");
        Serial.print(firstVal);
        Serial.print(", ");
        Serial.print(secondVal);
        Serial.print(", ");
        Serial.println(thirdVal);

      }

      return;


    }




  }


}

void loopSequence()
{
  Serial.println("Sequence of all possible combos is about to begin");
  Serial.println("Press 'f' at any time to stop sequence");
  Serial.println("Combo array is:");

  bool found = 0;


  int addrTest = 0;
  int comboVal;
  if (EEPROM.read(7) == 1)
  {
    Serial.println("There was something previously stored here...");
    for (int j = 0; j < 3; j++)
    {
      comboVal = EEPROM.read(addrTest);
      addrTest += 2;

      Serial.print(comboVal, DEC);


      comboArray[j] = comboVal;

    }
  }
  Serial.print("\n");

  while (found == 0)
  {

    //by default, array is set to have all zeros
    //start with 0,0,0 and then increment the last value, going until the last value hits the tenth value
    //increment the second value and start the for loop again



    for (int i = comboArray[2]; i < 10; i++)
    {
      //call switch case menu to execute the pressing of the buttons


      pressNumber(comboArray[0]);

      pressNumber(comboArray[1]);

      pressNumber(i);



      pressEnter();
      Serial.print(comboArray[0]);
      Serial.print(",");
      Serial.print(comboArray[1]);
      Serial.print(",");
      Serial.println(i);

      addrTest = 0;

      for (int j = 0; j < 3; j++)
      {
        int writeCombo = 0;
        writeCombo = comboArray[j];
        EEPROM.write(addrTest, writeCombo);
        addrTest += 2;

      }



      if (winState())
      {
        Serial.println("WINNING CODE WAS:");

        Serial.print(comboArray[0]);
        Serial.print(",");
        Serial.print(comboArray[1]);
        Serial.print(",");
        Serial.println(i);
        int tracker = 8;

        //save in the secondary values of EEPROM, get in other function
        EEPROM.write(tracker, comboArray[0]);
        tracker += 2;
        EEPROM.write(tracker, comboArray[1]);
        tracker += 2;
        EEPROM.write(tracker, i);


        //        Serial.println("Congratulations, The correct combo has been found! Your Combo is: ");
        //        Serial.print(comboArray[0]);
        //        Serial.print(", ");
        //        Serial.print(comboArray[1]);
        //        Serial.print(", ");
        //        Serial.print(i);
        //        //turnHandle();
        //        Serial.println("Exiting function....");
        found = 1;
        break;
      }



      if (timeOut % 3 == 0)
      {
        stopSequence();
      }
      timeOut++;


    }

    comboArray[2] = 0;
    //if the second combo does not have the value nine in it
    if (comboArray[1] < 9)
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
      if (comboArray[1] >= 9)
      {
        comboArray[0] += 1;
        comboArray[1] = 0;
      }



    }


  }
  return;

}

void startCombo()
{
  delay(10);
  Serial.println("ENTER THE COMBO TO BEGIN LOOP (Do not use letters, please only 3 digits long)");
  while (Serial.available() > 0)
  {

    //if there's something to read, take that number
    //save it in a value/ array
    //for now has an array of a large size, but could consider adding vector library, at the cost of memory

    //wait for carriage return

    while (Serial.available() > 0) {

      Serial.read();
      //clears out the buffer

    }


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

      Serial.println("Setting the code....");
      int counting = 0;

      EEPROM.write(counting, firstVal );
      counting += 2;
      EEPROM.write(counting, secondVal);
      counting += 2;
      EEPROM.write(counting, thirdVal);

      EEPROM.write(7, 1);

      return;
    }
  }



}

void retrieveWinCode()
{
  Serial.println("Entered retrieve Win");

  int tracker = 8;
  int saver = 0;
  
  Serial.print("The Safes code is: ");
  saver = EEPROM.read(tracker);
//  Serial.println(saver);
//  Serial.print(saver);

  winCodeArray[0] = saver;
  
  
  tracker += 2;

  saver = EEPROM.read(tracker);
//  Serial.print(saver);
  winCodeArray[1] = saver;
  tracker += 2;

  saver = EEPROM.read(tracker);
//  Serial.print(saver);
  winCodeArray[2] = saver;

  Serial.print("YOUR WNNING CODE WAS: ");
  for (int i = 0; i < 3; i++)
  {
   
    Serial.print(winCodeArray[i]);

  }
  Serial.print("\n");



}

void resetCombo()
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

    Serial.println("Setting the Combo: Please enter your 3 digit Code you would like to set.");
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

      pressNumber(firstVal);
      pressNumber(secondVal);
      pressNumber(thirdVal);
      pressEnter();



      return;

    }
  }


}


//TODO: print to serial monitor code that was inputted

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



bool winState()
{
  int counter = 0;
  //credit to Nathan Seidle for this code.
  //Wait for 1 second while the LED flashes (or doesn't)
  while (1)
  {
    counter++;
    if (counter > 1000)
    {
      return false;
    }
    delay(1);
    int temp = analogRead(photoResistor);
    //Serial.println(temp);
    if (temp > 100)
    {
      //Serial.println("GREEN");

      break;

    }

  }



  //Serial.println("CODE FOUND!!!!!!");


  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED3, HIGH);
  delay(500);
  digitalWrite(LED4, HIGH);
  delay(500);
  digitalWrite(LED5, HIGH);






  return true;



}





void stopSequence()
{
  //delay for a four minutes and 46 seconds while the pcb times out
  delay(286500);

}



//************RELAY PRESS*******************
void pressOne()
{
  digitalWrite(relay1, LOW);
  delay(delayTime);
  digitalWrite(relay1, HIGH);
  delay(delayTime);



}
void pressTwo()
{
  digitalWrite(relay2, LOW);
  delay(delayTime);
  digitalWrite(relay2, HIGH);
  delay(delayTime);


}
void pressThree()
{
  digitalWrite(relay3, LOW);
  delay(delayTime);
  digitalWrite(relay3, HIGH);
  delay(delayTime);


}
void pressFour()
{
  digitalWrite(relay4, LOW);
  delay(delayTime);
  digitalWrite(relay4, HIGH);
  delay(delayTime);



}
void pressFive()
{
  digitalWrite(relay5, LOW);
  delay(delayTime);
  digitalWrite(relay5, HIGH);
  delay(delayTime);

}
void pressSix()
{
  digitalWrite(relay6, LOW);
  delay(delayTime);
  digitalWrite(relay6, HIGH);
  delay(delayTime);

}
void pressSeven()
{
  digitalWrite(relay7, LOW);
  delay(delayTime);
  digitalWrite(relay7, HIGH);
  delay(delayTime);
}
void pressEight()
{
  digitalWrite(relay8, LOW);
  delay(delayTime);
  digitalWrite(relay8, HIGH);
  delay(delayTime);

}
void pressNine()
{
  digitalWrite(relay9, LOW);
  delay(delayTime);
  digitalWrite(relay9, HIGH);
  delay(delayTime);

}
void pressZero()
{

  digitalWrite(relay0, LOW);
  delay(delayTime);
  digitalWrite(relay0, HIGH);
  delay(delayTime);

}

void pressEnter()
{
  digitalWrite(EnterA, LOW);
  delay(delayTime);
  digitalWrite(EnterA, HIGH);
  delay(10);


}

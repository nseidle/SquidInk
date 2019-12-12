
//created by Lili Cushing-Quevli
//modified: December 11th, 2019

//DESIGNED TO WORK WITH A 3 DIGIT CODE
//simulate pressing of buttons with a switch menu
//take user input and then properly press keys
//store winning value in EEPROM if using loop sequence function (can be unplugged from computer at this point)
//otherwise prints to serial monitor when winning code is found

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

int readResistor;

//*******LEDS**************
int LED1 = A5;
int LED2 = A4;
int LED3 = A3;
int LED4 = A2;
int LED5 = A1;




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
//optional, currently commented out. For usage with the 3D printed handle cover and a servo motor (included in GH repo)
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

  //all relay switches are set to HIGH to be set LOW later when a button needs pressed
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

  //menu only prints once
  printMenu();


}

void loop() {

  //credit to Nathan Seidle for help with the code + lecture on serial.available
  while (Serial.available() > 0)
  {

    //if there's something to read, do something

    reading = Serial.read();

    if (reading == '\n') {
      Serial.print("\n");

    }

    //Serial.println(reading);
    //Didn't want to do ACII, so we do char reading instead
    if (reading == '1')
    {
      //sets EEMPROM code starting point
      startCombo();


    }

    else if (reading == '2')
    {
      //takes input from serial monitor and enters in correct buttons
      manualEnter();

    }

    else if (reading == '3')
    {
      //starts from EEPROM value, then increments in order, by going up one digit at a time
      loopSequence();

    }

    else if (reading == '4')
    {
      //set the combo
      startCombo();


    }

    else if (reading == '5')
    {
      //retrieve stored EEPROM win value once winState is triggered
      retrieveWinCode();

    }


    else
    {
      //don't do anything

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


void manualEnter()
{
  delay(10);
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
  Serial.println("Combo array is:");

  bool found = 0;


  int addrTest = 0;
  int comboVal;

  //the 7th value is a boolean to determine if it was ever run (w/o a board reset) that way, we can fill in 
  //the values, if they exist
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
        //int is 2 bytes, increment accordingly
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


      //every 3 combos, it requires a time out of 4 minutes and 46 seconds
      if (timeOut % 3 == 0)
      {
        stopSequence();
      }
      timeOut++;


    }

    //since the comboArray[2] was initially set with the EEPROM value, we now need to set it back to 0 when it starts the for loop again
    comboArray[2] = 0;

    //if the second combo does not have the value nine in it
    if (comboArray[1] < 9)
    {
      //move up one.
      //Example : instead of 0,0,0
      // it is now 0,1,[i]
      comboArray[1] += 1;


    }
    //on the 8th time, it will increment to 9 and then go through the for loop one last time with 9, then when it comes back
    //it is no longer less than 9, and will go to the next statement

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

  //get the EEPROM secondary values and then store in local array

  Serial.print("The Safes code is: ");
  saver = EEPROM.read(tracker);
  winCodeArray[0] = saver;


  tracker += 2;

  saver = EEPROM.read(tracker);
  winCodeArray[1] = saver;
  tracker += 2;

  saver = EEPROM.read(tracker);
  winCodeArray[2] = saver;

  Serial.print("YOUR WNNING CODE WAS: ");
  //print the array stored in EEPROM
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


//menu that takes a number passed into it and presses that number

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
      //if it has been more than a second, then the light has not flashed and we exit
      return false;
    }
    delay(1);
    int temp = analogRead(photoResistor);
    //Serial.println(temp);
    if (temp > 100)
    {
      //Serial.println("GREEN");
      //if the photoresistor has registered the LED green light, then leave

      break;

    }

  }



  //Serial.println("CODE FOUND!!!!!!");

  //turn on LEDS
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED3, HIGH);
  delay(500);
  digitalWrite(LED4, HIGH);
  delay(500);
  digitalWrite(LED5, HIGH);





//if we broke out of the loop without returning false, it has turned green, code found
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

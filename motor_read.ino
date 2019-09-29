
#define motorDirection 5
#define motorSpeed 6
#define encoderA 2
#define encoderB 3


int revCounter = 0;
int aTicks = 0;
int bTicks = 0;


void setup() {
  // put your setup code here, to run once:



  pinMode(motorDirection, OUTPUT);
  pinMode(motorSpeed, OUTPUT);
  pinMode(encoderA, INPUT);
  pinMode(encoderB, INPUT);

  Serial.begin(9600);





}

void loop() {
  // put your main code here, to run repeatedly:

  //  if (digitalRead(motorDirection) == LOW)
  //  {
  //
  //    Serial.println("is OFF");
  //  }
  //
  //  else
  //  {
  //    Serial.println("is ON");
  //  }
  //
  //if (digitalRead(motorSpeed) == HIGH)
  //  {
  //
  //    Serial.println("Motor speed is flowing");
  //  }
  //
  //  else
  //  {
  //    Serial.println("motor speed is NOT FLOWING");
  //  }


  digitalWrite(motorDirection, LOW);
  analogWrite(motorSpeed, 100);


  //count how many times the encoder is going around
  if (digitalRead(encoderA) == HIGH)
  {

    Serial.println("Encoder A is HIGH");
    aTicks++;
    Serial.println("A Ticks:");
    Serial.println(aTicks);
    Serial.println(bTicks);

    if (aTicks == 40)
    {
      Serial.println("STOP THE FUNCTION, FULL REVOLUTION");
      attachInterrupt(digitalPinToInterrupt(encoderA), countRev, RISING);
      attachInterrupt(digitalPinToInterrupt(encoderB), countRev, LOW);


    }

    if (digitalRead(encoderB) == HIGH)
    {
      Serial.println("Encoder B is HIGH");
      bTicks++;
      Serial.println("B Ticks:");
      Serial.println(bTicks);

    }



  }

 

}

void countRev()
{
  Serial.println("DELAYED");
  Serial.println("# revolutions");
  
  revCounter++;
  Serial.println(revCounter);
  bTicks = 0;
  aTicks = 0;
  delay(400);


}

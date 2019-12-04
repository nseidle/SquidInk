int relay1 = 13;
int relay5 = 5;
int relay9 = 9;
int Enter = 11;
int LED1 = A1;
int photoResistor = A0;
void setup() {
  // put your setup code here, to run once:
  pinMode(relay1, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay9, OUTPUT);
  pinMode(Enter, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(photoResistor, INPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay9, HIGH);
  digitalWrite(Enter, HIGH);
  Serial.begin(9600);

}

void loop() {
  digitalWrite(relay1, LOW);
  delay(1000);
  digitalWrite(relay1, HIGH);
  delay(1000);


  digitalWrite(relay5, LOW);
  delay(1000);
  digitalWrite(relay5, HIGH);
  delay(1000);

  digitalWrite(relay9, LOW);
  delay(1000);
  digitalWrite(relay9, HIGH);
  delay(1000);

  digitalWrite(Enter, LOW);
  delay(1000);
  digitalWrite(Enter, HIGH);
  delay(1000);
  
  int value = analogRead(photoResistor);
  Serial.println(value);

if(value>45){
  digitalWrite(LED1, HIGH);
  delay(10000);
  digitalWrite(LED1, LOW);
  }
  






}

#include <Servo.h>

const int servoPin = 7;
const int irPin = 13;
const int trigPin = 9;
const int echoPin = 11;

Servo doorControl;

void setup() 
{
  Serial.begin(9600);
  doorControl.attach(servoPin);
  pinMode(irPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() 
{
  watchForWaste();
  delay(1000);
}

void watchForWaste()
{
  int check = digitalRead(irPin);
  delay(1000);
  if(!check)
  {
    Serial.println("Waste Detected");
    Serial.println("Opening Bin");
    delay(50);
    openBin();
  }
}

void openBin()
{
  int pos;
  for (pos = 0; pos <= 100; pos += 1) 
  { 
    doorControl.write(pos);            
    delay(25);                   
  }
  Serial.println("Waiting to drop waste");
  delay(50);
  if(waitToDrop())
  {
    closeBin();
  }
}

bool waitToDrop()
{
  delay(3000);
  int checkWorkDone = check();
  while(checkWorkDone < 60)
  {
    checkWorkDone = check();
    delay(1000);
  }
  return true;
}

int check()
{
  int duration;
  int distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
}

void closeBin()
{
  Serial.println("Waste dropped, Closing bin");
  delay(50);
  int pos;
  for (pos = 100; pos >= 0; pos -= 1) 
  {
    doorControl.write(pos);         
    delay(25);                     
  }
}
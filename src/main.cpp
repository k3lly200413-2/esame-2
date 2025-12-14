#include<Servo.h>
#include <Arduino.h>

Servo myServo;

int pirOutPin = 2;
int val = LOW;
int pirState = LOW;
const float BETA = 3950; // should match the Beta Coefficient of the thermistor

int ledPin = 13;

void setup()
{
  myServo.attach(9);
}

void loop()
{
  myServo.write(15);
  open();
  close();
}

void open()
{
  myServo.write(180);
  for (int i = 0; i < 180; i++)
  {
    myServo.write(i);
    delay(15);
  }
}

void close()
{

  myServo.write(-180);

  for (int i = 180; i > 0; i--)
  {
    myServo.write(i);
    delay(15);
  }
}

void presenceSensorLoop()
{
  val = digitalRead(pirOutPin);
  if (val == HIGH)  // if it is
  {
    digitalWrite(ledPin, HIGH); //Turn the LED on
    if (pirState == LOW)        // if the last state was OFF
    {
      pirState = HIGH;          // set the last state to HIGH
      Serial.println("Motion detected");
    }
  }
  else            // if the sensor is NOT detecting motion now
  {
    digitalWrite(ledPin, LOW);  // turn OFF the LED
    if (pirState == HIGH)       // If the last state was HIGH
    {
      pirState = LOW;           // Set the last state to LOW
      Serial.println("Motion Stopped after 5 seconds");
    }
  }
}

void tempSensor()
{
  int analogValue = analogRead(A0);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" ℃");
  delay(1000);
}
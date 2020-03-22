//All the libraries needed
#include <LiquidCrystal.h>
#include <IRremote.h>  //including infrared remote header file     
#include "SevSeg.h"

//Sets up a seven segment, and a liquid crystal
SevSeg sevseg;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable, d4, d5, d6, d7)

//Sets variables to control speed, motor pins, and LCD display times
int speed1 = 0;
int m1 = 13;
int m2 = 12;
int a1 = 0, a2 = 0;
int value;
int DISPLAY_TIME = 100;  // In milliseconds

//Sets variables for Analog Pins for RGB LED
const int RED_PIN = A2;
const int GREEN_PIN = A1;
const int BLUE_PIN = A0;

//Sets up variables for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

//Sets up variables to calculate distance
long duration;
int distanceCm, distanceInch, adjustedDistance;

//Used to setup IR receeiver
int RECV_PIN = 8; // the pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN);
decode_results results;

//Sets up all of the pins required from the different components, along with a the serial monitor for troubleshooting
void setup()
{
  lcd.begin(16, 2);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Setup");

  irrecv.enableIRIn();

//Sets up variables for the 7 segment display
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {4, 3, 11, 1, 2, 5, 6, 7};
  bool resistorsOnSegments = true;

  byte hardwareConfig = COMMON_CATHODE;
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

}


void loop()
{

  //Used to read value from remote, to determine which button was pressed
  if (irrecv.decode(&results)) {
    value = results.value;
    Serial.println(value);
    irrecv.resume();
  }


  //For when walker is turned "on"
  if (value == 26775) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * 0.034 / 2; //Conversion rate for centimeteres
    distanceInch = duration * 0.0133 / 2; //Conversion rate for inches

    lcd.setCursor(0, 0);
    lcd.print("Distance: "); // Prints string "Distance" on the LCD
    lcd.print(distanceCm); // Prints the distance value from the sensor

    lcd.print("     ");

    adjustedDistance = distanceCm;

    if (adjustedDistance > 60) {
      adjustedDistance = 60;
    }
    Serial.println(adjustedDistance);
    if (adjustedDistance <= 9)
    {

      //Sets motor to 100% speed, and displays a 1
      sevseg.setNumber(1);
      delayMicroseconds(10);

      sevseg.refreshDisplay();

      a1 = 30000;      // subtracts a2 from 1000 ans saves the result in a1
      a2 = 0;
      digitalWrite(m1, HIGH); //Sets both motors to high
      digitalWrite(m2, HIGH);//Sets both motors to high
      delayMicroseconds(a1);
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      delayMicroseconds(a2);

      Serial.println(a2);

      //Displays a colour on the RGB
      analogWrite(A0, HIGH);
      analogWrite(A1, LOW);
      analogWrite(A2, LOW);
      delayMicroseconds(5);
      //mainColors();
      //showSpectrum();


    }
    //delay(10);

    else if (adjustedDistance >= 30)
    {

      //Sets motor to 0 speed, and turns off. Also displays a 3

      sevseg.setNumber(3);
      delayMicroseconds(30);

      sevseg.refreshDisplay();
      a2 = 30000;
      a1 = 0;      // subtracts a2 from 1000 ans saves the result in a1
      digitalWrite(m1, HIGH);
      digitalWrite(m2, HIGH);
      delayMicroseconds(a1);
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      delayMicroseconds(a2);
      Serial.println(a2);


      //Displays a colour on the RGB
      analogWrite(A2, LOW);
      analogWrite(A1, LOW);
      analogWrite(A0, HIGH);
      delayMicroseconds(5);

    }

    else
    {
      sevseg.setNumber(2);
      delayMicroseconds(30);

      sevseg.refreshDisplay();
      //Sets motor to medium / slow walking pace, and displays a 2
      a2 = 25000;
      a1 = 30000 - a2;    // subtracts a2 from 1000 ans saves the result in a1
      digitalWrite(m1, HIGH);
      digitalWrite(m2, HIGH);
      delayMicroseconds(a1);
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      delayMicroseconds(a2);
      Serial.println(a2);

      //Displays a colour on the RGB
      analogWrite(A2, LOW);
      analogWrite(A1, LOW);
      analogWrite(A0, HIGH);
      delayMicroseconds(5);

    }
  }

  //For when walker is turned "off"
  else if (value == 12495) {
    return;
  }

}

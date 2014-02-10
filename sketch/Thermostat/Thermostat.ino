#include <SerialLCD.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <json_arduino.h>

const int rotPin =  2;      // rotation
const int ledPin =  6;      // the number of the LED pin
const int buttonPin = 8;     // the number of the pushbutton pin
const int lcdPin =  11;      // LCD


const int temperatureAnalogPin = 0;
const int rotaryAnalogPin = 1;

SerialLCD slcd(lcdPin,12);

int requestedTemperature = 0;
float temperature = 0;

const int minTemperature = 15;
const int maxTemperature = 30;

int lastStatus = LOW;
int lightStatus = 0;         // variable for reading the pushbutton status

unsigned long timer1 = 0;
unsigned long timer2 = 0;

float getTemperature() {

  int a = analogRead(temperatureAnalogPin);
  int B=3975;                  //B value of the thermistor

  float resistance = (float)(1023-a)*10000/a; //get the resistance of the sensor;
  float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

  return temperature;
}

int getRotaryData() {
  // range: 0-1023
  int value = analogRead(rotaryAnalogPin);

  Serial.print("Rotary value");
  Serial.println(value);
  
  return value;
}

int checkTimer(unsigned long *timer, unsigned long interval) {
  unsigned long currTimer = millis();
  if(currTimer - *timer > interval) {
    // save the last time you blinked the LED
    *timer = currTimer;
    return 1;
  }
  return 0;
}

void readSerial() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    
    String content = "";
    char character;
  
    while(Serial.available()) {
      
        character = Serial.read();
        if(character == '\n')
          break;
          
        content.concat(character);
        delay(10);
    }

    Serial.println("Command:");
    Serial.println(content);

  }
}

void setup() {
  
  Serial.begin(9600);
  
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
 
  // set up
  slcd.begin();
  slcd.backlight();
  
}

void loop() {
  // read the state of the pushbutton value:
  int buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {

    if(checkTimer(&timer2, 500) == 1) {    
      Serial.println("Button click!");
      
      if(lightStatus == 0) {
        // turn LED on:    
        digitalWrite(ledPin, HIGH);  
        slcd.backlight();
        lightStatus = 1;
      }
      else {
        digitalWrite(ledPin, LOW);      
        slcd.noBacklight();
        lightStatus = 0;
      }
    }
  }
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  slcd.setCursor(0, 0);  
  slcd.print("Temperature");  

  if(checkTimer(&timer1, 1000) == 1) {
    
    temperature = getTemperature();
    slcd.setCursor(0, 1);  
    slcd.print(temperature, 2);  

    Serial.print("{");
//    Serial.print("{ millis: ");
//    Serial.print(millis());
//    Serial.print(", ");
    Serial.print("temperature: ");
    Serial.print(temperature);
    Serial.print(", requested: ");
    Serial.print(requestedTemperature);
    Serial.print("}\n");
   
  }

  // getRotaryData
  getRotaryData();
  
  /*
   * Read from serial
   */
   readSerial(); 
  
}


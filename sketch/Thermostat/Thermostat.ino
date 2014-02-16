#include <SerialLCD.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <json_arduino.h>

// digital pin
const int relayPin =  2;      // relay
const int ledPin =  6;      // the number of the LED pin
const int buttonPin = 8;     // the number of the pushbutton pin
const int lcdPin1 =  11;      // LCD
const int lcdPin2 =  12;      // LCD

// analog pin
const int temperatureAnalogPin = 0;
const int rotaryAnalogPin = 1;

SerialLCD slcd(lcdPin1,lcdPin2);


int requestedTemperature = 0;
float temperature = 0;

const int celsius = 1; // 1 celsius / 0 farheneit, but has to be handled thought
const int minTemperature = 15; // means OFF
const int maxTemperature = 30; // means VERY HOT

int rotaryValue = 0;

int lastStatus = LOW;
int lightStatus = 0;         // variable for reading the pushbutton status

unsigned long timer1 = 0;
unsigned long timer2 = 0;

int getTemperature() {

  int a = analogRead(temperatureAnalogPin);
  int B=3975;                  //B value of the thermistor

  float resistance = (float)(1023-a)*10000/a; //get the resistance of the sensor;
  float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

  return (int)temperature;
}

float getRotaryData() {
  // range: 0-1023
  int value = analogRead(rotaryAnalogPin);
  
//  minTemperature
//  maxTemperature

  // find value for 1 of 15 on rotary value
  float tempValue = minTemperature + (value / (1024/ (maxTemperature - minTemperature) ));

  if(tempValue != rotaryValue) {
    Serial.print("New rotary value: ");
    Serial.print(value);
    Serial.print(" - temp: ");
    Serial.println(tempValue);
  }

  rotaryValue = tempValue;
  return tempValue;
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
 
  pinMode(relayPin, OUTPUT);
 
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
//      Serial.println("Button click!");
      
      if(lightStatus == 0) {
        slcd.backlight();
        lightStatus = 1;
      }
      else {
        slcd.noBacklight();
        lightStatus = 0;
      }
    }
  }
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
//  slcd.setCursor(0, 0);  
//  slcd.print("Temperature");  

  // getRotaryData
  float rotaryVal = getRotaryData();
  
  // @todo change only if rotary has been used
  requestedTemperature = rotaryVal;
  
  temperature = getTemperature();
  
  
  // write to display  
  slcd.setCursor(0, 0);
  slcd.print(temperature, 0);
//  slcd.print("°");  
  slcd.print("            ");
  slcd.print(rotaryVal, 0);
//  slcd.print("°");

  slcd.setCursor(0, 1);
  
  for(int i = minTemperature; i <= maxTemperature; i++) {
    if(requestedTemperature >= i)
      slcd.print("#");
    else
      slcd.print(" ");
  }

  // turn on system if temperature is less than requested!
  if(requestedTemperature > temperature) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(relayPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);
  }
  
   
  if(checkTimer(&timer1, 1000) == 1) {   

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
  
  /*
   * Read from serial
   */
   readSerial(); 
  
}


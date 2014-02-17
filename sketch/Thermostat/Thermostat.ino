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


float requestedTemperature = 0;
float temperature = 0;

float rotaryValue = 0;



const int celsius = 1; // 1 celsius / 0 farheneit, but has to be implemented
const int minTemperature = 15; // means OFF
const int maxTemperature = 30; // means VERY HOT

int lastStatus = LOW;
int lightStatus = 0;         // variable for reading the pushbutton status

unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timerBackLight = 0;

int backLightTimeout = 5000;

int getTemperature() {

  int a = analogRead(temperatureAnalogPin);
  int B=3975;                  //B value of the thermistor

  float resistance = (float)(1023-a)*10000/a; //get the resistance of the sensor;
  float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;

  return (int)temperature;
}

int getRotaryData() {
  // range: 0-1023
  int value = analogRead(rotaryAnalogPin);
  
//  minTemperature
//  maxTemperature

  // find value for 1 of 15 on rotary value
  float tempValue = minTemperature + (value / (1024/ (maxTemperature - minTemperature) ));
  
  int changed = 0;
  if(tempValue != rotaryValue) {
    
    changed = 1;
    
    /*    
    Serial.print("New rotary value: ");
    Serial.print(value);
    Serial.print(" - temp: ");
    Serial.println(tempValue);
    */
  }

  rotaryValue = tempValue;
  
  return changed;
}

int checkTimer(unsigned long *timer, unsigned long interval) {
  unsigned long currTimer = millis();
  if(currTimer - *timer > interval) {
    *timer = currTimer;
    return 1;
  }
  return 0;
}

/*
  Handle timed LCD backLight on/off based on 
*/
int __backlightOn = 0;
void backLightHandler(int turnOn) {
  
  if(turnOn == 1) {
    
    if(__backlightOn != 1) {    
//      Serial.println("TURN **ON** LIGHT");
      __backlightOn = 1;
      slcd.backlight();
    }
  
  }
  else {
    
    if(__backlightOn != 0) {
      if(checkTimer(&timerBackLight, backLightTimeout) == 1) {
//        Serial.println("TURN **OFF** LIGHT");      
        __backlightOn = 0;
        slcd.noBacklight();  
      }
    }
    
  }
}

void backLightOff() {
  timerBackLight = 0;
  backLightHandler(0);
}

void backLightOn() {
  timerBackLight = millis();
  backLightHandler(1);
}

int readSerial() {
  
  int val = 0;
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

//    Serial.println("Command:");
//    Serial.println(content);

    char str[256];
    content += "\0";
    
    strncpy(str, content.c_str(), sizeof(str));

    token_list_t *token_list = NULL;
    token_list = create_token_list(25); 
    
    json_to_token_list(str, token_list); 
    val = atoi(json_get_value(token_list, "r"));

    release_token_list(token_list); // Release or Wipe the Token List, else memory-leak at your own peril.
    
  }
  
  return val;
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
  backLightOn();

}

void loop() {

  // read the state of the pushbutton value:
  int buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  //
  /*  
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
  //*/
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
//  slcd.setCursor(0, 0);  
//  slcd.print("Temperature");   
  

  // getRotaryData
  if(getRotaryData() == 1) {
    backLightOn();
    requestedTemperature = rotaryValue;
  } 

  /*
   * Read from serial
   */
  int serialVal = readSerial();
  if(serialVal > 0) {
    backLightOn();
    requestedTemperature = serialVal;
  }
    
  temperature = getTemperature();
  
  // write to display  
  slcd.setCursor(0, 0);
  slcd.print(temperature, 0);
//  slcd.print("°");  
  slcd.print("            ");
  slcd.print(requestedTemperature, 0);
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
    Serial.print("\"t\":");
    Serial.print(temperature);
    Serial.print(",\"r\":");
    Serial.print(requestedTemperature);
    Serial.print("}\n");
   
  }
     
  backLightHandler(0);
  
}


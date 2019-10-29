
/* 
 * Example of using the ChainableRGB library for controlling a Grove RGB.
 * This code puts a red dot (led) moving along a strip of blue dots.
 */

//what is needed:
//if press button> corresponding led will change colour and will send point to team
//cannot change tiles with coolof period of XXX sec or when they are 'frozen / on fire'
//frozen tiles provide different colour

#include <ChainableLED.h>
#define NUM_LEDS  5

ChainableLED leds(8, 9, NUM_LEDS); //in pins 8+9
const int buttonPin = 2;   

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int ledState = HIGH;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void setup()
{
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  int reading = digitalRead(buttonPin); //buttn high or low
  for (byte i=0; i<NUM_LEDS; i++){
    
    leds.setColorRGB(i, 255*reading, 0, 0);
  }
    
//      if (reading != lastButtonState) { //if button is pressed or unpressed
//        lastDebounceTime = millis();  //update button
//      }
//    
//      if ((millis() - lastDebounceTime) > debounceDelay) { //if aproved by timer
//    
//        if (reading != buttonState) { //if button has changed 
//          buttonState = reading; //update button (again?)
//    
//          if (buttonState == HIGH) { //if pressed
//            ledState = !ledState; //if pressed
//          }
//        }
//      }// end of button debounce
//    leds.setColorRGB(i, 255*ledState, 0, 0); 
//     
//    } //end of ledloop
    delay(100);
    Serial.print( " button: ");
    Serial.print(buttonState);
    Serial.println( " reading: ");
    Serial.println(reading);
    
}//end of void loop

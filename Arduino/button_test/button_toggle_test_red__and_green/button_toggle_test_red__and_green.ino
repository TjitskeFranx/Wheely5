/* switch
 * 
 * Each timestamp the input pin goes from LOW to HIGH (e.g. because of a push-button
 * press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 * a minimum delay between toggles to debounce the circuit (i.e. to ignore
 * noise).  
 *
 * David A. Mellis
 * 21 November 2006
 */

int ButtonVal = 2;         // the number of the input pin
int GreenLED = 13;       // the number of the output pin green
int RedLED = 10;       // the number of the output pin red

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin


bool buttonVal;
bool prevButtonVal;
unsigned long previousMillisRedLed;
int redLedInterval = 500; //ms
bool redLedState;

enum {ST_GREEN, ST_RED} currentState = ST_GREEN; //so ST_GREEN is 0, ST_RED is 1 in the switch..case

//comment

// the follow variables are long's because the timestamp, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long timestamp = 0;         // the last timestamp the output pin was toggled
long debounce = 2000;   // the debounce timestamp, increase if the output flickers

void setup()
{
  Serial.print(9600);
  pinMode(Button, INPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  
}

void loop()
{buttonVal = digitalRead(button);

  // put your main code here, to run repeatedly:
  switch (currentState)
  {
    case (ST_GREEN):
      //solid on
      digitalWrite(greenLed, HIGH);

      //check if button is pressed to take us out of this state
      if (buttonVal != prevButtonVal) //button changed...
      {
        if (!buttonVal) //.... to pressed
        {
#ifdef DEBUG
          Serial.println("going to red");
#endif
          digitalWrite(greenLed, LOW);
          currentState = ST_RED;
        }//button pressed
      }//button changed
      prevButtonVal = buttonVal;
      break;

    case (ST_RED):
      //blink
      if (millis() - previousMillisRedLed >= redLedInterval)
      {
        redLedState = !redLedState;
        digitalWrite(redLed, redLedState);
        previousMillisRedLed = millis();
      }

      //check if button is pressed to take us out of this state
      if (buttonVal != prevButtonVal) //button changed...
      {
        if (!buttonVal) //.... to pressed
        {
#ifdef DEBUG
          Serial.println("going to green");
#endif
          digitalWrite(redLed, LOW);
          currentState = ST_GREEN;
        }//button pressed
      }//button changed
      prevButtonVal = buttonVal;
      break;

  }//switch


}//loop
  
  
//  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the timestamp
  //if (reading == HIGH && previous == LOW && millis() - timestamp > debounce) {
    //if (state == HIGH)
      state = LOW;
    //else
      state = HIGH;

    //timestamp = millis();    
  }

  //digitalWrite(outPin, state);
  //Serial.println( timestamp);
  //previous = reading;
}

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

int inPin = 2;         // the number of the input pin
int groenPin = 12;       // the number of the output pin
int roodPin = 10;

String state = "rood";
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the timestamp, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long timestamp = 0;         // the last timestamp the output pin was toggled
long debounce = 2000;   // the debounce timestamp, increase if the output flickers

void setup()
{
  Serial.begin(9600);
  pinMode(groenPin, OUTPUT);
  pinMode(roodPin, OUTPUT);
  digitalWrite(roodPin, LOW);
  digitalWrite(groenPin, LOW);
  
}

void loop()
{
//  if (state=="rood"){
//      digitalWrite(groenPin, HIGH);
//      delay(100);
//      digitalWrite(groenPin, LOW);
//      delay(100);
//      digitalWrite(groenPin, HIGH);
//      delay(100);
//  }
  reading = digitalRead(inPin); //is knoppie aan of niet. HIGH = ingedrukt

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the timestamp
  
  if (reading == HIGH && previous == LOW && millis() - timestamp > debounce) {
      digitalWrite(roodPin, LOW);
      delay(500);
    
    if (state=="groen"){ //als ie groen was > zet naar rood
      state="rood";
      digitalWrite(roodPin, HIGH);
      digitalWrite(groenPin, LOW);
    }
    else if (state=="rood"){ //als ie rood was > zet naar groen
      state="groen";
      digitalWrite(roodPin, LOW);
      digitalWrite(groenPin, HIGH);
    }
    timestamp = millis();
    Serial.println( timestamp);
    
  }
  
  Serial.println(state);
  Serial.println("fdsf");
  previous = reading;
  delay(1);
}

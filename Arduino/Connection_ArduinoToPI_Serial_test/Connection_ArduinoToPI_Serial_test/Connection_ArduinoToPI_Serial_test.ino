#include <ChainableLED.h>

#define NUM_LEDS  1

ChainableLED leds(7, 8, NUM_LEDS);

char team = "BLAUW";
const int buttonPin = 2;     // the number of the pushbutton pin
//const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

byte red = 0;
byte green = 0;
byte blue = 0;

int teampie = 1;       // 1 = RED, 2 = BLUE

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  leds.init();
  //pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:

  if (buttonState == HIGH) {

    switch (teampie) {
      case 1:
        red = 0;
        blue = 255;
        teampie = 2;
                  leds.setColorRGB(0, red, green, blue);
        Serial.println("Button1: BLUE");
        break;
      case 2:
        red = 255;
        blue = 0;
        teampie = 1;
        leds.setColorRGB(0, red, green, blue);
        Serial.println("Button1: RED");
        break;


    }


  }

  delay(1000);




}

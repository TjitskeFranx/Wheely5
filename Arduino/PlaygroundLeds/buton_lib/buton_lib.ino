#include <AceButton.h>
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>


/*
 * A demo of a simplest AceButton that has a visible effect. One button is
 * connected to the digital pin BUTTON_PIN. It uses the internal pull-up
 * resistor (INPUT_PULLUP). Pressing the button turns on the built-in LED.
 * Releasing the button turns off the LED.
 */


using namespace ace_button;

// Some ESP32 boards have multiple builtin LEDs so don't define LED_BUILTIN.
#if defined(ESP32)
  const int LED_PIN = 2;
#else
  const int LED_PIN = LED_BUILTIN;
#endif

const int BUTTON_PIN = 2;

AceButton button(BUTTON_PIN);

void handleEvent(AceButton*, uint8_t, uint8_t); //dafuq is this?

void setup() {
  delay(1000); //is his needed?
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
}

void loop() {
  button.check();

}

void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("pressed");
      break;
    case AceButton::kEventReleased:
      Serial.println("released");
      break;
  }
}

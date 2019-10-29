//*
// * A demo of an array of 6 AceButton objects controlling an array of 6 LEDs. If
// * you have more than about 8 buttons or so, it might be worth using a
// * column/row button scanner instead. (Unfortunately AceButton does not support
// * that right now, maybe in the future).
// *
// * WARNING: This has been verified to compile, but I have not tested it with
// * actual hardware.
// */

#include <AceButton.h>
using namespace ace_button;

// LED states. Some microcontrollers wire their built-in LED the reverse.
const int LED_ON = HIGH;
const int LED_OFF = LOW;

// Number of buttons and LEDs.
const uint8_t NUM_TILES = 5;

// Helper struct that keeps track of the buttons and leds. An alternative is to
// use multiple arrays (e.g. BUTTON_PINS, LED_PINS, LED_STATES) which has the
// potential of reduing memory using PROGMEM, but for small to medium number of
// buttons, using a struct makes the program easier to read.
struct Info {
  const uint8_t buttonPin;
  bool ledState;
};

// An array of button pins, led pins, and the led states. Cannot be const
// because ledState is mutable.
Info INFOS[NUM_TILES] = { 
  {2, LED_OFF}, //0
  {3, LED_OFF}, //1
  {4, LED_OFF}, //2
  {5, LED_OFF}, //3
  {6, LED_OFF}, //4
};

// Define the buttons in an array using the default constructor. A static
// initializer array using { {int, int, int}, ...} does not seem to work,
// probably because of the 'explicit' keyword. So use the init() method in
// setup() instead. This has the disadvantage of invoking the default
// constructor NUM_TILES times, but this is no worse than explicitly declaring
// NUM_TILES instances of AceButton.
AceButton buttons[NUM_TILES];

void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(9600);
  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println(F("setup(): begin"));

  for (uint8_t i = 0; i < NUM_TILES; i++) {
    pinMode(INFOS[i].buttonPin, INPUT_PULLUP);

    // initialize the corresponding AceButton
    buttons[i].init(INFOS[i].buttonPin, HIGH, i); //dafug? 
  }

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  //is this needed?
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);

  Serial.println(F("setup(): ready"));
}



void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.//NO DELAYS
  for (uint8_t i = 0; i < NUM_TILES; i++) {
    buttons[i].check();
  }
}



// The event handler for the button.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events.
//  Serial.print(F("handleEvent(): eventType: "));
//  Serial.print(eventType);
//  Serial.print(F("; buttonState: "));
//  Serial.println(buttonState);

  // Get the button
  uint8_t id = button->getId();

  // Control the LED only for the Pressed and Released events.
  // Notice that if the MCU is rebooted while the button is pressed down, no
  // event is triggered and the LED remains off
  
  switch (eventType) { //?? maybe this can be different?
    case AceButton::kEventPressed:
        Serial.print("pressed");
        Serial.println(id); //get the id of the button
//      digitalWrite(ledPin, LED_ON);
//      INFOS[id].ledState = LED_ON;
      break;
    case AceButton::kEventReleased:
        Serial.print("released");
        Serial.println(INFOS[id].buttonPin); //get the pin
      break;
  }
}

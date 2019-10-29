

//latest try had very trippy results. ring responded blinking but seemed to crash the rest.. inconsistenly...
//latest version that worked is called ''_fixed'

//IS BECAUSE IT HAS BECOME SUPER SUPER DUPER SLOW.

//doesnt display led?
//when neutral > blue, does not fade becausee countdown is 0, fair


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <AceButton.h>
using namespace ace_button;
#include <ChainableLED.h>

#define LED_PIN    6
#define PIXEL_COUNT 16

// Number of buttons and LEDs.
const uint8_t NUM_TILES = 4;
const int RingId = NUM_TILES-1;
int generalCooldown = 100;
boolean WheelyInRange = false;
boolean wheelyFreeze = false;

ChainableLED leds(2, 3, NUM_TILES); //in pins 8+9
Adafruit_NeoPixel strip(PIXEL_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

struct Info {
  const uint8_t buttonPin;
  int cooldown;
  int timeAfterClick;
  int tileTeam;
  float lightI; //0-255
};

Info INFOS[NUM_TILES] = {
  {A0, generalCooldown, generalCooldown, 0, 0}, //id=0
  {A2, generalCooldown, generalCooldown, 0, 0}, //buttonpin, cooldown, timeAfterClick, teamTile, lightI
  {A4, generalCooldown, generalCooldown, 0, 0},
  {A6, generalCooldown, generalCooldown, 0, 0},
};
//WARNING: LED COLOURS ONLY UPDATED AT CHANGE

AceButton buttons[NUM_TILES];

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(115200);

  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println("setup(): begin");

  //buttonlib part
  for (uint8_t id = 0; id < NUM_TILES; id++) {

    // Button uses the built-in pull up register.
    pinMode(INFOS[id].buttonPin, INPUT_PULLUP);

    buttons[id].init(INFOS[id].buttonPin, LOW, id); //dafug?
    leds.setColorRGB(id, 0, 0, 0);
  }

  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);

  //adafruit part
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100);

  updateRing(1, 255);
  delay(500);

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.//NO DELAYS
  
  for (uint8_t id = 0; id < NUM_TILES; id++) {
    buttons[id].check();

    displayLight(id, INFOS[id].tileTeam, INFOS[id].lightI);

    if (INFOS[id].timeAfterClick < INFOS[id].cooldown) {
      INFOS[id].timeAfterClick++;
      INFOS[id].lightI = calculateBrightness(INFOS[id].timeAfterClick, INFOS[id].cooldown); //CALL

      //if ready
      if (INFOS[id].timeAfterClick == INFOS[id].cooldown) {
        Serial.println("READY button:");
        Serial.print(id);
        Serial.println("");
        INFOS[id].lightI = 255;
      }
    }
  }

  updateRing(INFOS[RingId].tileTeam, INFOS[RingId].lightI);
  
} //end of loop



void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) { //only called when happens
  // The event handler for the button.
 
  uint8_t id = button->getId();

  if (eventType == AceButton::kEventPressed) { //if pressed
    Serial.println("click from ");
    Serial.print(id);
    if (INFOS[id].timeAfterClick == INFOS[id].cooldown) { //if passed
        Serial.print(" -->  OK");
        INFOS[id].timeAfterClick = 0; //reset cooldown
  
        //WHERE THE MAGIC HAPPENS
        switch (INFOS[id].tileTeam) {
          case 1: //red
            INFOS[id].tileTeam = 2; //red>blue
            INFOS[id].cooldown = 100;
            break;
          case 2: //blue
            INFOS[id].tileTeam = 1; //blue >red
            INFOS[id].cooldown = 100;
            break;
          case 0: //neutral
            INFOS[id].tileTeam = 2; //neutral> ?(blue)
            INFOS[id].cooldown = 1;
            break;
        } //switches
  
       if(id=NUM_TILES-1 && WheelyInRange){
          INFOS[NUM_TILES-1].cooldown=500;
          wheelyFreeze=true;
       }
    else{
      Serial.print(" -->  X");
      Serial.println(" ");
      }
    }
  }
}

void displayLight(int id, int team, float lightI) {
  switch (team) { //for all model LED's
    case 1: //red
      leds.setColorRGB(id, lightI, 0, 0);
      break;
    case 2: //blue
      leds.setColorRGB(id, 0, 0, lightI);
      break;
    case 0: //neutral
      leds.setColorRGB(id, 0, 0, 0);
      break;
  }

}
void updateRing(int team, uint32_t lightI){ //CALLED 16 TIMES A STEP
  for(int i=0; i<PIXEL_COUNT; i++) { // For each pixel in strip...
    switch (team){
        case 1: //red
            strip.setPixelColor(i, strip.Color(lightI,   0,   0)); //no worky
//           Serial.println("I should be red right now");
        break;
        case 2: //blue
           strip.setPixelColor(i, strip.Color(0,   0,   lightI)); //worky 
        break;
        case 0: //neutral
          strip.setPixelColor(i, (0,0,0));
        break;
    }
    strip.show();                          //  Update strip to match
  }
}


float calculateBrightness(int isteps, int icooldown) { //isteps will allways be 0 or 100?
  //  float b=map(isteps,0,icooldown,0,2*PI); //from
  //  float c=map(sin(b),-1,1, 100,255);
  int a = isteps % 10;
  int b = map(a, 0, 20, 0, 255);
  return b;
}

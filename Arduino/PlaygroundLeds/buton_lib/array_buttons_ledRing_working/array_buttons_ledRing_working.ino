//WHATCH OUT CODE DOES NOT GET TOO MANY STEPS OR BUTTON LIB WILL FAIL
// LED RING FIXED!

//NOW ITS WHEELY TIME.
//wheely freeze should be subset of info: am i frozen or not? only ever freeze last one?
//freezing should be 'prettier''? just slower?

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
boolean wheelyInRange = true;

ChainableLED leds(2, 3, NUM_TILES); //in pins 8+9
Adafruit_NeoPixel strip(PIXEL_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

struct Info {
  const uint8_t buttonPin;
  int cooldown;
  int timeAfterClick;
  int tileTeam;
  float lightI; //0-255
  boolean wheelyPower;
};

Info INFOS[NUM_TILES] = {
  {A0, generalCooldown, generalCooldown, 0, 0, false}, //id=0
  {A2, generalCooldown, generalCooldown, 0, 0, false}, //buttonpin, cooldown, timeAfterClick, teamTile, lightI
  {A4, generalCooldown, generalCooldown, 0, 0, false},
  {A6, generalCooldown, generalCooldown, 0, 0, false},
};
//WARNING: LED COLOURS ONLY UPDATED AT CHANGE

AceButton buttons[NUM_TILES];

void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(9600);

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

  updateRing(1, 255, true);
  delay(500);

  Serial.println(F("setup(): ready"));
}

void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.//NO DELAYS
  
  for (uint8_t id = 0; id < NUM_TILES; id++) {
    buttons[id].check();

    displayLight(id, INFOS[id].tileTeam, INFOS[id].lightI, INFOS[id].wheelyPower);

    if (INFOS[id].timeAfterClick < INFOS[id].cooldown) {
      INFOS[id].timeAfterClick++;
      INFOS[id].lightI = calculateBrightness(INFOS[id].timeAfterClick, INFOS[id].cooldown, INFOS[id].wheelyPower); //CALL

      //if ready
      if (INFOS[id].timeAfterClick == INFOS[id].cooldown) {
        Serial.println("READY button:");
        Serial.print(id);
        Serial.println("");
        INFOS[id].lightI = 255;
        if (id==RingId){
          INFOS[RingId].wheelyPower=false;
          INFOS[RingId].cooldown=generalCooldown=INFOS[RingId].timeAfterClick;
        }
      }
    }
  }

  updateRing(INFOS[RingId].tileTeam, INFOS[RingId].lightI, INFOS[RingId].wheelyPower);
  
} //end of loop



void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) { //only called when happens
  // The event handler for the button.
 
  uint8_t id = button->getId();

  if (eventType == AceButton::kEventPressed) { //if pressed
    Serial.println("click from ");
    Serial.println(id);
    if (INFOS[id].timeAfterClick == INFOS[id].cooldown) { //if passed
        Serial.print("OK");
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

      if(id==2){restartGame();} //for TESTING
        
  
       if(id==RingId && wheelyInRange==true){
          INFOS[RingId].cooldown=300;
          INFOS[RingId].wheelyPower=true;
          Serial.println("power activated: ");
          Serial.println(RingId);
       }

    }
  }
}

void displayLight(int id, int team, float lightI, boolean WP) {
  switch (team) { //for all model LED's
    case 1: //red
      leds.setColorRGB(id, lightI, WP*50 , 0);
      break;
    case 2: //blue
      leds.setColorRGB(id, 0, WP*50, lightI);
      break;
    case 0: //neutral
      leds.setColorRGB(id, 0, 0, 0);
      break;
  }

}
void updateRing(int team, uint32_t lightI, boolean WP){ //CALLED 16 TIMES A STEP
  for(int i=0; i<PIXEL_COUNT; i++) { // For each pixel in strip...
    switch (team){
        case 1: //red
            strip.setPixelColor(i, strip.Color(lightI,   WP*50,   0)); //no worky
//           Serial.println("I should be red right now");
        break;
        case 2: //blue
           strip.setPixelColor(i, strip.Color(0,   WP*50,   lightI)); //worky 
        break;
        case 0: //neutral
          strip.setPixelColor(i, (0,0,0));
        break;
    }
    strip.show();                          //  Update strip to match
  }
}

void restartGame(){
   for (uint8_t id = 0; id < NUM_TILES; id++) {
      INFOS[id].tileTeam = 0;
      INFOS[id].cooldown = generalCooldown;
      INFOS[id].timeAfterClick = generalCooldown;
      INFOS[id].wheelyPower=false;
      wheelyInRange!=wheelyInRange;
   }
   Serial.println("Full Game restart");
}

float calculateBrightness(int isteps, int icooldown, boolean WP) { //isteps will allways be 0 or 100?
  //  float b=map(isteps,0,icooldown,0,2*PI); //from
  //  float c=map(sin(b),-1,1, 100,255);
  int a = isteps % (10 + WP*10); //if wheelyPower then make it blink slower
  int b = map(a, 0, 20, 0, 255);
  return b;
}

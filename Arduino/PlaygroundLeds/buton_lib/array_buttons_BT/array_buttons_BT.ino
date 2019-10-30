//WORKS!

//NOW ITS WHEELY TIME.
//wheely freeze should be subset of info: am i frozen or not? only ever freeze last one?
//freezing should be 'prettier''? just slower?

//hardware pins
//BT module
//--< EN: 5V
//--<VCC: 3.3V
//--<GND: ground
//-->TXT: 10;
//-->RX:11;

//SO LIB SOFTWARE STUFF NTO SUPPORTED BY DUE BECAUSE BAUTRAUT
//ALSO NO OTHER DETECTION THAN TJISKI PHONE ON HER CODE?


//INITS
//____________________________________________________________________________


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <AceButton.h>
using namespace ace_button;
#include <ChainableLED.h>
#include <SoftwareSerial.h>

SoftwareSerial BTserial(10, 11); // RX | TX

#define LED_PIN    6
#define PIXEL_COUNT 16

//BT communication
char macAdressDevice = "64B0:A6:A09F78";
char BTchar = ' '; //data that starts out empty
int signalStrenght;
String foundDeviceString = String ();
String signalStrenghtHex = String ();
int BTSearchTime = 10000;
int BTCooldown = 0;
int BTTime =0;
bool BTCheck = false;


//--------FUCTIONS-------------
//--------------BT communication
void setupBTSerial();
void checkBT();
void CheckBTTime();
int getSignalStrenght(String adress);


//--------------buttons and lights
void checkCooldown(int id);
void updateRing(int team, uint32_t lightI, boolean WP);
void displayLight(int id, int team, float lightI, boolean WP);
float calculateBrightness(int isteps, int icooldown, boolean WP);
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);
void restartGame();


// Number of buttons and LEDs.
const uint8_t NUM_TILES = 4;
const int RingId = NUM_TILES - 1;
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


AceButton buttons[NUM_TILES];

//_______________________________________________________________________________________


void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(38400);
  setupBTSerial();

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

  Serial.println("setup(): ready");
}

//------------------------------------------------------------------------


void loop() {
  // Should be called every 4-5ms or faster, for the default debouncing time
  // of ~20ms.//NO DELAYS
//  btDone = false;

  if(BTCheck){
    checkBT();
    CheckBTTime();
  }
  else{
    for (int id = 0; id < NUM_TILES; id++) {
      buttons[id].check();
      checkCooldown(id);
      displayLight(id, INFOS[id].tileTeam, INFOS[id].lightI, INFOS[id].wheelyPower);
      updateRing(INFOS[RingId].tileTeam, INFOS[RingId].lightI, INFOS[RingId].wheelyPower);

    }
  }

} //end of loop

//------------------------------------------------------------------------






//ALL LED/BUTTON RELATED FUNCTIONS
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) { //only called when happens
  // The event handler for the button.

  uint8_t id = button->getId();

  if (eventType == AceButton::kEventPressed) { //if PRESSED
    Serial.println("click from ");
    Serial.println(id);

    if (INFOS[id].timeAfterClick == INFOS[id].cooldown) { //if PASSED
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

      if (id == 2) {
        restartGame(); //for TESTING
      }
      else if(id==RingId){
        BTserial.println("AT+INQ");
        Serial.println("AT command send");
        for (int i = 0; i < PIXEL_COUNT; i++) { // For each pixel in strip...
          strip.setPixelColor(i, strip.Color(255,255,255));
          strip.show(); 
        }
        BTCooldown=5000;
        BTCheck=true;

//        INFOS[RingId].cooldown = 300;
//        INFOS[RingId].wheelyPower = true;
//        Serial.println("power activated: ");
//        Serial.println(RingId);
      }
      //      checkBTTime=2000;
      

    } //end if passed
  }
}

void checkCooldown(int id) { //handles cooldown system
  if (INFOS[id].timeAfterClick < INFOS[id].cooldown) {
    INFOS[id].timeAfterClick++;
    INFOS[id].lightI = calculateBrightness(INFOS[id].timeAfterClick, INFOS[id].cooldown, INFOS[id].wheelyPower);

    //if ready
    if (INFOS[id].timeAfterClick == INFOS[id].cooldown) {
      Serial.println("READY button:");
      Serial.print(id);
      Serial.println("");
      INFOS[id].lightI = 255;
      if (id == RingId) {
        INFOS[RingId].wheelyPower = false;
        INFOS[RingId].cooldown = generalCooldown = INFOS[RingId].timeAfterClick;
      }
    }
  }
}

void displayLight(int id, int team, float lightI, boolean WP) {
  switch (team) { //for all model LED's
    case 1: //red
      leds.setColorRGB(id, lightI, WP * 50 , 0);
      break;
    case 2: //blue
      leds.setColorRGB(id, 0, WP * 50, lightI);
      break;
    case 0: //neutral
      leds.setColorRGB(id, 0, 0, 0);
      break;
  }
}

void updateRing(int team, uint32_t lightI, boolean WP) { //CALLED 16 TIMES A STEP
  for (int i = 0; i < PIXEL_COUNT; i++) { // For each pixel in strip...
    switch (team) {
      case 1: //red
        strip.setPixelColor(i, strip.Color(lightI,   WP * 50,   0)); //no worky
        //           Serial.println("I should be red right now");
        break;
      case 2: //blue
        strip.setPixelColor(i, strip.Color(0,   WP * 50,   lightI)); //worky
        break;
      case 0: //neutral
        strip.setPixelColor(i, (0, 0, 0));
        break;
    }
    strip.show();                          //  Update strip to match
  }
}

void restartGame() {
  for (uint8_t id = 0; id < NUM_TILES; id++) {
    INFOS[id].tileTeam = 0;
    INFOS[id].cooldown = generalCooldown;
    INFOS[id].timeAfterClick = generalCooldown;
    INFOS[id].wheelyPower = false;
    wheelyInRange != wheelyInRange;
  }
  Serial.println("Full Game restart");
}

float calculateBrightness(int isteps, int icooldown, boolean WP) { //isteps will allways be 0 or 100?
  //  float b=map(isteps,0,icooldown,0,2*PI); //from
  //  float c=map(sin(b),-1,1, 100,255);
  int a = isteps % (10 + WP * 10); //if wheelyPower then make it blink slower
  int b = map(a, 0, 20, 0, 255);
  return b;
}


//ALL BT RELATED FUNCTIONS
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//void checkBTControlled() {
//  
//  if (checkBTTime == 1) {
//    Serial.println("BT searching time done");
//    checkBTTime = 0;
//  }
//  else if (checkBTTime > 0) {
//    checkBT();
//    checkBTTime--;
//  }
//}

void CheckBTTime(){ 
    
    if(millis()%10==0){
      BTCooldown--;
    }
    if(BTCooldown==0){
      Serial.println("BT Done checking");
      BTCheck=false;
      }
}

void checkBT() {

  if (Serial.available()) //if arduino send something > send to BT
  {
    BTchar = Serial.read();
    BTserial.write(BTchar);
  }
  if (BTserial.available()) //if BT send soemthing > send to Arduino
  {
    BTchar = BTserial.read();
    Serial.write(BTchar);
  

    // -- Code for assembling found strings of found MAC addresses---
    if (BTchar == '\n' || BTchar == '+')
    {
      foundDeviceString = ""; //resets?
    }
    if (BTchar != '\n' ) {
      foundDeviceString = foundDeviceString + BTchar;
    }
  
    //--- end string at 31because then its done
    if (foundDeviceString.length() == 31) {
      Serial.println(foundDeviceString);
  
      //WHEN RECOGNIZE
      if (foundDeviceString.startsWith("+INQ:64B0:A6:A09F78")) //when we recognize
      {
        Serial.println("FOUND WHEELY");
        signalStrenghtHex = foundDeviceString.substring(27); //starts at 27, reads last 4 characters which are signalstrengs
        signalStrenght = getSignalStrenght(signalStrenghtHex);
        Serial.println(signalStrenght);
  
        if (signalStrenght >= 0) {
          Serial.println("STRONG ENOUGH");
          Serial.println("SUCCES");
          INFOS[RingId].cooldown=300;
          INFOS[RingId].wheelyPower=true;
          BTCheck=false;
        }
        else {
          Serial.println("---> FAIL");
        }
      }
      else {
        Serial.println("NO wheely");
      }
    }//end of line to check
  }//if avalible data

//  btDone = true;
}

int getSignalStrenght(String adress) {
  unsigned int strenght = 0;
  int nextInt; //?

  for (int i = 0; i < adress.length(); i++) { //magic

    nextInt = int(adress.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    strenght = (strenght * 16) + nextInt;
  }
  return strenght + 50;
}

void setupBTSerial() {
  BTserial.begin(38400);
  BTserial.println("AT");
  Serial.println("10%--");
  delay(100);
  BTserial.println("AT+ORGL");
  Serial.println("24%---");
  delay(100);
  BTserial.println("AT+INIT");
  Serial.println("48%-----");
  delay(100);
  BTserial.println("AT+ROLE=1");
  Serial.println("73%---------");
  delay(100);
  BTserial.println("AT+INQM=1,8,5");
  Serial.println("95%-------------");
  delay(100);
  Serial.println("100%--------------");
  Serial.println("DONE");

  Serial.println("searching initialized");
  BTserial.println("AT+INQ");
}

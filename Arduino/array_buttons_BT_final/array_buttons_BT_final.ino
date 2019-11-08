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

#define LED_PIN    6  //for ledring
#define PIXEL_COUNT 16 //for ledring
#define RingId 4
#define RestartId 5
#define generalCooldown 50
#define macAdressDevice = "64B0:A6:A09F78";

//BT communication
char BTchar = ' '; //data that starts out empty
int signalStrenght;
String foundDeviceString = String ();
String signalStrenghtHex = String ();
int BTSearchTime = 10000;
int BTCooldown = 0;
int BTTime =0;
bool BTCheck = false;

//other
int redScore=3; //just initial: should be random to be honest
int blueScore=2;


//--------FUCTIONS-------------
//--------------BT communication
void setupBTSerial();
void checkBT();
void CheckBTTime();
int getSignalStrenght(String adress);

//--------------buttons and lights
//ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
//void setClickDelay(uint16_t clickDelay); // default 20 > set to 10
void checkCooldown(int id);
void updateRing(int team, uint32_t lightI, boolean WP);
void displayLight(int id, int team, float lightI, boolean WP);
float calculateBrightness(int isteps, int icooldown, boolean WP);
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);
void restartGame();


// Number of buttons and LEDs.
const uint8_t NUM_TILES = 6; //4 small ones, 1 big one, 1 virtual one to restart
boolean wheelyInRange = true;

ChainableLED leds(2, 3, NUM_TILES); //have to be equal to number of  buttons even though one less
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
  {A2, generalCooldown, generalCooldown, 1, 255, false}, //id=0
  {A4, generalCooldown, generalCooldown, 1, 255, false}, //buttonpin, cooldown, timeAfterClick, teamTile, lightI, Wheelypower
  {A6, generalCooldown, generalCooldown, 2, 255, false},
  {A8, generalCooldown, generalCooldown, 2, 255, false},
  {A0, generalCooldown, generalCooldown, 1, 255, false}, //LedRing, ''real-size Tile'
  {A12, 10, 10, 0, 0, false},                            //Restart button, no real tile

AceButton buttons[NUM_TILES]; //create the button objects, called 'buttons'

//_______________________________________________________________________________________


void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(38400);
//  setClickDelay(20);

//  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println("setup(): begin");

  setupBTSerial();
  
  
  //buttonlib part
  for (uint8_t id = 0; id < NUM_TILES; id++) {

    // Button uses the built-in pull up register.
    pinMode(INFOS[id].buttonPin, INPUT_PULLUP);

    buttons[id].init(INFOS[id].buttonPin, LOW, id); //dafug?
    leds.setColorRGB(id, 0, 0, 0);
  }
  
    //initiate the event handler so we can call event ''button clicked'' later
    ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
    buttonConfig->setEventHandler(handleEvent);
    buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  
  
    //initialize the LedRing
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(100); 
    Serial.println("setup(): ready");
  }
}

//------------------------------------------------------------------------


void loop() {

  if(BTCheck){    //checks EITHER BT or updates Leds. system cannot do both at same time.
    checkBT();
    CheckBTTime();
  }
  else{
    
    for (int id = 0; id < NUM_TILES; id++) {
      //functions that are checked every step
      buttons[id].check();
      checkCooldown(id);
      
      //heavy functions that are checked every 2 steps because otherwhise the system gets to slow
      if(millis()%2==0){
        displayLight(id, INFOS[id].tileTeam, INFOS[id].lightI, INFOS[id].wheelyPower);
        updateRing(INFOS[RingId].tileTeam, INFOS[RingId].lightI, INFOS[RingId].wheelyPower);
      }

    }
  }

} //end of loop

//------------------------------------------------------------------------






//ALL LED/BUTTON RELATED FUNCTIONS
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) { 

  uint8_t id = button->getId(); //which button is pressed?

  if (eventType == AceButton::kEventPressed) { //if button PRESSED

    if (INFOS[id].timeAfterClick == INFOS[id].cooldown) { //if button not blocked
      INFOS[id].timeAfterClick = 0; //reset cooldown timer


      //what happens when?
      switch (INFOS[id].tileTeam) {
        case 1: //red team
          INFOS[id].tileTeam = 2; //goes from red to blue
          blueScore++;
          redScore--;
          INFOS[id].cooldown = generalCooldown; //reset cooldown again
          break;
        case 2: //blue
          INFOS[id].tileTeam = 1; //goes from blue to red
          blueScore--;
          redScore++;
          INFOS[id].cooldown = generalCooldown;
          break;
        case 0: //neutral
          INFOS[id].tileTeam = 2; //goes from neutral/off to blue.. 
          INFOS[id].cooldown = 1; //...but allows for clicking again straight away to make it red again
          blueScore++;
          communicateScore();
          break;
      } //switches
      communicateScore();
      
      if (id == RestartId) { //restart when restart button is pressed
        restartGame(); 
      }
      else if(id==RingId){  //when the button of the real-size tile is pressed
        BTserial.println("AT+INQ"); //send search protocol
        Serial.println("searching...");
        BTCooldown=5000; //searches for 5 seconds
        BTCheck=true;
      }
      
    } 
  }
}

void checkCooldown(int id) { //handles cooldown system
  if (INFOS[id].timeAfterClick < INFOS[id].cooldown) {
    INFOS[id].timeAfterClick++;
    
    //the brightness of the leds is a function of how long the buttons have been blocked
    INFOS[id].lightI = calculateBrightness(INFOS[id].timeAfterClick, INFOS[id].cooldown, INFOS[id].wheelyPower);

    //if ready
    if (INFOS[id].timeAfterClick == INFOS[id].cooldown) { //when the timer runs out> unblock the button
      Serial.println("READY button:");
      Serial.print(id);
      Serial.println("");
      INFOS[id].lightI = 255; //...and make the leds brightly lit again.
      if (id == RingId) {
        INFOS[RingId].wheelyPower = false;
        INFOS[RingId].cooldown = INFOS[RingId].timeAfterClick = generalCooldown; //reset 'frozen tile' also.
      }
    }
  }
}

void displayLight(int id, int team, float lightI, boolean WP) {  //WP stands for wheely power
  switch (team) { //for all model LED's
    case 1: //red
      leds.setColorRGB(id,  lightI, (WP *(lightI/8)), 0); //if WheelyPower = on, make light slightly different
      break;
    case 2: //blue
      leds.setColorRGB(id,  0, (WP *(lightI/4)), lightI);
      break;
    case 0: //neutral
      leds.setColorRGB(id, 0, 0, 0);
      break;
  }
}

void updateRing(int team, uint32_t lightI, boolean WP) { 
  for (int i = 0; i < PIXEL_COUNT; i++) { // For each pixel in strip...
    switch (team) {
      case 1: //red
        strip.setPixelColor(i, strip.Color(lightI,   (WP *(lightI/8)), 0)); 
        break;
      case 2: //blue
        strip.setPixelColor(i, strip.Color(0,   (WP *(lightI/4)),   lightI)); 
        break;
      case 0: //neutral
        strip.setPixelColor(i, (0, 0, 0));
        break;
    }
    strip.show();  //strip needs to be updated in order to show changes
  }
}

void restartGame() {
  for (uint8_t id = 0; id < NUM_TILES; id++) { //in this case, every team or power is reset to default
    INFOS[id].tileTeam = 0;
    INFOS[id].cooldown = generalCooldown;
    INFOS[id].timeAfterClick = generalCooldown;
    INFOS[id].wheelyPower = false;
  }
  Serial.println("Full Game restart");
}

float calculateBrightness(int isteps, int icooldown, boolean WP) { 
  //converts TimeAfterClick to a Brichtness function
  int a = isteps % (5 + WP * 5); //make it blink slower when Tile is Frozen (if WP=true)
  int b = map(a, 0, 20, 0, 255); //brightness should be between 0-255
  return b;
}

void communicateScore(){
  Serial.println("redscore " + String(redScore));
  Serial.println("bluescore " + String(blueScore));
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
  BTserial.println("AT+INQM=1,5,5");
  Serial.println("95%-------------");
  delay(100);
  Serial.println("100%--------------");
  Serial.println("DONE");

  Serial.println("searching initialized");
  BTserial.println("AT+INQ");
}

//WORKS!

//NOW ITS WHEELY TIME.
//wheely freeze should be subset of info: am i frozen or not? only ever freeze last one?
//freezing should be 'prettier''? just slower?

//hardware pins
//BT module
//--< EN: 5V
//--<VCC: 3.3V
//--<GND: ground
//-->TXT: 18;
//-->RX: 19;

//Button pin attached to pin 2

//INITS
//____________________________________________________________________________


#include <SoftwareSerial.h>

SoftwareSerial BTserial(18, 19); // RX | TX
char macAdressDevice = "64B0:A6:A09F78"; //enter MAC address of your used beacon here!

//BT communication
char BTchar = ' '; //data that starts out empty
int signalStrenght;
String foundDeviceString = String ();
String signalStrenghtHex = String ();



//--------FUCTIONS-------------
//--------------BT communication
void setupBTSerial();
void checkBT();
void CheckBTTime();
int getSignalStrenght(String adress);

//--------------Button communication
int buttonPin = 2;
int ButtonState = 0;


//_______________________________________________________________________________________


void setup() {
  delay(1000); // some microcontrollers reboot twice
  Serial.begin(38400);
  //  setClickDelay(20);

  //  while (! Serial); // Wait until Serial is ready - Leonardo/Micro
  Serial.println("setup(): begin");

  setupBTSerial();

  // Button uses the built-in pull up register.
  pinMode(2, INPUT);

  Serial.println("setup(): ready");
}

//------------------------------------------------------------------------


void loop() {

  ButtonState = digitalRead(buttonPin);

  if (ButtonState == HIGH) { //when button is pressed
    BTserial.println("AT+INQ");
    Serial.println("searching...");
    delay(1000); // wait 1 second
  }

  if (Serial.available()) //if arduino send something > send to BT
  {
    BTchar = Serial.read();
    BTserial.write(BTchar);
  }


  if (BTserial.available()) { //if BT send soemthing > send to Arduino

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
      if (foundDeviceString.startsWith("+INQ:" + macAdressDevice)) { //when we recognize
        Serial.println("FOUND WHEELY");
        signalStrenghtHex = foundDeviceString.substring(27); //starts at 27, reads last 4 characters which are signalstrengs
        signalStrenght = getSignalStrenght(signalStrenghtHex);
        Serial.println(signalStrenght);

        if (signalStrenght >= 0) {
          Serial.println("STRONG ENOUGH");
          Serial.println("SUCCES");
        }
        else {
          Serial.println("---> FAIL");
        }

      }
      else {
        Serial.println("NO wheely");
      }
    }
  }
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
  BTserial.println("AT+INQM=1,10,5");
  Serial.println("95%-------------");
  delay(100);
  Serial.println("100%--------------");
  Serial.println("DONE");

  Serial.println("searching initialized");
  BTserial.println("AT+INQ");
}


int getSignalStrenght(String adress) {
  unsigned int strenght = 0;
  int nextInt;

  for (int i = 0; i < adress.length(); i++) {
    nextInt = int(adress.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    strenght = (strenght * 16) + nextInt;
  }
  return strenght + 50;
}

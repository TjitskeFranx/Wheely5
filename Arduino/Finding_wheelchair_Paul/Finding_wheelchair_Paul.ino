// Basic Bluetooth sketch HC-05_03 Using the state pin
// Connect the HC-05 module and communicate using the serial monitor
//
// The HC-05 defaults to communication mode when first powered on.
// The default baud rate for communication mode is 9600
//

//hardware pins
//BT module
//--< EN: 5V
//--<VCC: 3.3V
//--<GND: ground
//-->TXT: 10;
//-->RX:11;

#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX.
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// Connect the HC-05 STATE pin to Arduino pin 4.



//char macAddressDevice = "64B0:A6:A09F78"; //Tjiskephone
//"CCC0:79:4E788A" //PAUL PHONE
//"a0a8:cd:3b285b" //Paul Laptop
char macAdressDevice = "64B0:A6:A09F78";
char BTchar= ' '; //data that starts out empty
int signalStrenght;
String foundDeviceString = String ();
String signalStrenghtHex = String ();
char foundDeviceStringString;

int checkBTTimer =0;

//const byte BTpin = 9;
const int buttonPin = 4;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status



void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  Serial.println("Arduino is ready");
  Serial.println("Connect the HC-05 to an Android device to continue");  
  setupBTSerial();
}

void loop()
{

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
  {
    Serial.println("searching initialized");
    BTserial.println("AT+INQ");
    delay(1000);
  }

  if (checkBTTimer<50000){
      checkBT();
  }

    else if(checkBTTimer==50000){
    Serial.println("stop checking");
    
  }
  
  else if(checkBTTimer==100000){
    Serial.println("restart+sendATcommand");
    Serial.println("checkfor 1000");
    BTserial.println("AT+INQ"); 
    checkBTTimer=0;
  }
//  Serial.println(checkBTTimer);
  checkBTTimer++;
}




//ALL BT RELATED FUNCTIONS
//------------------------------------------------------------------------
//------------------------------------------------------------------------
void checkBT(){
  if (Serial.available()) //if arduino send something > send to BT
    {
     BTchar= Serial.read();
     BTserial.write(BTchar);
    }
  if (BTserial.available()) //if BT send soemthing > send to Arduino
    {
      BTchar= BTserial.read();
      Serial.write(BTchar);
  
  
  // -- Code for assembling found strings of found MAC addresses---
  if (BTchar == '\n' || BTchar == '+')
      {
        foundDeviceString = ""; //resets?
      }
  if (BTchar != '\n' ){
      foundDeviceString = foundDeviceString + BTchar;
    }


 //--- end string at 31because then its done
    if (foundDeviceString.length() == 31) {
      Serial.println(foundDeviceString + " ---> READ");


        //WHEN RECOGNIZE
      if (foundDeviceString.startsWith("+INQ:64B0:A6:A09F78")) //when we recognize
      {
        Serial.println("FOUND WHEELY");
        signalStrenghtHex = foundDeviceString.substring(27); //starts at 27, reads last 4 characters which are signalstrengs
        signalStrenght = getSignalStrenght(signalStrenghtHex);
        Serial.println(signalStrenght);
        
        if (signalStrenght >= 0){
          Serial.println("STRONG ENOUGH");
          Serial.println("SUCCES");
        }
        else{
          Serial.println("---> FAIL");
        }    
      }
      else {
        Serial.println("NO wheely");
      }
   }//end of line to check
  } //if no BT avalible
  
}


int getSignalStrenght(String adress){
      Serial.println(adress);
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
      return strenght+40;
}



void setupBTSerial(){
//
//  pinMode(BTpin, OUTPUT);
//  digitalWrite(BTpin, HIGH);
//
//  // initialize the pushbutton pin as an input:
//
//  while (!BTconnected) //YOU MAKE A PIN HIGH AND THEN CHECK FOR WHEN HE IS HIGH?
//  {
//    if ( digitalRead(BTpin) == HIGH)  { //wait untill BT pin is connected in startup
//      BTconnected = true;
//      Serial.println("BT is connected, of we go!  ");
//    };
//  }
  BTserial.begin(38400);
  BTserial.println("AT");
  Serial.println("10%--");
  delay(1000);
  BTserial.println("AT+ORGL");
  Serial.println("24%---");
  delay(1000);
  BTserial.println("AT+INIT");
  Serial.println("48%-----");
  delay(1000);
  BTserial.println("AT+ROLE=1");
  Serial.println("73%---------");
  delay(1000);
  BTserial.println("AT+INQM=1,6,10"); //1? 4devices 10 sec)?
  Serial.println("95%-------------");
  delay(1000);
  Serial.println("100%--------------");
  Serial.println("DONE");
}

// Basic Bluetooth sketch HC-05_03 Using the state pin
// Connect the HC-05 module and communicate using the serial monitor
//
// The HC-05 defaults to communication mode when first powered on.
// The default baud rate for communication mode is 9600
//

#include <SoftwareSerial.h>
SoftwareSerial BTserial(10, 11); // RX | TX
// Connect the HC-05 TX to Arduino pin 10 RX.
// Connect the HC-05 RX to Arduino pin 11 TX through a voltage divider.
// Connect the HC-05 STATE pin to Arduino pin 4.
//

int count = 0;
char macAddressDevice = "64B0:A6:A09F78";
char c = ' ';
char d = ' ' ;
char val;
int decValue;
String foundDevice = String ();
String hexString = String ();
char foundDeviceString;



// BTconnected will = false when not connected and true when connected
boolean BTconnected = false;

// connect the STATE pin to Arduino pin D4
const byte BTpin = 9;
const int buttonPin = 4;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status



void setup()
{
  // set the BTpin for input
  pinMode(BTpin, OUTPUT);
  digitalWrite(BTpin, HIGH);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // start serial communication with the serial monitor on the host computer
  Serial.begin(9600);
  Serial.println("Arduino is ready");
  Serial.println("Connect the HC-05 to an Android device to continue");

  // wait until the HC-05 has made a connection

  while (!BTconnected)
  {
    if ( digitalRead(BTpin) == HIGH)  {
      BTconnected = true;
    };
  }

  Serial.println("HC-05 is now connected");
  Serial.println("");

  // Start serial communication with the bluetooth module
  // HC-05 default serial speed for communication mode is 9600 but can be different

  BTserial.begin(38400);


  //  Serial.println("AT");
  BTserial.println("AT");
  delay(1000);
  //  Serial.println("AT+ORGL");
  BTserial.println("AT+ORGL");
  delay(1000);
  //  Serial.println("AT+INIT");
  BTserial.println("AT+INIT");
  delay(1000);
  //  Serial.println("AT+ROLE=1");
  BTserial.println("AT+ROLE=1");
  delay(1000);
  //  Serial.println("AT+INQM=1,2,10");
  BTserial.println("AT+INQM=1,4,5");
  delay(1000);
  //  Serial.println("AT+INQ");
  //  BTserial.println("AT+INQ");
  Serial.println("DONE");
}

void loop()
{

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);



  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH)
  {
    Serial.println("AT+INQ");
    BTserial.println("AT+INQ");
    delay(1000);
  }



  // Keep reading from Arduino Serial Monitor input field and send to HC-05
  if (Serial.available())
  {
    c = Serial.read();

    BTserial.write(c);

  }




  // Keep reading from the HC-05 and send to Arduino Serial Monitor
  if (BTserial.available())
  {
    c = BTserial.read();

    Serial.write(c);

    // -- Code for assembling found strings of found MAC addresses---

    if (c == '\n')
    {

      foundDevice = "";
      //Serial.println(foundDevice + "HOI");
    }

    if (c == '+') {
      foundDevice = "";
      //Serial.println(foundDevice + "heej");
    }

    if (c != '\n')
    {
      //Serial.println("hoi");
      foundDevice = foundDevice + c;
      //Serial.println(foundDevice + "hoi");
    }


    if (foundDevice.length() == 31) {
      Serial.println(foundDevice + " :That's a perfectly acceptable text message");
      if (foundDevice.startsWith("+INQ:64B0:A6:A09F78"))
      {
        Serial.println(foundDevice);
        hexString = foundDevice.substring(27);
        Serial.println(hexString);

        unsigned int decValue = 0;
        int nextInt;

        for (int i = 0; i < hexString.length(); i++) {

          nextInt = int(hexString.charAt(i));
          if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
          if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
          if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
          nextInt = constrain(nextInt, 0, 15);

          decValue = (decValue * 16) + nextInt;
        }


        Serial.println(decValue);

        if (decValue >= 65480 && decValue <= 65510) {
          Serial.println("I am a wheely!");

        }
        if (decValue < 65480 || decValue > 65510 )  {
          Serial.println("I am a foot!");
        }
      }
    }

    // -- end code finding strings MAC adresses


  }
}

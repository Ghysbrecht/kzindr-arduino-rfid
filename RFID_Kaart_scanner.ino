/* TITLE:
   99 BUGS HACKERSPACE RFID LOGIN

   DESCRIPTION:
   This program is made to be used in the 99bugs hackerspace @ VIVES Oostende. The members can send a checkin to the Kzindr app with the use of an RFID card.

   HARDWARE:
   - Arduino UNO/Arduino Nano
   - RFID-RC522 Card Scanner
   - Buzzer
   - Another Arduino with an Ethernet shield/module

   PIN CONFIG:
   RFID-RC522( SS -> 9, RST -> 8, MISO ->12, MOSI -> 13 )(SPI)
   Buzzer( Signal -> 5 )
   Handshaking( Signal -> 6 )

   NOTES:
   The UID is being sent as a 16 byte frame. It starts and end with a '%'.
*/

//Include statements
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

//Hardware
#define SOUND_PIN 5               //The pin used for the buzzer
#define HS_PIN 6                  //Handshaking
#define SS_PIN 9                  //Slave Select for SPI to the RFID
#define RST_PIN 8                 //Reset to the RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);  //Create MFRC522 instance.
SoftwareSerial mySerial(3, 2);    //RX, TX

//Variables
byte cardPresent;
String mainString;

//Setup
void setup() {
  Serial.begin(9600);    //Initialize serial communications with the PC
  mySerial.begin(9600);  //Init serial communication with the other arduino
  SPI.begin();           //Init SPI bus
  mfrc522.PCD_Init();    //Init MFRC522 card
  pinMode(HS_PIN, INPUT);
  pinMode(SOUND_PIN, OUTPUT);
}

void loop() {
  if (digitalRead(HS_PIN) == HIGH) {         //Is the POSTER ready to receive the ID?
    if (mfrc522.PICC_IsNewCardPresent()) {   //Is there a card?
      Serial.println("Card detected...");
      if (mfrc522.PICC_ReadCardSerial()) {   //Can I read the ID?
        Serial.println("Card ID read...");
        mainString = getCardId();            //Get the ID
        Serial.println(mainString);     
        sendUid(mainString);                 //Send the ID through the software serial
        playSound(500, 200);                 //Make a fun sound to say the card has been scannerd
        playSound(1000, 200);
        delay(2000);
      }
    }
  }
}

//Get the RFID card ID
String getCardId() {
  String tempString;
  for (byte i = 0; i < mfrc522.uid.size; i++) {                 //Do this for the size of the UID
    if (mfrc522.uid.uidByte[i] < 0x10) tempString.concat("0");
    tempString.concat(String(mfrc522.uid.uidByte[i], HEX));     //Concat all the bytes that make up the UID
  }
  tempString.toUpperCase();
  return tempString;
}

//Play a beep...
void playSound(int frequency, int delayTime) {
  tone(SOUND_PIN, frequency);
  delay(delayTime);
  noTone(SOUND_PIN);
  digitalWrite(HS_PIN, LOW);
}

//Send the UID
void sendUid(String tempString) {
  //A total of 16 bytes need to be send
  mySerial.print('%');                     //First byte
  if (mainString.length() < 14) {          //If the id is shorter than 14, puts some 0's in it
    short a = 14 - tempString.length();
    while (a > 0) {
      mySerial.print('0');
      a--;
    }
  }
  mySerial.print(tempString);
  mySerial.print('%');                    //16th byte
}



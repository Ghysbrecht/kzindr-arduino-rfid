/* TITLE:
 * 99 BUGS HACKERSPACE RFID LOGIN
 * 
 * DESCRIPTION:
 * This program is made to be used in the 99bugs hackerspace @ VIVES Oostende. The members can login on the Kzindr app with the use of a RFID card.
 * 
 * HARDWARE:
 * - Arduino UNO/Arduino Nano
 * - RFID-RC522 Card Scanner
 * - Buzzer
 * - .......
 * 
 * PIN CONFIG:
 * RFID-RC522( SS -> 10, RST -> 9, MISO ->12, MOSI -> 13 )(SPI)
 * Buzzer( Signal -> 8 )
 * 
 * NOTES:
 * mfrc522.uid.size
 */

//Include statements
#include <SPI.h>
#include <MFRC522.h>

//Hardware 
#define SOUND_PIN 8
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

//Variables
byte cardPresent;
String mainString;

//Setup
void setup() {
  Serial.begin(9600);  	// Initialize serial communications with the PC
  SPI.begin();		    	// Init SPI bus
  mfrc522.PCD_Init();	  // Init MFRC522 card
}

void loop() {
if(mfrc522.PICC_IsNewCardPresent()){     //Is there a card?
  Serial.println("Card detected...");
  if(mfrc522.PICC_ReadCardSerial()){     //Can I read the ID? 
    Serial.println("Card ID read...");
    Serial.println(getCardId());
    playSound(500, 200);
    playSound(1000, 200);
  }
}


     
}

//Get the RFID card ID
String getCardId(){
  String tempString;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     if(mfrc522.uid.uidByte[i] < 0x10) tempString.concat("0");
     tempString.concat(String(mfrc522.uid.uidByte[i], HEX));
  } 
  tempString.toUpperCase();
  return tempString;
}

//Play a beep...
void playSound(int frequency, int delayTime){
  tone(SOUND_PIN,frequency);
  delay(delayTime);
  noTone(SOUND_PIN);
}



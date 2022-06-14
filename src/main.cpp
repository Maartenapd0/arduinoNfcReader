// Programmeren Van Microcontrollers - K0730 | Eindopdracht
// Derrin Beekman & Maarten van Assenbergh

//Libaries importeren.
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

//Een nieuw NFC component aanmaken.
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
String uidString;
unsigned long previousMillis = 0; 

//Variabelen voor de button
unsigned long previousMillisButton = 0; 
int button = 2;

//Het LiquidCrystal LCD scherm component aanmaken.
LiquidCrystal_I2C lcd(0x27, 20, 4);

//4 classes om het gemakkelijker te maken om de printen op een regel op het LCD scherm.
void printLineOne(String text)
{
  lcd.setCursor(0, 0);
  lcd.print(text);
}
void printLineTwo(String text)
{
  lcd.setCursor(0, 1);
  lcd.print(text);
}
void printLineThree(String text)
{
  lcd.setCursor(0, 2);
  lcd.print(text);
}
void printLineFour(String text)
{
  lcd.setCursor(0, 3);
  lcd.print(text);
}

void setup()
{
  Serial.begin(9600); //Serial bandbreedte instellen op 9600mhz.

  //Instellingen van het LCD scherm
  lcd.init();
  lcd.backlight();

  nfc.begin(); //Starten van de NFC module.

//De button instellen
   pinMode(button, INPUT);
   digitalWrite(button, HIGH);

  //Het volgende geeft wat informatie over de PN532 NFC module. Het is niet verplicht maar handig om eventueel te debuggen.
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    Serial.print("Didn't find PN53x board");
    while (1)
      ;
  }
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);

  nfc.SAMConfig();

  //Standaard waardes van het LCD scherm.
  printLineOne("Welkom bij Aventus");
  printLineTwo("Apeldoorn | LVDM");
  printLineFour("Scan uw tag");
}

void loop()
{
  boolean success;
  success =  nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50); //timeout 0 means no timeout - will block forever.

  if (success)
  { //Wanneer een NFC Tag gescand wordt
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000)
    { //Dit is een soort vervanging voor de delay() functie. Het werkt uiteindelijk hetzelfde maar zorgt er niet voor dat de rest van het programma stil gezet wordt.

      previousMillis = currentMillis; //Opnieuw de waarde van de delay opslaan.

      //De UID converteren naar een String.
      for (uint8_t i = 0; i < uidLength; i++)
      {
        uidString += String(uid[i], HEX) + " ";
      }

      uidString.toUpperCase();

      printLineFour(uidString);
      Serial.println(uidString);

      uidString = "";
    }
  }

//Waneer de button wordt ingeklikt
if (digitalRead(button)==LOW){
      unsigned long currentMillisButton = millis();
    if (currentMillisButton - previousMillisButton >= 100)
    { //Zelfde delay systeem zodat de button maar 1x per seconde ingeklikt kan worden
      previousMillisButton = currentMillisButton;
    Serial.println("Button is pressed");
    printLineFour("Scan uw tag");

    }
  }
}
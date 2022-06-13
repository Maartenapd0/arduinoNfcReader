#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
String uidString;

unsigned long previousMillis = 0; 
unsigned long previousMillisButton = 0; 

//Button
int button = 2;

LiquidCrystal_I2C lcd(0x27, 20, 4);

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

static void handleClick()
{
  Serial.println("Clicked!");
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  nfc.begin();

   pinMode(button, INPUT);
  digitalWrite(button, HIGH); //activate arduino internal pull up

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

  printLineOne("Welkom bij Aventus");
  printLineTwo("Apeldoorn | LVDM");
  printLineFour("Scan uw tag");
}

void loop()
{
  boolean success;
  // success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  success =  nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50); //timeout 0 means no timeout - will block forever.

  if (success)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000)
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

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

if (digitalRead(button)==LOW){
      unsigned long currentMillisButton = millis();
    if (currentMillisButton - previousMillisButton >= 100)
    {
      previousMillisButton = currentMillisButton;
    Serial.println("Button is pressed");
    printLineFour("Scan uw tag");

    }
  }
}
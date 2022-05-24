#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
#include "OneButton.h"

PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
String uidString;

OneButton btn3(3, true);

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

static void handleClick() {
  Serial.println("Clicked!");
}

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  nfc.begin();

  btn3.attachClick(handleClick);

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
  unsigned long test = millis();
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  unsigned long test3 test millis();

  if (success)
  {
    for (uint8_t i = 0; i < uidLength; i++)
    {
        uidString += String(uid[i], HEX) + " ";
    }
    uidString.toUpperCase();
    printLineFour(uidString);
    uidString = "";

    // delay(1000);
  }
  btn3.tick();
}


// void run(){
//   btn3.tick();
// }

// std::thread thread_obj(btntick);
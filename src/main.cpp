#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

#define TRANSMITTER 3
#define RECEIVER 5

LiquidCrystal_I2C lcd(0x27, 20, 4);

IRrecv receiver(RECEIVER);
IRsend sender;

struct storedIRDataStruct {
    IRData receivedIRData;
    uint8_t rawCode[RAW_BUFFER_LENGTH];
    uint8_t rawCodeLength;
} results;

void setup() {
  pinMode(RECEIVER, INPUT);
  pinMode(TRANSMITTER, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("  IR Retransmitter");

  lcd.setCursor(0, 2);
  lcd.print("Status:");

  receiver.begin(RECEIVER, true);
  sender.begin(true);
}

void loop() {
  if (receiver.available()) {
    results.receivedIRData = *receiver.read();
    results.rawCodeLength = receiver.results.rawlen - 1;
    receiver.compensateAndStoreIRResultInArray(results.rawCode);
    
    lcd.setCursor(8, 2);
    lcd.print("Received");
    receiver.resume();
    delay(5000);

    lcd.setCursor(8, 2);
    lcd.print("Sending ");
    
    sender.sendRaw(results.rawCode, results.rawCodeLength, 38);
    delay(1000);
  }

  lcd.setCursor(8, 2);
  lcd.print("Waiting ");
  delay(5000);
}

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const uint64_t address = 0xF0F0F0F0E1LL;

void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.setChannel(120);
  radio.setPayloadSize(4);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[4] = "";
    radio.read(&text, sizeof(text));
    Serial.print((int)text[0]);
    Serial.print(":");
    Serial.print((int)text[1]-90);
     Serial.print(":");
    Serial.print((int)text[2]);
     Serial.print(":");
    Serial.print((int)text[3]);
    Serial.println("");
  }
}
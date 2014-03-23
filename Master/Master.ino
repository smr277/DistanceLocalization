#include <SPI.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <nRF24L01.h>

#define MICRO 1000000
#define CLICK_DELAY 0.05 * MICRO

IntervalTimer myTimer;

void setup() {
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  pinMode(4, OUTPUT);
  digitalWriteFast(4, LOW);
  Serial.begin(115200);
}

int offset = 0; 
long timeUntilSend; 

unsigned long time;
void loop() {
  noInterrupts();
  time = actualTime() + timeUntilSend;
  timeUntilSend = micros();
  byte data[Mirf.payload];
  data[0] = time & 0xFF;
  data[1] = (time >> 8) & 0xFF;
  data[2] = (time >> 16) & 0xFF;
  data[3] = (time >> 24) & 0xFF;
  Mirf.setTADDR((byte *)"clien");
  Mirf.send(data);
  myTimer.end();
  myTimer.begin(pulseOut, CLICK_DELAY);
  offset = micros();
  timeUntilSend = timeUntilSend - offset;
  interrupts();
  //attach Interupt for when we receive something
  //send out our current time (in blips)
  //when receive something, calculate difference in blips
  //set delay on the system (take into account delay between receive and send -> 10msec 
  //wait a specified amount of time, then flash LED
  delay(5000);
}

void pulseOut(void) {
  digitalWriteFast(4, HIGH);
  delayMicroseconds(30);
  digitalWriteFast(4, LOW); 
  myTimer.end();
  myTimer.begin(pulseOut, CLICK_DELAY);
}

unsigned long actualTime(void) {
  return micros() - offset;
}


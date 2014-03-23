#include <SPI.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <nRF24L01.h>

#define MICRO 1000000
#define CLICK_DELAY 0.05 * MICRO

IntervalTimer myTimer;
long time1;
long time2;
float skew;
void setup(void) {
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clien"); 
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  pinMode(4, OUTPUT);
  digitalWriteFast(4, LOW);
  Serial.begin(115200);
}
unsigned long offset;
void loop(void) {
   byte data[Mirf.payload];
  unsigned long recTime;
  if (Mirf.dataReady()) {
    noInterrupts();
    //time1 = actualTime();
    offset = micros();
    myTimer.end();
    myTimer.begin(pulseOut, CLICK_DELAY - 210);
    Mirf.getData(data);
    recTime = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
    //skew = (float)(time1-recTime)/(float)(10*MICRO);
    interrupts();
  }
  delayMicroseconds(10);
}

unsigned long actualTime(void) {
  return (long)((float)(1.0-0.000005348996271)*(float)(micros() - offset));
}

void pulseOut(void) {
  digitalWriteFast(4, HIGH);
  delayMicroseconds(30);
  digitalWriteFast(4, LOW); 
  myTimer.end();
  myTimer.begin(pulseOut, CLICK_DELAY);
}



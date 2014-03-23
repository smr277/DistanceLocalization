#define MICRO 1000000
#define CLICK_DELAY 0.0005 * MICRO
#define DELAY_SIGNAL 40

IntervalTimer timer;
void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWriteFast(4, LOW);
  digitalWriteFast(5, LOW);
  digitalWriteFast(5, HIGH);
  timer.begin(pulse, CLICK_DELAY-DELAY_SIGNAL);
}

void loop() {
}

void pulse() {
  digitalWriteFast(5, LOW);
  timer.end();
  timer.begin(pulse,CLICK_DELAY);
  digitalWriteFast(4,HIGH);
  delayMicroseconds(20);
  digitalWriteFast(4,LOW);
}

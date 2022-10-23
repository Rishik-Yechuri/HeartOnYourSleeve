#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

#include <LedControl.h>

int DIN = 12; 
int CS =  10;
int CLK = 11;

const int PulseWire = 3;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 610;           // Determine which Signal to "count as a beat" and which to ignore.

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
LedControl lc = LedControl(DIN,CLK,CS,0);

byte large[8] = {0x66,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C,0x18,};
byte med[8] = {0x00,0x00,0x66,0x7E,0x7E,0x3C,0x18,0x00,};
byte small[8] = {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,};
byte empty[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
int bpm = 0;
double bps = 0;
double secondsPerBeat = 0;

void setup() {
  Serial.begin(9600);
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,15);       // Set the brightness to maximum value
  lc.clearDisplay(0);          // and clear the display

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);  

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {
  delay(10);
  bpm = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
  bps = bpm / 60;
  secondsPerBeat = 1000/bps;

  Serial.println(bpm);
  if(secondsPerBeat < 10000){
  if(bpm <120){
    smallLoop(secondsPerBeat);
  }else if(bpm<205){
    smallMedLoop(secondsPerBeat);
  }else{
    smallLargeLoop(secondsPerBeat);
  }
  }
}
void smallLoop(double rate){
  printByte(empty);
  delay(round(0.5*rate));
  printByte(small);
  delay(round(0.5*rate));
}
void smallMedLoop(double rate) {
 
  printByte(small);
  delay(round(0.5*rate));
  printByte(med);
  delay(round(0.5*rate));
}
void smallLargeLoop(double rate){
  printByte(small);
  delay(round(0.25*rate));
  printByte(med);
  delay(round(0.20*rate));
  printByte(large);
  delay(round(0.35*rate));
  printByte(med);
  delay(round(0.20*rate));
}
void printByte(byte character []) {

int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }

}

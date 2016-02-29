#include <DallasTemperature.h>
#include <OneWire.h> 
#include <SevenSeg.h>

SevenSeg disp(8,14,6,A1,23,7,5);
const int numOfDigits=4;
int digitPins[numOfDigits]={16,17,3,4};

int DS18S20_Pin = A6; //DS18S20 Signal pin on digital 0

//Temperature chip i/o
OneWire ds(DS18S20_Pin); // on digital pin 0

DallasTemperature sensors(&ds);
DeviceAddress deviceAddress;

int potPin = A7;
int previousVal = 0;
int wait = 0;
float alarmTemp = 0;

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void writeScroll(String message, int displayTime = 5000){
  int scroll = 125;
  int len = message.length();
  int pos = 0;
  long previousMillis = millis();
  if(len <= 4){
    while(pos != 1){
      disp.write(message);
      unsigned long currentMillis = millis();
      if(currentMillis - previousMillis > displayTime){
        pos = 1;
      }
    }
  }
  else{
    while(pos <= len - 4){
      unsigned long currentMillis = millis();
      String msg = message.substring(pos, pos+4);
      disp.write(msg);
      if(pos == 0){
        scroll = 750;
      }
      else if(pos == len - 4){
        scroll = 500;
      }
      else{
        scroll = 125;
      }
      if(currentMillis - previousMillis > scroll){
        pos = pos + 1;
        previousMillis = currentMillis;
      }
    }
  }
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(0, HIGH);
    delayMicroseconds(tone);
    digitalWrite(0, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup(void) {
  disp.setDigitPins(numOfDigits, digitPins);
  disp.setDPPin(22);
  disp.setRefreshRate(150);
  sensors.requestTemperatures();
  sensors.setWaitForConversion(false);
  sensors.getAddress(deviceAddress, 0);
  pinMode(A7, INPUT);
}

void loop(void) {
  float temperature = sensors.getTempC(deviceAddress);
  sensors.requestTemperatures();
  disp.write(temperature);

  int currentVal = analogRead(potPin);
  int valDif = currentVal - previousVal;
  if(abs(valDif) > 10){
    long cMillis = millis();
    long pMillis = cMillis;
    while(abs(valDif) > 10 || cMillis - pMillis < 2000){
      currentVal = analogRead(potPin);
      float alarmTemp = 0 + (currentVal * .04887585532);
      if(alarmTemp > 0.001){
        disp.write(alarmTemp);
      }
      valDif = currentVal - previousVal;
      previousVal = currentVal;
      cMillis = millis();
    }
  }
  
  float alarmTemp = 0 + (currentVal * .04887585532);
  while(alarmTemp <= temperature) {
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // rest
      } else {
        playNote(notes[i], beats[i] * tempo);
      }
  
      // pause between notes
      delay(tempo / 2); 
    }
  }
  
  long cMillis = millis();
  long pMillis = cMillis - 1;
  while(cMillis - pMillis < 2000 || abs(valDif) > 50){
    valDif = currentVal - previousVal;
    currentVal = analogRead(potPin);
    disp.write(currentVal);
    previousVal = currentVal;
    cMillis = millis();
  }
}


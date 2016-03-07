#include <DallasTemperature.h>
#include <OneWire.h> 
#include <SevenSeg.h>


// Pin assignments

SevenSeg disp(8,14,6,A1,23,7,5);
const int numOfDigits=4;
int digitPins[numOfDigits]={16,17,3,4};

int DS18S20_Pin = 0;
OneWire ds(DS18S20_Pin);

int potPin = A7;

int speakerPin = 10;


// Temperature chip i/o

DallasTemperature sensors(&ds);
DeviceAddress deviceAddress;


// Input cleaning

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


// Alarm Temp

double temperature = 0;
int previousVal = 0;
double alarmTemp = 0;
int currentVal = 0;


int cleanInput(int inputPin) {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPin);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  currentVal = total / numReadings;
  return currentVal;
}


void writeScroll(String message, int displayTime = 5000){
  int scroll = 125;
  int len = message.length();
  int pos = 0;
  long previousMillis = millis();
// simply display message if display is big enough
  if(len <= numOfDigits){
    while(pos != 1){
      disp.write(message);
      unsigned long currentMillis = millis();
      if(currentMillis - previousMillis > displayTime){
        pos = 1;
      }
    }
  }
// scroll if needed
  else{
    while(pos <= len - numOfDigits){
      unsigned long currentMillis = millis();
      String msg = message.substring(pos, pos+numOfDigits);
      disp.write(msg);
      if(pos == 0){
        scroll = 750;  // long first pause for readability
      }
      else if(pos == len - numOfDigits){
        scroll = 500;  // long last pause for readability
      }
      else{
        scroll = 125;  // fast pauses
      }
      if(currentMillis - previousMillis > scroll){
        pos = pos + 1;
        previousMillis = currentMillis;
      }
    }
  }
}


// Alarm sound

int length = 4; // the number of notes
char notes[] = "Cafc"; // a space represents a rest
int beats[] = { 1, 1, 1, 1};
int tempo = 100;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
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
  pinMode(speakerPin, OUTPUT);
  pinMode(A7, INPUT);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop(void) {
  currentVal = cleanInput(potPin);

  int valDif = currentVal - previousVal;
// show temp when not changing the alarm value
  while(abs(valDif) <= 5){
    previousVal = currentVal;

    temperature = sensors.getTempC(deviceAddress);
    sensors.requestTemperatures();
    writeScroll(" HOT" + String(temperature));

    currentVal = cleanInput(potPin);
    valDif = currentVal - previousVal;
    double alarmTemp = 10 + (currentVal * .04887585532);
  // when temp reaches alarm temp play alarm
    while(alarmTemp < temperature) {
      for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
          //delay(beats[i] * tempo); // rest
        } else {
          playNote(notes[i], beats[i] * tempo);
        }
        // pause between notes
        //delay(tempo / 2); 
      }
      currentVal = cleanInput(potPin);
      alarmTemp =  10 + (currentVal * .04887585532);
      temperature = sensors.getTempC(deviceAddress);
      sensors.requestTemperatures();
    }
  }
  long cMillis = millis();
  long pMillis = cMillis;
// when pot is being changed, display corresponding alarm temp
  while(abs(valDif) > 5 || cMillis - pMillis < 2000){
    currentVal = cleanInput(potPin);
    if(currentVal < 1) {currentVal = 1;}
    double alarmTemp = 10 + (currentVal * .04887585532);
    double dispTemp = round(alarmTemp*10)/10;
    disp.write(dispTemp);
    if(cMillis - pMillis > 250) {
      valDif = currentVal - previousVal;
      previousVal = currentVal;
      if(abs(valDif) > 5) {
        pMillis = millis();
      }
      cMillis = millis();
    }
  }
}


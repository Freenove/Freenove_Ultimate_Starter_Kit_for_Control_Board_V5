/**********************************************************************
  Filename     : Sketch_21.2.2_4_Digit_7-Segment_Display
  Description  : 4-digit 7-segment Display
  Auther       : www.freenove.com
  Modification : 2024/08/05
  CallbackTimerR4 is provided by Kingsman
  Urls         : https://github.com/embedded-kiddie/CallbackTimerR4
**********************************************************************/

#include "Arduino.h"
#include "CBTimer.h"


int latchPin = 12;              // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 13;              // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 11;               // Pin connected to DS of 74HC595（Pin14）
int comPin[] = { 7, 6, 5, 4 };  // Common pin (anode) of 4 digit 7-segment display

int second = 0;  // Define variables stored record time

// Define the encoding of characters 0-F for the common-anode 7-Segment Display
byte num[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e };

void setup() {
  // set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(comPin[i], OUTPUT);
  }
  static CBTimer timer;
  timer.begin(1000, callback_func);
}

void loop() {
  // Calculate the seconds of each digit number
  byte bit[4];
  bit[0] = second % 10;
  bit[1] = second / 10 % 10;
  bit[2] = second / 100 % 10;
  bit[3] = second / 1000 % 10;
  // Show seconds
  for (int i = 0; i < 4; i++) {
    // Select a single 7-segment display
    chooseCommon(i);
    // Send data to 74HC595
    writeData(num[bit[3 - i]]);
    delay(5);
    // Clear the display content
    writeData(0xff);
  }
}

void callback_func(void) {
  second++;  // second plus 1
}

void chooseCommon(byte com) {
  // Close all single 7-segment display
  for (int i = 0; i < 4; i++) {
    digitalWrite(comPin[i], LOW);
  }
  // Open the selected single 7-segment display
  digitalWrite(comPin[com], HIGH);
}

void writeData(int value) {
  // Make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // Make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}

/*
Davicom Products SMI access library
Author: Russell Liu
Copyright: GPL V2

This library provide a SMI access to read/write register
from/to the DMavicom products with SMI.

2014-9-18 <russell_liu@davicom.com.tw>
*/

#include "smi.h"

#define SMI_HIGH 1
#define SMI_LOW 0

#define SMI_OUTPUT 1
#define SMI_INPUT 0

static int _clockPin;
static int _dataPin;

static void start();
static void opRead();
static void opWrite();
static void putAddress(uint8_t HB, uint8_t LB);
static void readTurnA();
static void writeTurnA();
static void getData(uint8_t target[]);
static void sendData(uint8_t target[]);

static void smiPinMode(const uint8_t pin, const char state){
	if (state)		{ //output
		SMI_PORTDDR |= (1<<pin);
	}
	else{
		SMI_PORT &= ~(1<<pin);
		SMI_PORTDDR &= ~(1<<pin);//input
	}
}
static void smiPinWrite (const uint8_t pin, const char state){
	if (state)		{
		SMI_PORT |= (1<<pin);
	}
	else{
		SMI_PORT &= ~(1<<pin);
	}
}

static char smiPinRead(uint8_t pin){
	return ((SMI_PIN>>pin) & 1);
}

void SMIinit(uint8_t clockPin, uint8_t dataPin) {
	_clockPin = clockPin;
	_dataPin = dataPin;
}

void smiWrite(uint8_t HB, uint8_t LB, uint8_t target[]) {
	start();
	//OP code
	opWrite();
	putAddress(HB, LB);     //send register address
	writeTurnA();
	sendData(target);
}

void smiRead(uint8_t HB, uint8_t LB, uint8_t target[]) {
	start();
	//OP code
	opRead();
	putAddress(HB, LB);  	//send register address
	readTurnA();
	getData(target);
}

// used to send a clock on clock pin
void clockPulse() {
	smiPinWrite (_clockPin, SMI_HIGH);
	smiPinWrite (_clockPin, SMI_LOW);
}

// Preamble + SFD
static void start() {
	//Preamble
	smiPinWrite (_dataPin, SMI_HIGH);
	for (uint8_t smi_i=0; smi_i<32; smi_i++) {
		clockPulse();
	}
	//SFD
	smiPinWrite (_dataPin, SMI_LOW);
	clockPulse();
	smiPinWrite (_dataPin, SMI_HIGH);
	clockPulse();
}

static void opRead() {
	//data=10
	smiPinWrite (_dataPin, SMI_HIGH);
	clockPulse();
	smiPinWrite (_dataPin, SMI_LOW);
	clockPulse();
}

static void opWrite() {
	//data=01
	smiPinWrite (_dataPin, SMI_LOW);
	clockPulse();
	smiPinWrite (_dataPin, SMI_HIGH);
	clockPulse();
}

// Send register address
static void putAddress(uint8_t HB, uint8_t LB) {
	int8_t smi_i;
	for (smi_i=1; smi_i>=0; smi_i--){
		smiPinWrite (_dataPin, HB &(1<<smi_i));
		clockPulse();
	}
	for (smi_i=7; smi_i>=0; smi_i--){
		smiPinWrite (_dataPin, LB &(1<<smi_i));
		clockPulse();
	}
}

static void readTurnA() {
	smiPinMode(_dataPin, SMI_INPUT);
	clockPulse();
}

static void writeTurnA() {
	smiPinWrite (_dataPin, SMI_HIGH);
	clockPulse();
	smiPinWrite (_dataPin, SMI_LOW);
	clockPulse();
}

static void getData(uint8_t target[]) {
	int8_t smi_i, smi_j;
	for (smi_j=1; smi_j>=0; smi_j--) {
		target[smi_j]=0; // reset data array
		for (smi_i=7; smi_i>=0; smi_i--) {
			smiPinWrite (_clockPin, SMI_HIGH);
			if (smiPinRead(_dataPin)==SMI_HIGH) {
				target[smi_j]=(target[smi_j] + (1<<smi_i));
			}
			smiPinWrite (_clockPin, SMI_LOW);
		}
	}
	smiPinMode(_dataPin,SMI_OUTPUT);
	smiPinWrite(_dataPin,SMI_HIGH);
}

static void sendData(uint8_t target[]) {
	int8_t smi_i, smi_j;
	for (smi_j=1; smi_j>=0; smi_j--) {
		for (smi_i=7; smi_i>=0; smi_i--) {
			if ((target[smi_j] & (1<<smi_i))==0) {
				smiPinWrite (_dataPin, SMI_LOW);
				} else {
				smiPinWrite (_dataPin, SMI_HIGH);
			}
			clockPulse();
		}
	}
}
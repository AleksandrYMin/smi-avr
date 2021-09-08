/*
* hubsetup.c
*
* Created: 16.06.2021 16:22:37
* Author : lab162
*/
#define F_CPU 9600000L
#define SMI_CLOCK_PIN 3
#define SMI_DATA_PIN 4
#define SMI_ADDR 0x01

#include <avr/io.h>
#include <util/delay.h>
#include "smi.h"

void readMultiChip(uint8_t device, uint8_t reg, uint8_t regvalue[2]){
	const uint8_t addr = SMI_ADDR;
	uint8_t proxData [2];
	uint8_t regML = addr<<5;
	
	proxData[1] = 0 | (1<<7) | (1<<4) | (2<<2) |  device>>3;
	proxData[0] = device<<5 | reg;
	smiWrite(0, regML, proxData);

	regML |= 0x01;
	smiRead(0, regML, regvalue);
}

void writeMultiChip(uint8_t device, uint8_t reg, uint8_t regvalue[2]){
	const uint8_t addr = SMI_ADDR;
	uint8_t proxData [2];
	uint8_t regML = (addr<<5) + 1;

	smiWrite(0, regML, regvalue);
	
	proxData[1] = 0 | (1<<7) | (1<<4) | (1<<2) | device>>3;
	proxData[0] = device<<5 | reg;
	
	regML--;
	smiWrite(0, regML, proxData);
}

void enableSFP(const uint8_t n){ //n - ftp interface address
	uint8_t regvalue[2] = {0, 0};
	uint8_t reg = 0;
	uint8_t phyAddress = 0x0C+n;
	reg = 0x10 ;

	regvalue[1] = 0;
	regvalue[0] = 0xd9;
	writeMultiChip(phyAddress, reg, regvalue);

	reg = 0x0;
	regvalue[1] = 0x91;
	regvalue[0] = 0x40;
	writeMultiChip(phyAddress, reg, regvalue);

	reg = 0x4;
	regvalue[1] = 0x00;
	regvalue[0] = 0x20;
	writeMultiChip(phyAddress, reg, regvalue);

	reg = 0x0;
	regvalue[1] = 0x13;
	regvalue[0] = 0x40;
	writeMultiChip(phyAddress, reg, regvalue);

	phyAddress = 0x14+n;
	reg = 0x14;
	regvalue[1] = 0xc4;
	regvalue[0] = 0x03;
	writeMultiChip(phyAddress, reg, regvalue);

	reg = 0x00;
	readMultiChip(phyAddress, reg, regvalue);
	
	regvalue[0] |= 0x05;
	regvalue[0] &= ~(1<<1);
	writeMultiChip(phyAddress, reg, regvalue);
}

void __attribute__ ((__noinline__, __optimize__("Os"))) delay_300ms() {
	_delay_ms(300);
}

int main(void)
{
	SMI_PORT = 0x14;
	SMI_PORTDDR |= 0x1C;
	SMIinit(SMI_CLOCK_PIN, SMI_DATA_PIN);
	delay_300ms();
	enableSFP(0);
	enableSFP(1);
	enableSFP(2);
	enableSFP(3);
	SMI_PORTDDR = 0x04;
	SMI_PORT = 0x0;
	/* Replace with your application code */
	while (1)
	{
		SMI_PORT ^= (1<<2);
		delay_300ms();
		delay_300ms();
		delay_300ms();
	}
}
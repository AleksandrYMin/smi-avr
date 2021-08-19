#ifndef smi_h
#define smi_h
#include <avr/io.h>

#define SMI_PORTDDR DDRB
#define SMI_PORT PORTB

void SMIinit(uint8_t clockPin, uint8_t dataPin);

void smiWrite(uint8_t HB, uint8_t LB, uint8_t target[]);

void smiRead(uint8_t HB, uint8_t LB, uint8_t target[]);


#endif
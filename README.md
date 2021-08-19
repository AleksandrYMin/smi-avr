# smi-avr
SMI read and write functions for 8-bit AVR microcontrollers (ATtiny13).<br>
DDR and PORT registers must be specified in smi.h (SMI_PORTDDR and SMI_PORT) according to the location of SMI pins.<br>
<br>
<b>void SMIinit(uint8_t clockPin, uint8_t dataPin)</b> - initializes pin variables<br>
<b>void smiWrite(uint8_t HB, uint8_t LB, uint8_t target[])</b> - writes data to SMI bus<br>
<b>void smiRead(uint8_t HB, uint8_t LB, uint8_t target[])</b> - read data from SMI bus<br>
<br>
In both smiRead and smiWrite functions:<br>
<b>HB[1:0]</b> - bits [4:3] of PHY address, other bits are not used<br>
<b>LB[7:5]</b> - bits [2:0] of PHY address<br>
<b>LB[4:0]</b> - bits [5:0] of register address<br>
<b>target[]</b> - 2-byte data array<br>
<br>
MARVELL Multichip functions:<br>
<b>void readMultiChip(uint8_t device, uint8_t reg, uint8_t regvalue[2])</b><br>
<b>void writeMultiChip(uint8_t device, uint8_t reg, uint8_t regvalue[2])</b><br>
<b>uint8_t device</b> - PHY address;<br>
<b>uint8_t reg</b> - register addres;<br>
<b>uint8_t regvalue[2]</b> - 2-byte data array<br>
<b>#define SMI_ADDR 0x02</b> - multichip address of MARVELL IC
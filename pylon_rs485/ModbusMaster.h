#define SERIAL_PORT Serial1
#include "application.h"
#ifndef ModbusMaster_h
#define ModbusMaster_h

#define lowWord(ww) ((uint16_t) ((ww) & 0xFFFF))
#define highWord(ww) ((uint16_t) ((ww) >> 16))
#define LONG(hi, lo) ((uint32_t) ((hi) << 16 | (lo)))
#define LowByte(w) ((uint8_t) (((unsigned)w) & 0xff)) 
#define HighByte(w) ((uint8_t) (((unsigned)w&0xff00) >> 8))
#define bitRead(value, bit)			(((value) >> (bit)) & 0x01)
#define bitSet(value, bit)			 ((value) |= (1UL << (bit)))
#define bitClear(value, bit)		   ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

	extern void node();
	extern void node_begin(uint16_t);
	void idle(void (*)());
	static const uint8_t ku8MBIllegalFunction			= 0x01;
	static const uint8_t ku8MBIllegalDataAddress		 = 0x02;
	static const uint8_t ku8MBIllegalDataValue		   = 0x03;
	static const uint8_t ku8MBSlaveDeviceFailure		 = 0x04;
	static const uint8_t ku8MBSuccess					= 0x00;
	static const uint8_t ku8MBInvalidSlaveID			 = 0xE0;
	static const uint8_t ku8MBInvalidFunction			= 0xE1;
	static const uint8_t ku8MBResponseTimedOut		   = 0xE2;
	static const uint8_t ku8MBInvalidCRC				 = 0xE3;
	extern uint16_t node_getResponseBuffer(uint8_t);
	extern void node_enableTXpin(uint8_t);
	extern uint8_t  node_readInputRegisters(uint16_t, uint8_t);
	
#endif

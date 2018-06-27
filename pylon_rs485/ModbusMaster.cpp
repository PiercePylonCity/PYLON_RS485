#include "ModbusMaster.h"

USARTSerial MBSerial =  SERIAL_PORT;
uint8_t MBUseEnablePin = 0;			 ///< Should a TX_ENABLE pin be used? 0 = No, 1 = Yes
uint8_t MBTXEnablePin = 7;
	uint8_t  _u8SerialPort;									  ///< serial port (0..3) initialized in constructor
	uint8_t  _u8MBSlave;										 ///< Modbus slave (1..255) initialized in constructor
	uint16_t _u16BaudRate;									   ///< baud rate (300..115200) initialized in begin()
	static const uint8_t ku8MaxBufferSize				= 64;   ///< size of response/transmit buffers	
	uint16_t _u16ReadAddress;									///< slave register from which to read
	uint16_t _u16ReadQty;										///< quantity of words to read
	uint16_t _u16ResponseBuffer[ku8MaxBufferSize];			   ///< buffer to store Modbus slave response; read via GetResponseBuffer()
	uint16_t _u16WriteAddress;								   ///< slave register to which to write
	uint16_t _u16WriteQty;									   ///< quantity of words to write
	uint16_t _u16TransmitBuffer[ku8MaxBufferSize];			   ///< buffer containing data to transmit to Modbus slave; set via SetTransmitBuffer()
	uint16_t* txBuffer; // from Wire.h -- need to clean this up Rx
	uint8_t _u8TransmitBufferIndex;
	uint16_t u16TransmitBufferLength;
	uint16_t* rxBuffer; // from Wire.h -- need to clean this up Rx
	uint8_t _u8ResponseBufferIndex;
	uint8_t _u8ResponseBufferLength;
	
	static const uint8_t ku8MBReadInputRegisters		 = 0x04; ///< Modbus function 0x04 Read Input Registers
	static const uint8_t ku8MBResponseTimeout			= 2000;  ///< Modbus timeout [milliseconds]
	uint8_t ModbusMasterTransaction(uint8_t u8MBFunction);
	void (*_idle)();
uint16_t word(uint8_t high, uint8_t low) {
	uint16_t ret_val = low;
	ret_val += (high << 8);
	return ret_val;
}

uint16_t _crc16_update(uint16_t crc, uint8_t a) {
	int i;
	
	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
		if (crc & 1)
			crc = (crc >> 1) ^ 0xA001;
		else
			crc = (crc >> 1);
	}
	return crc;
}

void node(void) {
	_u8SerialPort = 1;
	_u8MBSlave = 1;
}

void node_begin(uint16_t u16BaudRate) {
	_u8TransmitBufferIndex = 0;
	u16TransmitBufferLength = 0;
	
	switch(_u8SerialPort) {
		case 1:
			MBSerial = SERIAL_PORT;
			break;
		default:
			MBSerial = SERIAL_PORT;		// Default to Serial1 for Spark Core
		break;
	}
	MBSerial.begin(u16BaudRate,SERIAL_8E1);
}

void node_enableTXpin(uint8_t pin) {
	pinMode(pin, OUTPUT);
	MBTXEnablePin = pin;
	MBUseEnablePin = 1; 
}

void idle(void (*idle)()) {
	_idle = idle;
}

uint16_t node_getResponseBuffer(uint8_t u8Index) {
	if (u8Index < ku8MaxBufferSize) {
		return _u16ResponseBuffer[u8Index];
	} else {
		return 0xFFFF;
	}
}

uint8_t node_readInputRegisters(uint16_t u16ReadAddress,
  uint8_t u16ReadQty) {
	_u16ReadAddress = u16ReadAddress;
	_u16ReadQty = u16ReadQty;
	return ModbusMasterTransaction(ku8MBReadInputRegisters);
}

uint8_t ModbusMasterTransaction(uint8_t u8MBFunction) {
	uint8_t u8ModbusADU[256];
	uint8_t u8ModbusADUSize = 0;
	uint8_t i, u8Qty;
	uint16_t u16CRC;
	uint32_t u32StartTime;
	uint8_t u8BytesLeft = 9;
	uint8_t u8MBStatus = ku8MBSuccess;
	
	// assemble Modbus Request Application Data Unit
	u8ModbusADU[u8ModbusADUSize++] = _u8MBSlave;
	u8ModbusADU[u8ModbusADUSize++] = u8MBFunction;
  
	switch(u8MBFunction) {
		case ku8MBReadInputRegisters:
			u8ModbusADU[u8ModbusADUSize++] = HighByte(_u16ReadAddress);
			u8ModbusADU[u8ModbusADUSize++] = LowByte(_u16ReadAddress);
			u8ModbusADU[u8ModbusADUSize++] = HighByte(_u16ReadQty);
			u8ModbusADU[u8ModbusADUSize++] = LowByte(_u16ReadQty);
			break;
	}
	u16CRC = 0xFFFF;
	for (i = 0; i < u8ModbusADUSize; i++) {
		u16CRC = _crc16_update(u16CRC, u8ModbusADU[i]);
	}
	u8ModbusADU[u8ModbusADUSize++] = LowByte(u16CRC);
	u8ModbusADU[u8ModbusADUSize++] = HighByte(u16CRC);
	u8ModbusADU[u8ModbusADUSize] = 0;
	
	if (MBSerial.available()) { //Empty the receive buffer before beginning.
		MBSerial.read();
	}
			
	if (MBUseEnablePin == 1) {  //Switch RS485 driver to transmitting mode.
		digitalWrite(MBTXEnablePin, HIGH);  
	}
	for (i = 0; i < u8ModbusADUSize; i++) {
		MBSerial.write(u8ModbusADU[i]);
	}
	u8ModbusADUSize = 0;
	MBSerial.flush(); //Wait for transmission to get completed
	if (MBUseEnablePin == 1) {  //Switch RS485 driver back to receiving mode.
		digitalWrite(MBTXEnablePin, LOW);  
	}
	u32StartTime = millis();
	while (u8BytesLeft && !u8MBStatus) {
		if (MBSerial.available()) {
			u8ModbusADU[u8ModbusADUSize++] = MBSerial.read();
			if (u8ModbusADU[0] == 0) { 
				u8ModbusADUSize--;
				continue;
			}
			u8BytesLeft--;
		} else {
			if (_idle) {
				_idle();
			}
		}
		if (u8ModbusADUSize == 5) {
			if (u8ModbusADU[0] != _u8MBSlave) {
				u8MBStatus = ku8MBInvalidSlaveID;
				break;
			}
			if ((u8ModbusADU[1] & 0x7F) != u8MBFunction) {
				u8MBStatus = ku8MBInvalidFunction;
				break;
			}
			if (bitRead(u8ModbusADU[1], 7)) {
				u8MBStatus = u8ModbusADU[2];
				break;
			}
			switch(u8ModbusADU[1]) {
				case ku8MBReadInputRegisters:
					u8BytesLeft = u8ModbusADU[2];
					break;
			}
		}
		if (millis() > (u32StartTime + ku8MBResponseTimeout)) {
			u8MBStatus = ku8MBResponseTimedOut;
		}
	}
	if (!u8MBStatus && u8ModbusADUSize >= 5) {
		// calculate CRC
		u16CRC = 0xFFFF;
		for (i = 0; i < (u8ModbusADUSize - 2); i++) {
			u16CRC = _crc16_update(u16CRC, u8ModbusADU[i]);
		}
		
		if (!u8MBStatus && (LowByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 2] ||
		HighByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 1])) {
			u8MBStatus = ku8MBInvalidCRC;
		}
	}

	if (!u8MBStatus) {
		switch(u8ModbusADU[1]) {
			case ku8MBReadInputRegisters:
				for (i = 0; i < (u8ModbusADU[2] >> 1); i++) {
					if (i < ku8MaxBufferSize) {
						_u16ResponseBuffer[i] = word(u8ModbusADU[2 * i + 3], u8ModbusADU[2 * i + 4]);
					}
					_u8ResponseBufferLength = i;
				}
				break;
		}
	}
  
	_u8TransmitBufferIndex = 0;
	u16TransmitBufferLength = 0;
	_u8ResponseBufferIndex = 0;
	return u8MBStatus;
}
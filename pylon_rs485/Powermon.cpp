
#include "Powermon.h"
Powermon powermon;
void powermonStart(){
    node();
    node_begin(9600);
	node_enableTXpin(7);  //Pin point to connect to MAX485 enable 
}
void powermonGo(){
    uint16_t a[2];uint8_t result;
    result = node_readInputRegisters(powermon.addrVa,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Va, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrVb,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Vb, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrVc,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Vc, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrIa,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Ia, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrIb,bytesQty);
    a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Ib, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrIc,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Ic, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrP,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.P, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrPa,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Pa, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrPb,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Pb, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrPc,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Pc, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrpfa,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.pfa, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrpfb,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.pfb, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrpfc,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.pfc, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrfreq,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.freq, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrEin,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Ein, a, sizeof(a));
	result = node_readInputRegisters(powermon.addrEout,bytesQty);
	a[1] =node_getResponseBuffer(0);
	a[0] = node_getResponseBuffer(1);
	memcpy(&powermon.Eout, a, sizeof(a));
}
Powermon powermonGet(void){
    return powermon; 
}
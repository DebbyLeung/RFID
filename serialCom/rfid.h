#ifndef MODBUS
#define MODBUS

#include "serialCom_v2.h"

class Rfid: public SerialCom {
public:
	BYTE* track_tags_once();

private:
	BYTE _header[2] = {0x52, 0x46}; //ASCII 'R','F'
	BYTE _track_tags_once_tx[9] = { 0x52, 0x46, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00 };//=46

};



#endif
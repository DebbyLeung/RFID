#include "rfid.h"

BYTE Rfid::track_tags_once() {
	calculateCheckSum(_track_tags_once_tx, 9);
	sendByte(_track_tags_once_tx, 9);
	BYTE val = this->rv_arr[3] << 4 | this->rv_arr[4];
	printf("Torque: %d\n", val);
	return val;
}
#include "rfid.h"

BYTE* Rfid::track_tags_once() {
	calculateCheckSum(_track_tags_once_tx, 9);
	return _track_tags_once_tx;
}
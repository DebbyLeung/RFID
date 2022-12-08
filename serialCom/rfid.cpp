#include "rfid.h"
#include <format>
std::vector<BYTE> xsplit_int(uint16_t val) {
	BYTE high = (BYTE)((val & 0xFF00) >> 8);
	BYTE low = (BYTE)(val & 0x00FF);
	std::vector<BYTE> arr = { high , low };
	return  arr;
}

int Rfid::track_tags_once(std::vector<BYTE> arr) {
	track_tags_once_tx[3] = arr[0];
	track_tags_once_tx[4] = arr[1];
	BYTE* track_tags_once_rx;
	calculateCheckSum(&track_tags_once_tx[0], track_tags_once_tx.size());
	sendByte(&track_tags_once_tx[0], track_tags_once_tx.size());
	readByte(track_tags_once_rx, buffer);

	BYTE val = this->rv_arr[3] << 4 | this->rv_arr[4];
	printf("Torque: %d\n", val);
	return val;
}

std::string Rfid::get_str(char mode) {
	int len;
	BYTE* arr;
	switch (mode) {
	case 's':
		len = track_tags_once_tx.size();
		arr = &track_tags_once_tx[0];
		break;
	case 'r':
		len = write_tag_once.size();
		arr = &write_tag_once[0];
		break;
	
	}
	calculateCheckSum(arr, len);

	//convert string
	std::string str;
	for (int i = 0; i < len; i++) {
		std::string hexi = std::format("{:02x}", *(arr + i));
		str.append("%");
		str.append(hexi);
	}
	return str;

}